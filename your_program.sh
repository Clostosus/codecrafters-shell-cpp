#!/bin/bash
#
# Use this script to run your program LOCALLY.
#
# Note: Changing this script WILL NOT affect how CodeCrafters runs your program.
#
# Learn more: https://codecrafters.io/program-interface

set -e # Exit early if any commands fail

# Copied from .codecrafters/compile.sh
#
# - Edit this to change how your program compiles locally
# - Edit .codecrafters/compile.sh to change how your program compiles remotely
#(
#  cd "$(dirname "$0")" # Ensure compile steps are run within the repository directory
#  cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
#  cmake --build ./build
#)

# Copied from .codecrafters/run.sh
#
# - Edit this to change how your program runs locally
# - Edit .codecrafters/run.sh to change how your program runs remotely

stream_characters() {
  while IFS= read -r -s -n1 character; do
    if [[ "$character" = "" ]]; then
      printf $'\n'
    else
      printf "$character"
    fi
  done
}

# Stellt sicher, dass stty sane nach Beenden der Shell ausgeführt wird
trap 'stty sane' EXIT
./cmake-build-debug/shell "$@" < <(stream_characters 2> /dev/null)

