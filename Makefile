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

ladder: makeladder runladder

makeladder: BIN INCLUDE SRC OBJ TEST LIB
	$(CC) $(CFLAGS) -Iinclude src/*.c test/testladder.c -o bin/ladder


runladder: makeladder
	cd bin; ./ladder

makedisalvo: BIN INCLUDE SRC OBJ LIB
	$(CC) $(CFLAGS) -Iinclude src/*.c test/DisalvoGrayCode.c -o bin/disalvo

rundisalvo: makedisalvo
	cd bin; ./disalvo

##runs the disalvo ladder generating algorithm
disalvo: rundisalvo

clean: 
	if [ -d "bin" ]; then rm -rf ./bin/; fi; \
