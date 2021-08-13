#!/bin/bash
set -e

# Execute script
cd ${SHELL_ROOT}
source ./shell/build.sh
if [ -d ${SHELL_ROOT}/${DIR_PUBLISH}/ ]; then
    echo ""
    ${SHELL_ROOT}/${DIR_PUBLISH}/bin/${APPLICATION_NAME}
    echo ""
fi
