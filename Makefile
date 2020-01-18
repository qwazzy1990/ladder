CC = gcc
CFLAGS = -std=c11 -Wall -g -pedantic

all: ladder

BIN:
	if [ ! -d "bin" ]; then mkdir bin; fi;

INCLUDE:
	if [ ! -d "include" ]; then mkdir include; fi;

SRC: 
	if [ ! -d "src" ]; then mkdir src; fi;

OBJ:
	if [ ! -d "obj" ]; then mkdir obj; fi;

TEST:
	if [ ! -d "test" ]; then mkdir test; fi;

LIB:
	if [ ! -d "lib" ]; then mkdir lib; fi;

ladder: makeladder

makeladder: BIN INCLUDE SRC OBJ TEST LIB
	$(CC) $(CFLAGS) -Iinclude src/*.c test/testladder.c -o bin/ladder; cd bin; ./ladder
	

runfive: makeladder
	./bin/ladder > bin/ladderfive.txt

runsix:
	./bin/ladder > bin/laddersix.txt

runseven:
	./bin/ladder > bin/ladderseven.txt
clean: 
	if [ -d "bin" ]; then rm -rf ./bin/; fi; \