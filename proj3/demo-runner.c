#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sfi.h"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Usage: demo-runner <module.sfi>\n");
    exit(EXIT_FAILURE);
  }

  module_load(argv[1]);
  module_check();
  module_activate();

  printf("5 + 6 = %lu\n",
    sfi_call(sfi_entrypoint("do_add"), 5, 6));

  printf("state is %lu\n",
    sfi_call(sfi_entrypoint("local_state")));
  printf("state is %lu\n",
    sfi_call(sfi_entrypoint("local_state")));
  printf("state is %lu\n",
    sfi_call(sfi_entrypoint("local_state")));

  for (int c = 'a'; c <= 'z'; c++)
    printf("  %c", c);
  printf("\n");
  for (int c = 'a'; c <= 'z'; c++)
    printf("%3ld",
           sfi_call(sfi_entrypoint("find_in_string"), c));
  printf("\n");

  printf("Collatz steps for 77,031: %lu\n",
         sfi_call(sfi_entrypoint("collatz"), 77031));

  for (long i = 0; i < 20; i++)
    printf("%ld ", sfi_call(sfi_entrypoint("fib"), i));
  printf("\n");

  return 0;
}
