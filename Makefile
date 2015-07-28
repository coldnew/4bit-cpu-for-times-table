CC = gcc
CFLAGS = -pedantic -ansi -Wall -Wstrict-prototypes -std=c99

all:
	mkdir -p bin
	$(CC) $(CFLAGS) main.c -o bin/4bit-cpu-for-times-table

clean:
	rm -rf bin/
