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
	$(CC) $(CFLAGS) -Iinclude src/*.c test/SvgMain.c -o bin/svg

runladder: makeladder
	./bin/svg > Graphics/graphics.html

clean: 
	if [ -d "bin" ]; then rm -rf ./bin/; fi; \
	rm Graphics/graphics.html
