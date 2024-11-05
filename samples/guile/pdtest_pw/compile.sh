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

guild compile \
    -L $(pwd) \
    src/main.scm \
    -o libpd_test || echo "ERR - could not launch guile"

