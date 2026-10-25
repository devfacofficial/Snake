CC = gcc
CFLAGS ?= -Wall -Wextra -std=c11
LDFLAGS ?=
LDLIBS ?= -lm

TARGET = snake.exe
OBJECTS = main.o DrawObject.o GameObj.o Wall.o Food.o Snake.o
HEADERS = DrawObject.h GameObj.h Wall.h Food.h Snake.h Array.h

.PHONY: all clean

all: $(TARGET)

$(OBJECTS): $(HEADERS)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ main.c

GameObj.o: GameObj.c
	$(CC) $(CFLAGS) -c -o $@ GameObj.c

DrawObject.o: DrawObject.c
	$(CC) $(CFLAGS) -c -o $@ DrawObject.c

Wall.o: Wall.c
	$(CC) $(CFLAGS) -c -o $@ Wall.c

Food.o: Food.c
	$(CC) $(CFLAGS) -c -o $@ Food.c

Snake.o: Snake.c
	$(CC) $(CFLAGS) -c -o $@ Snake.c

clean:
	$(RM) $(OBJECTS) $(TARGET)
