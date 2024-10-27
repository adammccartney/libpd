#!/bin/sh

if ! [ -f "./manifest.scm" ]; then
    echo "ERR - expected to find 'manifest.scm' in pwd"
    exit 1
fi

LIBPD_PROJECT_ROOT="$(dirname $(pwd))"
PKG_CONFIG_PATH="${LIBPD_PROJECT_ROOT}":"${PKG_CONFIG_PATH}"

guix shell -m manifest.scm \
    --expose=${LIBPD_PROJECT_ROOT} \
    --preserve='^DISPLAY$' \
    --preserve='^XAUTHORITY$' \
    --preserve='^XDG_RUNTIME_DIR$' \
    --preserve='^PKG_CONFIG_PATH$' \
    coreutils \
    gcc-toolchain \
    -- guild compile-ffi ffi/libpd.ffi || echo "ERR - unknown error"
