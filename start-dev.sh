#!/bin/sh

if ! [ -f "./manifest.scm" ]; then
    echo "ERR - must call from project root"
    exit 1
fi

#PATCH="$1"
#if [ -z $PATCH ]; then
#    echo "ERR - please specify a patch"
#    exit 1
#fi

guix shell -m manifest.scm \
    --preserve='^DISPLAY$' \
    --preserve='^XAUTHORITY$' \
    --preserve='^XDG_RUNTIME_DIR$' \
    coreutils \
    gcc-toolchain \
    vim \
    -- bash || echo "ERR - unknown error"
