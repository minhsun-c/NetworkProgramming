CC = gcc
CFLAGS = -Wall -Wextra -g
INC = -Iinclude
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

all: tmp myshell

myshell: $(OBJS)
	$(CC) $(CFLAGS) $(INC) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

tmp:
	mkdir tmp

clean:
	rm obj/*.o myshell

number:
	g++ -o ./bin/number bsrc/number.cpp