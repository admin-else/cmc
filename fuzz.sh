#!/usr/bin/bash
set -xe
make clean
mkdir -p ./bin
gcc -o src/Main.o -c src/Main.c -Wall -O2 -g
gcc -o src/heap-utils.o -c src/heap-utils.c -Wall -O2 -g
gcc -o src/Packets.o -c src/Packets.c -Wall -O2 -g
gcc -o src/MConn.o -c src/MConn.c -Wall -O2 -g
gcc -o src/Nbt.o -c src/Nbt.c -Wall -O2 -g
gcc -o src/FileUtils.o -c src/FileUtils.c -Wall -O2 -g
gcc -o src/MCbuffer.o -c src/MCbuffer.c -Wall -O2 -g
clang -o ./bin/packeter-fuzz src/*.o -Wall -O2 -g -fsanitize=fuzzer
./bin/packeter-fuzz ./fuzz