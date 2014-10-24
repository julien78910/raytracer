CC=gcc
CFLAGS= -std=c99 -pedantic -Werror -Wall -Wextra
BIN=rt
SRC=src/raytracer.c src/vector3.c src/vector3bis.c
OBJS=$(SRC:.c=.o)

all: release 

release: $(OBJS)
	$(CC) $(BIN) -o $@

debug: CFLAGS += -g3 -ggdb

debug: release

.PHONY: clean
clean:
	rm -rf $(OBJS)
	rm -rf $(BIN)
