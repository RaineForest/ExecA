
asm: main.o dlList.o
	gcc -std=c99 -Wall main.o dlList.o -o asm

main.o: main.c dlList.h
	gcc -std=c99 -Wall -c main.c -o main.o

dlList.o: dlList.h dlList.c
	gcc -std=c99 -Wall -c dlList.c -o dlList.o

clean:
	rm *.o asm
