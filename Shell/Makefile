CC = gcc
CFLAGS = -Wall -Wextra -g
INC = -Iinclude
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

all: myshell

myshell: $(OBJS)
	$(CC) $(CFLAGS) -o bin/$@ $^

# $@: myshell
# $^: $(OBJS)

obj/%.o: src/%.c 
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

clean: 
	rm obj/*.o bin/myshell