CC=gcc
CFLAGS=-O3 -Wall -Wextra

all: clean main

clean:
	sudo rm -f mkrle *.rle *.out

main:
	$(CC) $(CFLAGS) rlecompression.c -o mkrle
