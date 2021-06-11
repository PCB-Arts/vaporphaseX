#!/bin/sh

set -e

if [ $# -ne 1 ]; then
    echo "Usage: $0 output_keyfile.pem"
fi

openssl ecparam -genkey -name secp256r1 -out "$1"

