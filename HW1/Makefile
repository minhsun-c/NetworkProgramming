CC = gcc
CFLAGS = -Wall -Wextra -g
INC = -Iinclude
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

all: obj bin tmp number myshell

obj: 
	mkdir obj

bin:
	mkdir bin

tmp:
	mkdir tmp

myshell: $(OBJS)
	$(CC) $(CFLAGS) $(INC) -o $@ $^

# $@: myshell
# $^: $(OBJS)

obj/%.o: src/%.c 
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

clean: 
	rm obj/*.o bin/*

number:
	g++ -o ./bin/number builtin_src/number.cpp