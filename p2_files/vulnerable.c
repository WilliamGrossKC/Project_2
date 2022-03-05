/*
 * This vulnerable program would be easy to exploit by
 * stack smashing, but not if it is protected with a
 * shadow stack!
 * You can test that your protection extends to other
 * dangerous C functions by modifying this program.
 * Keep in mind that some functions in libc may 
 * overwrite t6.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void notcalled(void)
{
  printf("This function is never called.\n");
  printf("Why is this message even here?\n");
  exit(EXIT_SUCCESS);
}

void foo(char *arg)
{
  char buf[64];

  strcpy(buf, arg);
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Usage: vulnerable [egg]\n");
    exit(EXIT_FAILURE);
  }

  foo(argv[1]);
  return 0;
}
