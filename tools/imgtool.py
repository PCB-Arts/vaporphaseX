#!/usr/bin/env python3
"""
Create update images and signing keys, without make, bash, openssl or objcopy,
so it runs the same after a build in the CubeIDE UI and from the Makefiles.
Install tools/requirements.txt before usage.
"""

import argparse
import hashlib
import os
import struct
import sys
from pathlib import Path

try:
    from Crypto.Hash import SHA256
    from Crypto.PublicKey import ECC
    from Crypto.Signature import DSS
except ImportError as e:
    sys.exit(f"error: {e}; install with: pip install -r tools/requirements.txt")

ROOT = Path(__file__).resolve().parent.parent

DEFAULT_PRIV_KEY = ROOT / "keys" / "key.pem"
# Public key include compiled into display-bootloader (command.c)
PUB_KEY_INC = ROOT / "keys" / "include" / "fw_update_sig_pk.c_inc"

# Load address ranges of the memories an image is cut from
INTERNAL_FLASH = (0x08000000, 0x0C000000)
EXTERNAL_FLASH = (0x90000000, 0xA0000000)

# Unsigned images: (project, image name, memory).
# They are written next to the project's .elf as <name>_unsigned.bin.
# The update file is <name>.BIN, the name display-bootloader expects.
IMAGES = (
    # Starts with the image header at 0x08008000, behind core-bootloader
    ("core-fw", "VPX_CORE", INTERNAL_FLASH),
    # Internal flash part, starts with the image header at 0x08040000
    ("display-fw", "VPX_DISP", INTERNAL_FLASH),
    # OSPI content (TouchGFX assets) from 0x90000000
    ("display-fw", "VPX_DISP_ASSETS", EXTERNAL_FLASH),
)

# Image header at the start of the user flash (image_header.s, checked by
# jump_to_user_fw.c): skip indicator (4), image size (4), SHA-256 digest (32).
# An unfilled header starts with this mark.
IMAGE_HEADER_MARK = b"VPX"
IMAGE_HEADER_SIZE = 40


def load_segments(project, config):
    """(load address, data) of all PT_LOAD segments with file content"""
    path = ROOT / project / config / f"{project}.elf"
    if not path.is_file():
        sys.exit(f"error: {path} not found: build {project} ({config}) first")
    data = path.read_bytes()
    if data[:4] != b"\x7fELF" or data[4:6] != b"\x01\x01":
        sys.exit(f"error: {path}: not a 32 bit little endian ELF file")

    (phoff,) = struct.unpack_from("<I", data, 0x1C)
    phentsize, phnum = struct.unpack_from("<HH", data, 0x2A)
    segments = []
    for i in range(phnum):
        p_type, offset, _, paddr, filesz = struct.unpack_from(
            "<5I", data, phoff + i * phentsize
        )
        if p_type == 1 and filesz:
            segments.append((paddr, data[offset : offset + filesz]))
    return segments


def write_images(config):
    """Writes all unsigned images, like objcopy -O binary (gaps filled with zeros)."""
    images = []
    for project, name, (lo, hi) in IMAGES:
        segments = [(a, d) for a, d in load_segments(project, config) if lo <= a < hi]
        if not segments:
            sys.exit(f"error: {project}: nothing loaded at 0x{lo:08X}")
        start = min(a for a, _ in segments)
        image = bytearray(max(a + len(d) for a, d in segments) - start)
        for a, d in segments:
            image[a - start : a - start + len(d)] = d
        path = ROOT / project / config / f"{name}_unsigned.bin"
        path.write_bytes(image)
        print(f"{path.relative_to(ROOT)}: {len(image)} bytes")
        images.append((name, image))
    return images


def fill_image_header(image):
    """Writes size and digest into an unfilled image header and clears the
    mark, which enables display-/core-bootloader's check before boot. Images
    without header (the assets) are left alone."""
    if not image.startswith(IMAGE_HEADER_MARK):
        return False
    program = bytes(image[IMAGE_HEADER_SIZE:])
    image[:IMAGE_HEADER_SIZE] = (
        bytes(4) + len(program).to_bytes(4, "little") + hashlib.sha256(program).digest()
    )
    return True


