CC = clang
CFLAGS = -O2 -ggdb -Wall -Wno-macro-redefined -fsanitize=address
LDFLAGS = -lz -lcrypto -lssl -lcurl -fsanitize=address

SRC  = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ  = $(patsubst src/%.c,bin/%.o,$(SRC))
BIN = bin
FUZZ_DIR = fuzz_input  # Create a directory for fuzz test cases

.PHONY: all clean

all: dirs codegen packeter

dirs:
	mkdir -p ./$(BIN)

run: all
	$(BIN)/packeter

packeter: $(OBJ)
	$(CC) -o $(BIN)/packeter $^ $(LDFLAGS)

bin/%.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

codegen:
	python3 codegen.py

clean:
	rm -rf $(BIN) $(OBJ)

fuzz: $(OBJ)
	mkdir -p ./$(FUZZ_DIR)
	# Copy your valid test cases to the fuzz directory if needed
	# cp valid_test_case.bin ./$(FUZZ_DIR)/
	$(CC) -o ./$(BIN)/packeter-fuzz $^ $(CFLAGS) -fsanitize=fuzzer,address $(LDFLAGS)
	./$(BIN)/packeter-fuzz ./$(FUZZ_DIR)
