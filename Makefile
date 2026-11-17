CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
SRC = src/main.c src/board.c src/display.c src/input.c
OBJ = $(SRC:.c=.o)
TARGET = dots_and_boxes

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

src/%.o: src/%.c $(wildcard include/*.h)
	$(CC) $(CFLAGS) -c $< -o $@

build/test_board: tests/test_board.c src/board.c include/board.h
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ tests/test_board.c src/board.c

test: build/test_board
	./build/test_board

valgrind: build/test_board
	valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --error-exitcode=1 ./build/test_board

clean:
	rm -f src/*.o $(TARGET) build/test_board

.PHONY: all clean test valgrind
