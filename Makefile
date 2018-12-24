CC=gcc
CFLAGS=-O3 -Wall -Wextra -Werror

clean:
	rm -f *.rle *.out

rle.out: rlecompression.c
	$(CC) $(CFLAGS) $< -o $@

run: rle.out
	./$<

S=sample
test: rle.out $S1 $S3
	./$< -c $S1 $S1.rle $S3 $S3.rle
	./$< -e $S1.rle $S1.bak.rle $S3.rle $S3.bak.rle