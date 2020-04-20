CFLAGS = -Wall -g -std=c11
BIN = bin/
ROOT = ../
INC = include/
SRC = src/
XML_PATH = /usr/include/libxml2
#XML_PATH = /Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/usr/include/libxml2


# Standard common makefile
parser: $(ROOT)libsvgparse.so

$(ROOT)libsvgparse.so: $(BIN)SVGParser.o $(BIN)LinkedListAPI.o
	gcc -shared -o $(ROOT)libsvgparse.so $(BIN)SVGParser.o $(BIN)LinkedListAPI.o -lxml2 -lm

$(BIN)SVGParser.o: $(SRC)SVGParser.c $(INC)LinkedListAPI.h $(INC)SVGParser.h
	gcc -c -fpic $(CFLAGS) -I$(XML_PATH) -I$(INC) $(SRC)SVGParser.c -o $(BIN)SVGParser.o

$(BIN)LinkedListAPI.o: $(SRC)LinkedListAPI.c $(INC)LinkedListAPI.h
	gcc -c -fpic $(CFLAGS) -I$(INC) $(SRC)LinkedListAPI.c -o $(BIN)LinkedListAPI.o

clean:
	rm $(BIN)*.o $(ROOT)*.so
