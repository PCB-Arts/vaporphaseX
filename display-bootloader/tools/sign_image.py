#!/usr/bin/env python3

from Crypto.Hash import SHA256
from Crypto.PublicKey import ECC
from Crypto.Signature import DSS

# write size of data and program digest to header of the program
# this is different from the header of the file
# this header is verified by the bootloader before starting the image
def write_size_and_digest_in_image_header(data):
    # if the first bytes write VPO we replace the first bytes with the header information
    header_mark = data[0:3]

    if header_mark != bytes("VPO", encoding='ASCII'):
        print ("SKIP size and digest header")
        return

    print ("WRITE size and digest header")

    #clear VPO mark this enables the bootloaders program verification before boot
    data[0:4] = bytearray(4)

    program_data = data[40:]
    h = SHA256.new(program_data)
    data[4:8] = len(program_data).to_bytes(4, byteorder='little')

    data[8:40] = h.digest()

def sign_blob(pri_key_bytes, data):
    key = ECC.import_key(pri_key_bytes)
    h = SHA256.new(data)
    signer = DSS.new(key, 'deterministic-rfc6979')
    signature = signer.sign(h)

    # signature is the straight concatenation of r and s
    return signature + h.digest()

def just_hash(data):
    h = SHA256.new(data)
    signature = bytearray(32 + 32)

    return signature + h.digest()

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--private_key_pem', type=argparse.FileType())
    parser.add_argument('input_image', type=argparse.FileType('rb'))
    parser.add_argument('output_image', type=argparse.FileType('wb'))
    parser.add_argument('--just_hash', action='store_true')
    args = parser.parse_args()

    input_data = bytearray(args.input_image.read())

    write_size_and_digest_in_image_header(input_data)

    if args.just_hash:
        header = just_hash(input_data)
    else:
        pri_key_bytes = args.private_key_pem.read()

        header = sign_blob(pri_key_bytes, input_data)

    args.output_image.write(header + input_data)
