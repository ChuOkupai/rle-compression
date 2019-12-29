CC= gcc
CFLAGS= -Wall -Wextra -Werror -Ofast

all: rle
	./$<

clean:
	rm -f rle.o rle

re: clean all

rle.o: src/rle.c
	$(CC) $(CFLAGS) -c $<

rle: src/main.c rle.o inc/rle.h
	$(CC) $(CFLAGS) -I./inc $< rle.o -o $@
