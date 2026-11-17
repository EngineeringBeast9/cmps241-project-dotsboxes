CC = gcc
CFLAGS = -Wall -Wextra -g -Isrc
SRC = src/main.c src/board.c src/game.c src/display.c src/input.c
OBJ = $(SRC:.c=.o)
TARGET = dots_and_boxes

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean
