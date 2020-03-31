CC=gcc

CFLAGS=-c -Wall

all: main

main: main.o Semaphores.o SharedMemory.o
	$(CC) -o main main.o Semaphores.o SharedMemory.o

main.o: main.c
	$(CC) $(CFLAGS) main.c

Semaphores.o: Semaphores.c
	$(CC) $(CFLAGS) Semaphores.c

Shared Memory.o: SharedMemory.c 
	$(CC) $(CFLAGS) SharedMemory.c

clean:
	rm -rf *o main


