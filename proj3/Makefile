CFLAGS := -O2 -Wall -g

.PHONY: default clean
default: target demo-runner

target: target.o sfi.o riscv-disas.o
	cc target.o sfi.o riscv-disas.o -lelf -o target

target.o: target.c sfi.h
	cc -c $(CFLAGS) target.c -o target.o

demo-runner: demo-runner.o sfi.o riscv-disas.o
	cc demo-runner.o sfi.o riscv-disas.o -lelf -o demo-runner

demo-runner.o: demo-runner.c sfi.h
	cc -c $(CFLAGS) demo-runner.c -o demo-runner.o

sfi.o: sfi.c sfi.h riscv-disas.h
	cc -c $(CFLAGS) sfi.c -o sfi.o

riscv-disas.o: riscv-disas.c riscv-disas.h
	cc -c $(CFLAGS) riscv-disas.c -o riscv-disas.o

clean:
	-rm -f demo-runner.o target.o sfi.o riscv-disas.o
	-rm -f demo-runner target demo-runner.core target.core
