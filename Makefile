CC=gcc
CFLAGS=-O3 -Wall -Wextra -Werror

clean:
	rm -f *.rle *.out

run: mkrle.out
	./$<

mkrle.out: rlecompression.c
	$(CC) $(CFLAGS) $< -o $@
