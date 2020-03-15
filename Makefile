CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror -Ofast
SRC			:= $(wildcard src/*.c)
OBJ			:= $(SRC:%.c=%.o)
BIN			:= rle
BIN_PATH	:= /usr/local/bin

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -I./inc $(OBJ) -o $@

clean:
	rm -f $(OBJ) $(BIN)

install: $(BIN)
	install rle $(BIN_PATH)

re: clean $(BIN)

uninstall:
	rm -f $(BIN_PATH)/$(BIN)

src/%.o: src/%.c inc/rle.h
	$(CC) $(CFLAGS) -I./inc -c $< -o $@