def write_update_files(config, private_key, out_dir):
    """Update file: ECDSA P-256 signature r and s (32 bytes each), SHA-256 of
    the image, image; checked by display-bootloader (command.c). Without a key
    the signature is all zeros, which is only accepted if the core allows third
    party software."""
    out_dir.mkdir(parents=True, exist_ok=True)
    for name, image in write_images(config):
        note = "image header filled" if fill_image_header(image) else "no image header"
        image = bytes(image)
        if private_key is None:
            signature = bytes(64)
        else:
            signer = DSS.new(private_key, "deterministic-rfc6979")
            signature = signer.sign(SHA256.new(image))
        out = out_dir / f"{name}.BIN"
        out.write_bytes(signature + hashlib.sha256(image).digest() + image)
        print(f"  -> {out.relative_to(ROOT)} ({note})")
    print(f"==> update files in {out_dir.relative_to(ROOT)}")


def import_key(path, private=False):
    if not path.is_file():
        sys.exit(f"error: {path} not found: create it with 'imgtool.py privkey'")
    key = ECC.import_key(path.read_text())
    if key.curve != "NIST P-256":
        sys.exit(f"error: {path}: not a NIST P-256 key")
    if private and not key.has_private():
        sys.exit(f"error: {path} is a public key; signing needs the private key")
    return key


def write_privkey(path):
    path.parent.mkdir(parents=True, exist_ok=True)
    try:
        # O_EXCL: a new key invalidates every image signed with the old one
        fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
    except FileExistsError:
        sys.exit(f"error: {path} exists; refusing to overwrite a signing key")
    with os.fdopen(fd, "w") as f:
        f.write(ECC.generate(curve="P-256").export_key(format="PEM"))
    print(f"==> created {path}")


def write_pubkey(key_path, pem_out):
    public_key = import_key(key_path).public_key()
    if pem_out:
        pem_out.write_text(public_key.export_key(format="PEM") + "\n")
        print(f"==> public key written to {pem_out}")
        return

    # C initializer of struct fw_upd_pk (command.c): x and y as little endian
    # mbedtls_mpi_uint limbs
    def limbs(coordinate):
        data = int(coordinate).to_bytes(32, "little")
        return "\n".join(
            "    BYTES_TO_T_UINT_8( "
            + ", ".join(f"0x{b:02x}" for b in data[i : i + 8])
            + " ),"
            for i in range(0, 32, 8)
        )

    q = public_key.pointQ
    PUB_KEY_INC.parent.mkdir(parents=True, exist_ok=True)
    PUB_KEY_INC.write_text(
        f"\n.x = {{\n{limbs(q.x)}\n}},\n\n.y = {{\n{limbs(q.y)}\n}}\n\n"
    )
    print(f"==> display-bootloader trusts {key_path} once rebuilt")


def main():
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    commands = parser.add_subparsers(dest="command", required=True)

    def add(name, help, config=False, key=False):
        cmd = commands.add_parser(name, help=help)
        if config:
            cmd.add_argument(
                "--config",
                default="Debug",
                help="build configuration (Debug/Release), default Debug",
            )
        if key:
            cmd.add_argument(
                "--key",
                type=Path,
                default=DEFAULT_PRIV_KEY,
                help=f"default {DEFAULT_PRIV_KEY}",
            )
        return cmd

    add("images", "unsigned images (*_unsigned.bin) from the .elf files", config=True)
    add("sign", "images, signed -> out/<config>/signed/", config=True, key=True)
    add(
        "hash",
        "images with zero signature -> out/<config>/hash-only/ "
        "(needs the third party jumper on the core board)",
        config=True,
    )
    add("privkey", "create a signing key (refuses to overwrite)", key=True)
    add(
        "pubkey",
        f"public key include for display-bootloader -> {PUB_KEY_INC.relative_to(ROOT)}",
        key=True,
    ).add_argument(
        "--pem-out",
        type=Path,
        help="write the public key as PEM instead (e.g. to hand out the "
        "production key's public part)",
    )
    args = parser.parse_args()

    if args.command == "images":
        write_images(args.config)
    elif args.command == "sign":
        key = import_key(args.key, private=True)
        write_update_files(args.config, key, ROOT / "out" / args.config / "signed")
    elif args.command == "hash":
        write_update_files(args.config, None, ROOT / "out" / args.config / "hash-only")
    elif args.command == "privkey":
        write_privkey(args.key)
    elif args.command == "pubkey":
        write_pubkey(args.key, args.pem_out)


if __name__ == "__main__":
    main()
