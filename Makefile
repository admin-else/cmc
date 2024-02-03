CC = clang
CFLAGS = -O2 -ggdb -Wno-macro-redefined
LDFLAGS = -lz -lcrypto -lssl -lcurl

SRC  = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ  = $(SRC:.c=.o)
BIN = bin
FUZZ_DIR = fuzz_input  # Create a directory for fuzz test cases

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

fuzz: $(OBJ)
	mkdir -p ./$(FUZZ_DIR)
	# Copy your valid test cases to the fuzz directory if needed
	# cp valid_test_case.bin ./$(FUZZ_DIR)/
	clang -o ./$(BIN)/packeter-fuzz $^ $(CFLAGS) -fsanitize=fuzzer,address
	./$(BIN)/packeter-fuzz ./$(FUZZ_DIR)
