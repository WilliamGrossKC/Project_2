/*
 * Example function placed in .ctors array and run early in
 * process initialization, before main is called.  This file,
 * unlike other C files in this directory, is compiled with
 * clang, so you have access to clang's inline assembly syntax.
 * You can use a mix of C and assembly, as you prefer, but
 * beware of accidental overwriting of your t6 value in
 * compiled C code, especially if you call libc functions.
 */

__attribute__((constructor))
static void init_protection(void)
{
  // TODO: replace with initialization of shadow stack segment
  __asm__("addi	sp, sp, -2\n"
          "mv	a0, sp\n"	// buf
          "li	a1, 2\n"// buflen
	  "mv	a2, zero\n"	// flags
          "li	t0, 563\n"	// getrandom
	  "ecall\n"
          "ld	t6, 0(sp)\n"
          "addi	sp, sp, 2\n"
	  "slli t6, t6, 12\n"
	  "mv a0, t6\n"
	  "li a1, 0xA0000\n"
	  "li a2, 0x1|0x2\n"
	  "li a3, 0x10|0x1000\n"
	  "li a4, -1\n"
	  "li a5, 0\n"
	  "li t0, 477\n"
	  "ecall\n"
	  );
}
