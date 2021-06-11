#!/usr/bin/env python3

import warnings
import struct
from dataclasses import dataclass, fields, field
from enum import Enum
import zlib
import binascii
import math
import string
import time
import random
from contextlib import contextmanager

import serial
from cobs import cobs


class RemoteError(SystemError):
    pass


class ProtocolError(SystemError):
    pass


class PacketType(Enum):
    NOP = 0
    RESPONSE = 1
    IDENTIFY = 2
    REBOOT = 3
    BL_FLASH_CLEAR = 4
    BL_FLASH_WRITE = 5
    PROP_SET = 6
    PROP_GET = 7
    LOG_MSG = 8
    ERROR = 255


class LogPrio(Enum):
    P_CRITICAL = 0
    P_ERROR = 1
    P_WARNING = 2
    P_INFO = 3
    P_DEBUG = 4
    P_DEBUG_FINE = 5
    P_TRACE = 6
    P_USER = 7


class RemoteErrorCode(Enum):
    ES_OK = 0
    PRES_GENERIC_ERROR = 1
    PRES_INVALID_REQUEST = 2
    PRES_SYSTEM_ERROR = 3
    PRES_COMMS_ERROR = 4
    PRES_PROPERTY_NOT_FOUND = 5
    PRES_COMMAND_NOT_FOUND = 6


class VPOPropertyId(Enum):
    TEMP_LIQUID = 0,
    TEMP_COOLANT = 1,


def unpackable(kls):
    kls = dataclass(kls)

    @classmethod
    def format(kls):
        return ''.join(field.type for field in fields(kls) if field.init)

    @classmethod
    def unpack(kls, data, allow_leftovers=False):
        *field_vals, rest = unpack_head(kls.format(), data)

        if allow_leftovers:
            return kls(*field_vals), rest

        assert not rest
        return kls(*field_vals)

    def pack(self):
        return struct.pack(self.format(), *[
            getattr(self, field.name) for field in fields(type(self)) if field.init])

    @property
    def pretty(self, indent=''):
        max_fieldlen = max(len(field.name) for field in fields(self))
        return '\n'.join(f'{indent}{field.name.rjust(max_fieldlen)}: {getattr(self, field.name)}'
                         for field in fields(self) if field.repr)
    kls.format = format
    kls.pretty = pretty
    kls.pack = pack
    kls.unpack = unpack
    return kls


@unpackable
class ResponseIdentify:
    sys_time_ms: 'I'
    device_id_raw: '12s'
    device_id: str = field(init=False)
    flash_size_kbytes: 'H'
    chip_rev: 'H'
    chip_id: 'H'
    fw_id: 'H'
    fw_ver_major: 'H' = field(repr=False)
    fw_ver_minor: 'H' = field(repr=False)
    fw_ver: str = field(init=False)
    hw_id: 'H'
    hw_rev_major: 'H' = field(repr=False)
    hw_rev_minor: 'H' = field(repr=False)
    hw_rev: str = field(init=False)

    def __post_init__(self):
        self.device_id = binascii.hexlify(self.device_id_raw).decode()
        self.hw_rev = f'{self.hw_rev_major}.{self.hw_rev_minor}'
        self.fw_ver = f'{self.fw_ver_major}.{self.fw_ver_minor}'


def calculate_crc(payload):
    return zlib.crc32(bytes(b for x in payload for b in (0, 0, 0, x)))


def unpack_head(fmt, data):
    split = struct.calcsize(fmt)
    return [*struct.unpack(fmt, data[:split]), data[split:]]


def packetize(ptype, payload=b'', pid=0):
    crc_contents = struct.pack('BB', pid, ptype.value) + payload
    crc = struct.pack('I', calculate_crc(crc_contents))
    return cobs.encode(crc + crc_contents) + b'\0'


def depacketize(data):
    _crc, pid, ptype, payload = unpack_head('IBB', data)
    # TODO add CRC & pid checking
    return PacketType(ptype), payload


@contextmanager
def time_op(desc):
    print(desc)
    t0 = time.time()
    yield
    print(
        f'\033[38;5;240mDone. Δt = {(time.time() - t0) * 1000:.3f} ms\033[0m')


startup = time.time()


