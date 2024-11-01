#!/bin/sh

if ! [ -f "./manifest.scm" ]; then
    echo "ERR - expected to find 'manifest.scm' in pwd"
    exit 1
fi

export LIBPD_PROJECT_ROOT=$(dirname $(dirname $(dirname $(pwd))))
export GUILE_LIBPD_DIR=${LIBPD_PROJECT_ROOT}/guile/ffi

# This is needed for picking up our custom pd.
PKG_CONFIG_PATH=${LIBPD_PROJECT_ROOT}

guix shell -m manifest.scm \
    --preserve='^DISPLAY$' \
    --preserve='^XAUTHORITY$' \
    --preserve='^XDG_RUNTIME_DIR$' \
    --preserve='^LIBPD_PROJECT_ROOT$' \
    --share=$HOME \
    --share=$LIBPD_PROJECT_ROOT \
    -- guile -l ${GUILE_LIBPD_DIR}/libpd.scm -s src/main.scm || echo "ERR - unknown error"
