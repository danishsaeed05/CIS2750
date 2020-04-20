CFLAGS = -Wall -g -std=c11
BIN = bin/
INC = include/
SRC = src/
#XML_PATH = /usr/include/libxml2
XML_PATH = /Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/usr/include/libxml2
LDFLAGS = -Lbin/

# Standard common makefile
parser:test

test:$(BIN)test.o $(BIN)libsvgparse.so
	gcc $(CFLAGS) $(LDFLAGS) -o output $(BIN)test.o $(BIN)SVGParser.o $(BIN)LinkedListAPI.o -lxml2 -lm

$(BIN)libsvgparse.so: $(BIN)SVGParser.o $(BIN)LinkedListAPI.o $(BIN)test.o
	gcc -shared -o $(BIN)libsvgparse.so $(BIN)SVGParser.o $(BIN)LinkedListAPI.o $(BIN)test.o -lxml2 -lm

$(BIN)test.o:$(SRC)test.c $(SRC)SVGParser.c $(INC)LinkedListAPI.h $(INC)SVGParser_A2.h 
	gcc -c -fpic $(CFLAGS) -I$(XML_PATH) -I$(INC) -c $(SRC)test.c -o $(BIN)test.o

$(BIN)SVGParser.o: $(SRC)SVGParser.c $(INC)LinkedListAPI.h $(INC)SVGParser_A2.h 
	gcc -c -fpic $(CFLAGS) -I$(XML_PATH) -I$(INC) $(SRC)SVGParser.c -o $(BIN)SVGParser.o

$(BIN)LinkedListAPI.o: $(SRC)LinkedListAPI.c $(INC)LinkedListAPI.h
	gcc -c -fpic $(CFLAGS) -I$(INC) $(SRC)LinkedListAPI.c -o $(BIN)LinkedListAPI.o


	
clean:
	rm $(BIN)*.o $(BIN)*.so output


# make xmlExample 
# ./xmlExample Emoji_poo.svg

#make StructListDemo
#./StructListDemo Emoji_poo.svg

#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
