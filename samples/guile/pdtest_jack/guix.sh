#!/bin/sh

if ! [ -f "./manifest.scm" ]; then
    echo "ERR - expected to find 'manifest.scm' in pwd"
    exit 1
fi

LIBPD_PROJECT_ROOT=$(dirname $(dirname $(dirname $(pwd))))
GUILE_LIBPD_DIR=${LIBPD_PROJECT_ROOT}/guile/ffi
GUILE_LOAD_PATH=$GUILE_LIBPD_DIR:$GUILE_LOAD_PATH

# This is needed for picking up our custom pd.
PKG_CONFIG_PATH=${LIBPD_PROJECT_ROOT}

guix shell -m manifest.scm \
    --preserve='^DISPLAY$' \
    --preserve='^XAUTHORITY$' \
    --preserve='^XDG_RUNTIME_DIR$' \
    --preserve='^PKG_CONFIG_PATH$' \
    --preserve='^GUILE_LIBPD_DIR$' \
    --preserve='^GUILE_LOAD_PATH$' \
    --preserve='^PKG_CONFIG_PATH' \
    --share=$HOME \
    --share=$LIBPD_PROJECT_ROOT \
    --search-paths \
    emacs emacs-geiser \
#    guile
    -- bash || echo "ERR - unknown error"
