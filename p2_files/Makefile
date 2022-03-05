vulnerable: vulnerable.o ctor.o
	cc vulnerable.o ctor.o -o vulnerable

vulnerable.o: vulnerable.c
	local/bin/tcc -g -c vulnerable.c -o vulnerable.o

ctor.o: ctor.c
	cc -g -c -ffixed-x30 -ffixed-x31 ctor.c -o ctor.o

clean:
	-rm -f vulnerable vulnerable.o ctor.o vulnerable.core
