/*
 * include this file in the .c for every module.
 */

__asm__(
  "  .globl __trampoline\n"
  "  .p2align 2\n"
  "  .type __trampoline, STT_FUNC\n"
  "__trampoline:\n"
  "  .cfi_startproc\n"
  "  jr x31\n"
  ".L__trampoline_end1:\n"
  "  .size __trampoline, .L__trampoline_end1-__trampoline\n"
  "  .cfi_endproc"
);
