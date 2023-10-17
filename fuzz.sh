#!/usr/bin/bash
set -xe
make clean
mkdir -p ./bin
clang -o ./bin/packeter-fuzz src/*.c -Wall -O2 -g -fsanitize=fuzzer
./bin/packeter-fuzz ./fuzz