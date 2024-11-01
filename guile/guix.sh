#!/bin/sh

if ! [ -f "./manifest.scm" ]; then
    echo "ERR - expected to find 'manifest.scm' in pwd"
    exit 1
fi

# Make sure pkg-config can find the libpd.pc file we've defined
LIBPD_PROJECT_ROOT="$(dirname $(pwd))"
PKG_CONFIG_PATH="${LIBPD_PROJECT_ROOT}"

guix shell -m manifest.scm \
    --share=${LIBPD_PROJECT_ROOT} \
    --preserve='^DISPLAY$' \
    --preserve='^XAUTHORITY$' \
    --preserve='^XDG_RUNTIME_DIR$' \
    --preserve='^PKG_CONFIG_PATH$' \
    coreutils \
    gcc-toolchain \
    vim \
    -- bash || echo "ERR - unknown error"
