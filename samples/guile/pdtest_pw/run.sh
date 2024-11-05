#!/usr/bin/bash
#
# This file should be run from inside an appropriately configured environment.
# Such an environment is configured in `guix.sh`.
# 1. > ./guix.sh
# 2. [env] ./run.sh

if [ -z "${GUILE_EXTENSIONS_PATH-}" ]; then
    export GUILE_EXTENSIONS_PATH="$GUIX_ENVIRONMENT/lib"
else
    GUILE_EXTENSIONS_PATH="$GUIX_ENVIRONMENT/lib:$GUILE_EXTENSIONS_PATH"
fi

pw-jack guile \
    -l ${GUILE_LIBPD_DIR}/ffi/libpd.scm \
    -l $(pwd)/ffi/jack.scm \
    -s src/main.scm || echo "ERR - could not launch guile"

