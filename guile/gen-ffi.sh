#!/bin/sh

if ! [ -f "./manifest.scm" ]; then
    echo "ERR - expected to find 'manifest.scm' in pwd"
    exit 1
fi

export LIBPD_PROJECT_ROOT="$(dirname $(pwd))"
export PKG_CONFIG_PATH="${LIBPD_PROJECT_ROOT}"

guix shell -m manifest.scm \
    --expose=${LIBPD_PROJECT_ROOT} \
    --preserve='^DISPLAY$' \
    --preserve='^XAUTHORITY$' \
    --preserve='^XDG_RUNTIME_DIR$' \
    --preserve='^PKG_CONFIG_PATH$' \
    coreutils \
    -- guild compile-ffi ffi/libpd.ffi || echo "ERR - unknown error"
