#include "trampoline.h"

int doit(void)
{
  __asm__(
    "li x5, 0x6268\n"
    "nop\n"
    "add x5, x5, x31\n"
    "nop\n"
    "fld f5, 0(x5)\n"
    "nop\n"
    "fmv.x.d x6, f5\n"
    "nop\n"
    "addiw x6, x6, -90\n"
    "nop\n"
    "fmv.d.x f5, x6\n"
    "nop\n"
    "fsd f5, 0(x5)\n"
    "nop\n"
  );
  return 2;
}
