CC = gcc

SOURCEDIR = "."
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')

%.o: %.c $(SOURCES)
	$(CC) -c -o $@ $<

build: main.o node.o solve.o
	$(CC) -o logic main.o node.o solve.o

debug: main.o node.o
	$(CC) -g3 -o logic main.o node.o

test1: debug
	# TEST 1
	#gdb -ex=r --args 
	./logic "p ^ q"

test2: debug
	# TEST 2
	./logic "( p ^ q | r ) | p"
	#gdb -ex=r --args ./logic "( p ^ q | r ) | p"

test3: debug
	# TEST 3
	./logic "( ( p ^ q ) | p ) ~ ( ( p | q ) ^ p ) ~ p"

test4: debug
	# TEST 4
	./logic "( ( - p ^ q ) | p ) ~ ( ( p | q ) ^ - p ) ~ p"

test: test1 test2 test3 test4


default: build run
