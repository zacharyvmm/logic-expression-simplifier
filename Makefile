CC = gcc

SOURCEDIR = "."
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')

EXECUTABLE = "./logic"
OBJECTS := main.o node.o solve.o display.o

%.o: %.c $(SOURCES)
	$(CC) -c -o $@ $<

build: $(OBJECTS)
	$(CC) -o $(EXECUTABLE) $(OBJECTS)

debug: $(OBJECTS)
	$(CC) -g3 -o $(EXECUTABLE) $(OBJECTS)

test: debug
	# run without arguments and it automaticly runs test
	$(EXECUTABLE)

gdb: debug
	# run without arguments and it automaticly runs test
	gdb -ex=r $(EXECUTABLE)


default: build run


clean:
	rm $(EXECUTABLE) $(OBJECTS)
