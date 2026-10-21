CC = gcc
CFLAGS ?= -Wall -Wextra -std=c11
LDFLAGS ?=
LDLIBS ?= -lm

TARGET = snake.exe
OBJECTS = main.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ main.c

clean:
	$(RM) $(OBJECTS) $(TARGET)
