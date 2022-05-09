#include "trampoline.h"

int doit(void)
{
	 __asm__(
		"li x15, 0x0\n"
		"add x15, x15, x31\n"
		"li x7, 25192\n"
		"add x15, x15, x7\n"
		"jal x0, 6\n"
		"addi x8, x8, 1592\n"
		
		"addi x9, x9, -90\n"
		"jal x0, 6\n"
		"addi x8, x8, -456 \n"
		
		);
	 return 0;
}

