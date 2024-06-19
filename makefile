CC = gcc
CFLAGS = -Wall -Wextra

all: main_program

main_program: main.o huffman.o
	$(CC) $(CFLAGS) -o main_program main.o huffman.o

main.o: main.c huffman.h
	$(CC) $(CFLAGS) -c -o main.o main.c

huffman.o: huffman.c huffman.h
	$(CC) $(CFLAGS) -c -o huffman.o huffman.c

clean:
	rm -f main_program *.o
