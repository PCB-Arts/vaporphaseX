#!/bin/bash

export SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

BASE_DIR=${2:-"${SCRIPT_DIR}/../.."}

# fall back to default dev key
export KEY=${1:-"${SCRIPT_DIR}/../crypto/key.pem"}

function sign_image {
    set -e
    unsigned_image=$1
    signed_image="${unsigned_image%_unsigned.bin}.BIN"

    python3 "${SCRIPT_DIR}/sign_image.py" "--private_key_pem" "${KEY}" "${unsigned_image}" "${signed_image}"

    echo "signed ${signed_image} with key ${KEY}"
}

export -f sign_image

find "${BASE_DIR}" -name "*_unsigned.bin" | xargs -I {} bash -c 'sign_image "$@"' _ {}
