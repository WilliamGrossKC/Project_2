#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sfi.h"

/* not called ... ? */
void print_flag(void)
{
  printf("FLAG: decafbad abad1dea\n");
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Usage: target <module.sfi>\n");
    exit(EXIT_FAILURE);
  }

  module_load(argv[1]);
  module_check();
  module_activate();

  sfi_call(sfi_entrypoint("doit"));

  return 0;
}
