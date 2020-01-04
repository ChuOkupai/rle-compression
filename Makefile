CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -Ofast
OBJ		= rle.o
BIN		= rle

all: rle
	./$<

clean:
	rm -f $(OBJ) rle

re: clean all

%.o: src/%.c
	$(CC) $(CFLAGS) -c $<

$(BIN): src/main.c $(OBJ) inc/rle.h
	$(CC) $(CFLAGS) -I./inc $< $(OBJ) -o $@
