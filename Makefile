CC=gcc
CFLAGS= -std=c99 -pedantic -Werror -Wall -Wextra
BIN=rt
SRC=src/raytracer.c src/vector3.c src/vector3bis.c src/parser.c src/visual.c src/destroyer.c\
    src/lighting.c src/intersec.c
OBJS=$(SRC:.c=.o)
LDFLAGS=-lm

all: release

release: $(OBJS)
	$(CC) $^ -o $(BIN) $(LDFLAGS)

debug: CFLAGS += -g3 -ggdb

debug: release

.PHONY: clean
clean:
	rm -rf $(OBJS)
	rm -rf $(BIN)
