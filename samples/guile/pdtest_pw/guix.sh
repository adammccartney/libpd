#!/usr/bin/bash

if ! [ -f "./manifest.scm" ]; then
    echo "ERR - expected to find 'manifest.scm' in pwd"
    exit 1
fi

export LIBPD_PROJECT_ROOT=$(dirname $(dirname $(dirname $(pwd))))
export GUILE_LIBPD_DIR=${LIBPD_PROJECT_ROOT}/guile

guix shell -m manifest.scm \
    --preserve='^DISPLAY$' \
    --preserve='^XAUTHORITY$' \
    --preserve='^XDG_RUNTIME_DIR$' \
    --preserve='^LIBPD_PROJECT_ROOT$' \
    --share=$HOME \
    --share=$LIBPD_PROJECT_ROOT \
    -- bash || echo "ERR - unkown error"

