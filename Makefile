# for var names see $info make section 10.3
CFLAGS = -O2 -ggdb -Wall -Wextra -W -fsanitize=address,undefined
LDFLAGS = -fsanitize=address,undefined
LDLIBS = -lz -lcrypto -lssl -lcurl


SRC  = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ  = $(patsubst src/%.c,bin/%.o,$(SRC))
FUZZ_DIR = fuzz_input  # Create a directory for fuzz test cases

.PHONY: all clean dirs

all: dirs packeter

run: all
	bin/packeter

packeter: $(OBJ)
	$(CC) -o bin/packeter $^ $(LDFLAGS) $(LDLIBS)

bin/%.o: src/%.c
	mkdir -p ./bin
	$(CC) -o $@ -c $< $(CFLAGS)

format:
	clang-format -i ./src/*.c ./src/*.h

codegen:
	python3 codegen.py
	clang-format -i ./src/*.c ./src/*.h

clean:
	rm -rf bin/packeter $(OBJ)

fuzz: $(OBJ)
	mkdir -p ./$(FUZZ_DIR)
	# Copy your valid test cases to the fuzz directory if needed
	# cp valid_test_case.bin ./$(FUZZ_DIR)/
	$(CC) -o ./bin/packeter-fuzz $^ $(CFLAGS) -fsanitize=fuzzer,address $(LDFLAGS)
	./bin/packeter-fuzz ./$(FUZZ_DIR)
