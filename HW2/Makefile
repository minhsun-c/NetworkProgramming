CC = gcc
CFLAGS = -Wall -Wextra -g
INC = -Iinclude

SRCS1 = $(wildcard src/shell/*.c)
SRCS2 = $(wildcard src/network/*.c)
OBJS = $(patsubst src/shell/%.c, obj/%.o, $(SRCS1))
OBJS += $(patsubst src/network/%.c, obj/%.o, $(SRCS2))

BSRCS1 = $(wildcard bsrc/*.c)
BSRCS2 = $(wildcard bsrc/*.cpp)
BINS1 = $(patsubst bsrc/%.c, bin/%, $(BSRCS1))
BINS2 = $(patsubst bsrc/%.cpp, bin/%, $(BSRCS2))

all: tmp bin obj $(BINS1) $(BINS2) myserver

myserver: $(OBJS) 
	$(CC) $(CFLAGS) $(INC) -o $@ $(OBJS)

obj/%.o: src/shell/%.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

obj/%.o: src/network/%.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

bin/%: bsrc/%.c
	$(CC) $(INC) -o $@ $^

bin/%: bsrc/%.cpp
	g++ $(INC) -o $@ $^

bin:
	mkdir bin

obj:
	mkdir obj

tmp:
	mkdir tmp

clean:
	rm obj/* bin/* myserver tmp/*