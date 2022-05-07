CCFLAGS := -O2 -Wall
ASFLAGS := -march=rv64g -ffixed-x26 -ffixed-x27 -ffixed-x28 \
                        -ffixed-x29 -ffixed-x30 -ffixed-x31
LDFLAGS := -static -nostdlib -nostartfiles -Wl,-T,ldscript

.PHONY: default clean
default:

.SUFFIXES: .c .s0 .s .sfi

.bin.enc:
	./encoder ${.IMPSRC} > ${.TARGET}

.s.sfi:
	cc $(LDFLAGS) ${.IMPSRC} -o ${.TARGET}

.s0.s:
	./rewrite.pl < ${.IMPSRC} > ${.TARGET}

.c.s0:
	cc -S $(CCFLAGS) $(ASFLAGS) ${.IMPSRC} -o ${.TARGET}

clean:
	-rm -f *.s0 *.s *.sfi
