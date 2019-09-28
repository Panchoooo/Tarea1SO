programa: main.o manejo.o
	gcc -Wall -o programa main.o manejo.o

main.o: main.c manejodeArchivos.h
	gcc -Wall -c -o main.o main.c

manejo.o: manejodeArchivos.c
	gcc -Wall -c  -o  manejo.o manejodeArchivos.c

clean:
	rm -f programa *.o
