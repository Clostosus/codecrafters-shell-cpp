#!/bin/bash
#
# This script is used to run your program on CodeCrafters
# 
# This runs after .codecrafters/compile.sh
#
# Learn more: https://codecrafters.io/program-interface

# Exit early if any commands fail
set -e

stream_characters() {
  while IFS= read -r -s -n1 character; do
    if [[ "$character" = "" ]]; then
      printf $'\n'
    else
      printf "$character"
    fi
  done
}

exec ./build/shell "$@" < <(stream_characters 2>/dev/null)
