CC = gcc
CFLAGS ?= -Wall -Wextra -std=c11
LDFLAGS ?=
LDLIBS ?= -lm

TARGET = snake.exe
OBJECTS = main.o DrawObject.o Wall.o Food.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ main.c

DrawObject.o: DrawObject.c
	$(CC) $(CFLAGS) -c -o $@ DrawObject.c

wall.o: wall.c
	$(CC) $(CFLAGS) -c -o $@ wall.c

Food.o: Food.c
	$(CC) $(CFLAGS) -c -o $@ Food.c

clean:
	$(RM) $(OBJECTS) $(TARGET)
