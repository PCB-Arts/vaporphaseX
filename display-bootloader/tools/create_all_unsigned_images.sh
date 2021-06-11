#!/bin/bash

export SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

BASE_DIR=${2:-"${SCRIPT_DIR}/../.."}

function sign_image {
    set -e
    unsigned_image=$1
    signed_image="${unsigned_image%_unsigned.bin}.BIN"

    python3 "${SCRIPT_DIR}/sign_image.py" "--just_hash" "${unsigned_image}" "${signed_image}"

    echo "added image header to ${signed_image}"
}

export -f sign_image

find "${BASE_DIR}" -name "*_unsigned.bin" | xargs -I {} bash -c 'sign_image "$@"' _ {}
