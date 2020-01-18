CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -Ofast
SRC		= $(wildcard src/*.c)
OBJ		= $(SRC:%.c=%.o)
BIN		= rle

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -I./inc $(OBJ) -o $@

clean:
	rm -f $(OBJ) $(BIN)

re: clean $(BIN)

src/%.o: src/%.c inc/rle.h
	$(CC) $(CFLAGS) -I./inc -c $< -o $@
