#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

CRYPTO_DIR="${DIR}/../crypto"

#enshure dirs exist
mkdir -p "${CRYPTO_DIR}/include"

#generate new key, overrides old dev key
"${DIR}/gen_key.sh" "${CRYPTO_DIR}/key.pem"

# generate c sources from key
"${DIR}/key_to_cfile.py" \
    < "${CRYPTO_DIR}/key.pem" \
    > "${CRYPTO_DIR}/include/fw_update_sig_pk.c_inc"