def _print_line(write, ts, line, width=16):
    h, m, s, ms = int(ts//3600), int((ts//60) % 60), int(ts %
                                                         60), int((ts % 1.0) * 1000)
    timestamp = f'{h: 3d}:{m:02d}:{s:02d}:{ms:03d}'
    line = list(line) + [None]*(width-len(line))
    hexcol = '\033[0m'
    def col(b, s): return s if b != 0 else f'\033[91m{s}{hexcol}'
    hexfmt = '  '.join(
        ' '.join(col(b, f'{b:02x}')
                 if b is not None else '  ' for b in line[i*8:i*8+8])
        for i in range(1 + (len(line)-1)//8))
    asciifmt = ''.join(chr(c) if c is not None and chr(
        c) in string.printable and c >= 0x20 else '.' for c in line)
    write(
        f'\033[38;5;244m{timestamp}  {hexcol}{hexfmt}  \033[38;5;244m|\033[92m{asciifmt}\033[38;5;244m|\033[0m', flush=True, end='')


def hexdump(write, packet, width=16, prefix=''):
    ts = time.time()
    while len(packet) > width:
        write(prefix, end='')
        chunk, packet = packet[:width], packet[width:]
        _print_line(write, ts-startup, chunk, width=width)
        write()
    write(prefix, end='')
    _print_line(write, ts-startup, packet, width=width)
    write(flush=True)


class SerialInterface:
    def __init__(self, port, baudrate=115200):
        self.ser = serial.Serial(port, baudrate=baudrate)
        self.synchronize()

    def synchronize(self):
        self.ser.flushInput()
        for _ in range(5):
            self.send(packetize(PacketType.NOP))

    def receiver_synchronize(self):
        self.ser.flushInput()
        self.ser.read_until(b'\0')

    def receive_packet(self):
        data = self.ser.read_until(b'\0').rstrip(b'\0')
        try:
            data = cobs.decode(data)
            hexdump(print, data, prefix=' <- ')
        except Exception as e:
            hexdump(print, data, prefix=' FE ')
            raise e
        return depacketize(data)

    def receive_response(self):
        while True:
            ptype, payload = self.receive_packet()

            # Allow device to send NOPs to synchronize
            if ptype == PacketType.NOP:
                continue

            # Ignore log messages sent by device
            if ptype == PacketType.LOG_MSG:
                continue

            if ptype == PacketType.ERROR:
                ec, msg = unpack_head('I', payload)
                raise RemoteError(
                    f'Remote error: {RemoteErrorCode(ec)} "{msg.decode()}"')

            if ptype != PacketType.RESPONSE:
                raise ValueError(f'Invalid type {ptype} for response packet')

            return payload

    def send(self, data):
        hexdump(print, data, prefix=' -> ')
        self.ser.write(data)

    def expect_ok(self):
        payload = self.receive_response()
        if payload:
            raise ValueError('Invalid response')

    def cmd_identify(self):
        self.send(packetize(PacketType.IDENTIFY))
        return ResponseIdentify.unpack(self.receive_response(), allow_leftovers=True)

    def cmd_flash_clear(self):
        self.send(packetize(PacketType.BL_FLASH_CLEAR,
                            struct.pack('I', 0xDEAD5732)))
        self.expect_ok()

    def cmd_reboot(self):
        self.send(packetize(PacketType.REBOOT))
        self.expect_ok()

    def cmd_flash_write(self, addr, data):
        self.send(packetize(PacketType.BL_FLASH_WRITE,
                            struct.pack('II', 0x00131200, addr) + data))
        self.expect_ok()

    def flash_user_fw(self, img, block_size=128, base=0):
        with time_op('\033[91mClearing flash\033[38;5;244m...\033[0m'):
            self.cmd_flash_clear()

        for offset in range(0, len(img), block_size):
            block = img[offset:offset+block_size]
            with time_op(f'\033[91mWriting block \033[38;5;244m@{offset:08x}...\033[0m'):
                self.cmd_flash_write(base + offset, block)

        with time_op(f'\033[91mRebooting microcontroller\033[38;5;244m...\033[0m'):
            self.cmd_reboot()

    def packet_loop(self, identity=None, property_proxy=None, log_handler=None):
        while True:
            ptype, payload = self.receive_packet()

            if ptype == PacketType.ERROR:
                ec, msg = unpack_head('I', payload)
                raise RemoteError(
                    f'Remote error: {RemoteErrorCode(ec)} "{msg.decode()}"')

            elif ptype == PacketType.NOP:
                pass

            elif ptype == PacketType.RESPONSE:
                warnings.warn(
                    f'Received unexpected response packet: {payload}')

            elif ptype == PacketType.PROP_SET and property_proxy is not None:
                prop_id, value = unpack_head('H', payload)
                property_proxy[prop_id] = value

            elif ptype == PacketType.PROP_GET and property_proxy is not None:
                prop_id, = struct.unpack('H', payload)
                self.send(packetize(PacketType.RESPONSE,
                                    property_proxy[prop_id]))

            elif ptype == PacketType.IDENTIFY and identity is not None:
                self.send(packetize(PacketType.RESPONSE, identity))

            elif ptype == PacketType.LOG_MSG:
                prio, msg = unpack_head('B', payload)
                prio = LogPrio(prio)
                msg = msg.decode('utf-8')
                if log_handler:
                    log_handler(prio, msg)
                else:
                    print(
                        f'\033[38;5;240m[log] {prio.name[2:]:>10} \033[93m{msg}\033[0m', flush=True)

            else:
                yield ptype, payload

    def mock_bootloader(self):
        import numpy as np
        import hashlib
        image = np.zeros(int(10e6), dtype=np.uint8)
        image_end = 0
        flash_cleared = False

        identity = ResponseIdentify(
            sys_time_ms=0,
            device_id_raw=b'LOOPBACKDBGR',
            flash_size_kbytes=128,
            chip_rev=0x99,
            chip_id=0x2342,
            fw_id=99,
            fw_ver_major=23,
            fw_ver_minor=42,
            hw_id=99,
            hw_rev_major=23,
            hw_rev_minor=42)

        self.receiver_synchronize()
        for ptype, payload in self.packet_loop(identity=identity.pack()):

            if ptype == PacketType.REBOOT:
                if not flash_cleared:
                    warnings.warn('Reboot without flash clear!')

                hash = hashlib.sha256(image[:image_end]).hexdigest()
                print(f'Image size: {image_end} bytes')
                print(f'Image hash: {hash}')
                print('Reboot requested.')
                self.send(packetize(PacketType.RESPONSE))

            elif ptype == PacketType.BL_FLASH_CLEAR:
                magic, = struct.unpack('I', payload)

                image[:] = 0
                image_end = 0
                flash_cleared = True

                if magic != 0xDEAD5732:
                    warnings.warn(f'Invalid clear magic {magic}')

                print(f'Flash clear requested, magic=0x{magic:08x}')
                self.send(packetize(PacketType.RESPONSE))

            elif ptype == PacketType.BL_FLASH_WRITE:
                magic, address, data = unpack_head('II', payload)

                image[address: address+len(data)] = list(data)
                image_end = max(image_end, address+len(data))

                if not flash_cleared:
                    warnings.warn('Flash written before clear!')

                if magic != 0x00131200:
                    warnings.warn(f'Invalid write magic {magic}')

                print(
                    f'Writing flash block @0x{address:08x}, magic=0x{magic:08x}')
                self.send(packetize(PacketType.RESPONSE))

                hash = hashlib.sha256(image[:image_end]).hexdigest()
                print(f'Image size: {image_end} bytes')
                print(f'Image hash: {hash}')

            else:
                warnings.warn(f'Unhandled packet type {ptype}')

    def log_receiver(self):
        self.receiver_synchronize()
        for ptype, payload in self.packet_loop():
            warnings.warn(f'Unexpected packet: type={ptype} payload={payload}')

    def prop_faker(self):
        class Props:
            def __init__(self):
                self.t0 = time.time()

            def __getitem__(self, prop_id):
                # temperature properties
                arg = time.time() - self.t0 + prop_id * math.pi/8
                val = 80 + 60 * math.sin(arg * math.pi)
                val = int(val*1000)  # convert to millidegree convention
                return struct.pack("ii", val, 0)

        self.receiver_synchronize()
        for ptype, payload in self.packet_loop(
                property_proxy=Props()
        ):
            warnings.warn(f'Unexpected packet: type={ptype} payload={payload}')
    
    def load_test(self):
        self.receiver_synchronize()




def load_image(args):
    intf = SerialInterface(args.port, args.baudrate)
    # intf.synchronize()
    intf.ser.flushInput()

    print('Device identification:')
    structured, rest = intf.cmd_identify()
    print(structured.pretty)

    if rest:
        print('Leftover data:', rest)

    print('Flashing...')
    intf.flash_user_fw(args.image.read())


def mock_bootloader(args):
    intf = SerialInterface(args.port, args.baudrate)
    intf.mock_bootloader()


def log_receiver(args):
    intf = SerialInterface(args.port, args.baudrate)
    intf.log_receiver()


def prop_faker(args):
    intf = SerialInterface(args.port, args.baudrate)
    intf.prop_faker()

def load_test(args):
    intf = SerialInterface(args.port, args.baudrate)
    intf.ser.flushInput()

    print("request device identification continuously")

    count = 0
    while True:
        structured, rest = intf.cmd_identify()
        print(structured.pretty)

        if rest:
            print('Leftover data:', rest)
            return

        count += 1

        print ( "packages sent: " + str(count) )


if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('port', help='Serial port to device')
    parser.add_argument('--baudrate', type=int,
                        default=115200, help='Baudrate to use')

    subparsers = parser.add_subparsers(
        title='subcommands', required=True, dest='subcommands')
    load_image_parser = subparsers.add_parser(
        'load-image', help='Load image using device bootloader')
    load_image_parser.add_argument('image', type=argparse.FileType('rb'))
    load_image_parser.set_defaults(func=load_image)
    bootloader_parser = subparsers.add_parser(
        'mock-bootloader', help='Emulate device bootloader for testing')
    bootloader_parser.set_defaults(func=mock_bootloader)
    bootloader_parser = subparsers.add_parser(
        'log-receiver', help='Print log messages')
    bootloader_parser.set_defaults(func=log_receiver)
    bootloader_parser = subparsers.add_parser(
        'prop-faker', help='Emulate properties')
    bootloader_parser.set_defaults(func=prop_faker)

    bootloader_parser = subparsers.add_parser(
        'load-test', help='Create load')
    bootloader_parser.set_defaults(func=load_test)

    args = parser.parse_args()
    args.func(args)
