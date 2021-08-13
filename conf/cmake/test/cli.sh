#!/bin/bash
set -e

# Declare variable
source ./shell/config.sh

# Execute script
source ./shell/build.sh
source ./shell/run.sh ${@}
