#include "trampoline.h"

int doit(void)
{
  __asm__(
	"li x6, 0x0\n"
	"add x6, x6, x31\n"
	"li x7, 25192\n"
	"add x6, x6, x7\n"
	"fld f0, (x6)\n"
	"fmv.x.d x7, f0\n"
	"addi x7, x7, -90\n"
	"fmv.d.x f0, x7\n"
	"fsd f0,(x6)\n"
  );
  return 0;
}
