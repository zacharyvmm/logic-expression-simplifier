CC = gcc

SOURCEDIR = "."
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')

EXECUTABLE = "./logic"
OBJECTS := main.o node.o solve.o

%.o: %.c $(SOURCES)
	$(CC) -c -o $@ $<

build: $(OBJECTS)
	$(CC) -o $(EXCUTABLE) $(OBJECTS)

debug: $(OBJECTS)
	$(CC) -g3 -o $(EXCUTABLE) $(OBJECTS)

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


clean:
	rm $(EXCUTABLE) $(OBJECTS)
