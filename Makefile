CC = gcc
CFLAGS = -Wall -O2 -g -Ilib/libnbt/libnbt.h
LDFLAGS = -lm -lz

SRC  = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ  = $(SRC:.c=.o)
BIN = bin

.PHONY: all clean

all: dirs packeter

dirs:
	mkdir -p ./$(BIN)

run: all
	$(BIN)/packeter

packeter: $(OBJ)
	$(CC) -o $(BIN)/packeter $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
