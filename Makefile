cflags := -Wall

run: ntmain
	./ntmain

ntmain: o/nt.o o/main.o o/bigint.o
	gcc o/nt.o o/main.o o/bigint.o -o ntmain

o/nt.o: nt.c nt.h
	gcc $(cflags) -c nt.c -o o/nt.o

o/main.o: main.c nt.h bigint.h
	gcc $(cflags) -c main.c -o o/main.o

o/bigint.o: bigint.c bigint.h
	gcc $(cflags) -c bigint.c -o o/bigint.o
