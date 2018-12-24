CC=gcc
CFLAGS=-O3 -Wall -Wextra -Werror

clean:
	rm -f *.rle *.out

rle.out: rlecompression.c
	$(CC) $(CFLAGS) $< -o $@

run: rle.out
	./$<