#include "trampoline.h"

int do_add(int a, int b)
{
  return a+b;
}

int local_state(void)
{
  static int counter = 0;

  return counter++;
}

static char searchstr[] = "the quick brown fox jumps over the lazy dog";

int find_in_string(int c)
{
  char *p;
  for (p = searchstr; *p != '\0'; p++)
    if (*p == c)
      return p - searchstr;
  return -1;
}

typedef long (*opfn)(long);

static long
even_op(long n)
{
  return n >> 1;
}

static long
odd_op(long n)
{
  return 3*n+1;
}

static opfn ops[] = {even_op, odd_op};

long
collatz(long n)
{
  long i = (long)0;
  opfn op;

  while (n != 1) {
    op = ops[n & 0b1];
    n = (*op)(n);
    i++;
  }
  
  return i;
}

unsigned long
fib(unsigned long n)
{
  return n < 2 ? n
               : fib(n-1) + fib(n-2);
}
