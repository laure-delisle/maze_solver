CC = gcc
CFLAGS = -g -Wall 
IFLAGS = -I./include/

all: bin/maze

bin/maze: obj/main.o obj/daedalus.o obj/array.o
	$(CC) $(CFLAGS) $(IFLAGS) obj/main.o obj/daedalus.o obj/array.o -o bin/maze

obj/main.o: src/main.c include/daedalus.h include/struct.h
	$(CC) $(CFLAGS) $(IFLAGS) src/main.c -c -o obj/main.o

obj/daedalus.o: src/daedalus.c include/array.h include/struct.h
	$(CC) $(CFLAGS) $(IFLAGS) src/daedalus.c -c -o obj/daedalus.o

obj/array.o: src/array.c include/array.h include/struct.h
	$(CC) $(CFLAGS) $(IFLAGS) src/array.c -c -o obj/array.o

.PHONY:clean
	
clean:
	rm obj/*.o bin/maze