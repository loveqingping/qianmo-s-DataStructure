RM=rm -f
CC=gcc
CFLAGS=-Wall -g3 
BIN=btree_test
SRC=btree.c btree_test.c

OBJ=$(SRC:.c=.o)

.PHONY: all
all:$(BIN) 

$(BIN): $(SRC) $(OBJ) Makefile
	$(CC) $(CFLAGS) $(OBJ) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJ) $(BIN)
