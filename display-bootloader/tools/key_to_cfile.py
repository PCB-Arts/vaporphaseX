#!/usr/bin/env python3

import textwrap

# Requires pycryptodome
from Crypto.PublicKey import ECC

def convert_key(key_bytes):
    key = ECC.import_key(key_bytes).public_key()
    assert key.curve == 'NIST P-256'

    px_bytes = list(reversed(key._point.x.to_bytes(32)))
    py_bytes = list(reversed(key._point.y.to_bytes(32)))

    def chunked(l, chunk_len=8):
        for i in range(0, len(l), chunk_len):
            yield l[i:i+chunk_len]

    def join_hex(data):
        return ', '.join(f'0x{b:02x}' for b in data)

    def make_array(l):
        return '\n'.join(f'BYTES_TO_T_UINT_8( {join_hex(chunk)} ),' for chunk in chunked(l, 8))

    return f'''
.x = {{
{textwrap.indent(make_array(px_bytes), "    ")}
}},

.y = {{
{textwrap.indent(make_array(py_bytes), "    ")}
}}
'''

if __name__ == '__main__':
    import sys

    key_pem_data = sys.stdin.read()
    print(convert_key(key_pem_data))

