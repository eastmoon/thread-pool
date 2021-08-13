#!/bin/bash
set -e

# Execute script
cd ${SHELL_ROOT}
if [ -d ${SHELL_ROOT}/${DIR_SOURCE} ]; then
    # Initial directroy
    [ ! -d ${SHELL_ROOT}/${DIR_BUILD} ] && mkdir -p ${SHELL_ROOT}/${DIR_BUILD}
    [ ! -d ${SHELL_ROOT}/${DIR_PUBLISH} ] && mkdir -p ${SHELL_ROOT}/${DIR_PUBLISH}

    # Call make command
    cmake -S ${SHELL_ROOT}/ -B ${SHELL_ROOT}/${DIR_BUILD}/ \
        -D APPLICATION_NAME=${APPLICATION_NAME} \
        -D APPLICATION_SOURCE_DIR=${SHELL_ROOT}/${DIR_SOURCE} \
        -D APPLICATION_PUBLISH_DIR=${SHELL_ROOT}/${DIR_PUBLISH}
    make -C ${SHELL_ROOT}/${DIR_BUILD}/
fi
