#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <libelf.h>
#include "riscv-disas.h"

/* from stevens. */
#define	MAXLINE		4096	/* max text line length */

#define ERR_DO_ERRNO 1
#define ERR_DO_ELF 2

static void
err_doit(int errnoflag, const char *fmt, va_list ap)
{
  int errno_save, n;
  char buf[MAXLINE];

  errno_save = errno;		/* value caller might want printed */
  vsnprintf(buf, sizeof(buf), fmt, ap);	/* this is safe */
  n = strlen(buf);
  if (errnoflag == ERR_DO_ERRNO)
    snprintf(buf+n, sizeof(buf)-n, ": %s", strerror(errno_save));
  else if (errnoflag == ERR_DO_ELF)
    snprintf(buf+n, sizeof(buf)-n, ": %s", elf_errmsg(elf_errno()));
  strcat(buf, "\n");

  fflush(stdout);         /* in case stdout and stderr are the same */
  fputs(buf, stderr);
  fflush(stderr);

  return;
}

static void
err_sys(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(ERR_DO_ERRNO, fmt, ap);
  va_end(ap);
  exit(1);
}

static void
err_elf(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(ERR_DO_ELF, fmt, ap);
  va_end(ap);
  exit(1);
}

static void
err_quit(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(0, fmt, ap);
  va_end(ap);
  exit(1);
}

static char *
Strdup(const char *str)
{
  char	*ptr;

  if ( (ptr = strdup(str)) == NULL)
    err_sys("strdup error");
  return(ptr);
}

static ssize_t
readn(int fd, void *ptr, size_t n)
{
  size_t	nleft;
  ssize_t	nread;

  nleft = n;
  while (nleft > 0) {
    if ( (nread = read(fd, ptr, nleft)) < 0) {
      if (nleft == n)
        return -1;	/* error, nothing read */
      else
        break;		/* error, return amount read so far */
    } else if (nread == 0) {
        break;		/* EOF */
    }
    nleft -= nread;
    ptr   += nread;
  }
  return(n - nleft);
}
/* end stevens */


struct elf_fn
{
  char *name;
  size_t offset;
  size_t len;
};

#define AND_MASK   0x0fffff
#define TEXT_START 0x200000	/* also text OR_MASK */
#define TEXT_STOP  0x300000
#define DATA_START 0x300000	/* also data OR_MASK */
#define DATA_STOP  0x400000
#define GUARD_SIZE 0x002000	/* 2^13 */

/* yes i know you could do this with preprocessor hacks */
#define AND_MASK_STR   "0x0fffff"
#define TEXT_START_STR "0x200000"
#define TEXT_STOP_STR  "0x300000"
#define DATA_START_STR "0x300000"
#define DATA_STOP_STR  "0x400000"

/* File offsets: */
static size_t text_start   = 0; /* start of .text */
static size_t text_stop    = 0; /* end of .text */
static size_t rodata_start = 0; /* start of .rodata */
static size_t rodata_stop  = 0; /* end of .rodata */
static size_t data_start   = 0; /* start of .data */
static size_t data_stop    = 0; /* end of .data */
static size_t bss_size     = 0; /* size of .bss beyond .data */

/* entrypoints has n_entrypoints entries, sorted by offset */
static int n_entrypoints = 0;
static struct elf_fn *entrypoints = NULL;


static void
elf_read(int fd)
{
  Elf *elf;
  Elf64_Ehdr *ehdr;
  Elf64_Phdr *phdr;

  Elf_Scn *scn, *str_scn;
  Elf64_Shdr *shdr;

  int str_link;
  Elf_Data *symdata, *strdata;
  int nsyms;

  Elf64_Sym *symarr;
  char *strs;

  int i;


  if (elf_version(EV_CURRENT) == EV_NONE)
    err_elf("Cannot set ELF version");
  if ( (elf = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
    err_elf("Cannot begin ELF reading");

  if ( (ehdr = elf64_getehdr(elf)) == NULL)
    err_elf("Cannot get eheader");
  if ( (phdr = elf64_getphdr(elf)) == NULL)
    err_elf("Cannot get pheader");

  for (i = 0; i < ehdr->e_phnum; i++)
    if (phdr[i].p_type  == PT_LOAD &&
        phdr[i].p_flags == (PF_X|PF_R))
      break;
  if (i == ehdr->e_phnum)
    err_quit("Can't find text segment");

  if (phdr[i].p_vaddr != TEXT_START ||
      phdr[i].p_paddr != TEXT_START ||
      phdr[i].p_align != 0x001000   ||
     (phdr[i].p_offset & 0x000fff) != 0)
    err_quit("Text segment has improper placement");

  if (phdr[i].p_filesz != phdr[i].p_memsz ||
      phdr[i].p_memsz > TEXT_STOP - TEXT_START)
    err_quit("Text segment has improper size");

  text_start = phdr[i].p_offset;
  text_stop = phdr[i].p_offset + phdr[i].p_filesz;

  /*
   * rodata (if present)
   */

  for (i = 0; i < ehdr->e_phnum; i++)
    if (phdr[i].p_type  == PT_LOAD &&
        phdr[i].p_flags == (PF_R))
      break;
  if (i != ehdr->e_phnum) {
    if (phdr[i].p_vaddr != DATA_START ||
        phdr[i].p_paddr != DATA_START ||
        phdr[i].p_align != 0x001000 ||
       (phdr[i].p_offset & 0x000fff) != 0)
      err_quit("Read-only data segment has improper placement");

    if (phdr[i].p_filesz != phdr[i].p_memsz ||
        phdr[i].p_memsz > DATA_STOP - DATA_START)
      err_quit("Read-only data segment has improper size");

    rodata_start = phdr[i].p_offset;
    rodata_stop  = phdr[i].p_offset + phdr[i].p_filesz;
  } else {	/* no rodata */
    rodata_start = rodata_stop = 0;
  }

  /*
   * data and bss
   */

  for (i = 0; i < ehdr->e_phnum; i++)
    if (phdr[i].p_type  == PT_LOAD &&
        phdr[i].p_flags == (PF_W|PF_R))
      break;
  if (i != ehdr->e_phnum) {
    if (phdr[i].p_vaddr != DATA_START + (rodata_stop - rodata_start) ||
        phdr[i].p_paddr != DATA_START + (rodata_stop - rodata_start) ||
        phdr[i].p_align != 0x001000 ||
       (phdr[i].p_offset & 0x000fff) != 
           ((rodata_stop - rodata_start) & 0x000fff))
      err_quit("Data segment has improper placement");

    if (phdr[i].p_filesz > phdr[i].p_memsz ||
        phdr[i].p_memsz + (rodata_stop - rodata_start)
             > DATA_STOP - DATA_START)
      err_quit("Data segment has improper size");

    data_start = phdr[i].p_offset;
    data_stop  = phdr[i].p_offset + phdr[i].p_filesz;
    bss_size   = phdr[i].p_memsz - phdr[i].p_filesz;
  } else {	/* no data */
    data_start = data_stop = 0;
    bss_size = 0;
  }

  for (scn = NULL, shdr = NULL, i = 0; i < ehdr->e_shnum; i++)
    {
      if ( (scn = elf_getscn(elf, i)) == NULL)
        err_elf("Cannot get section %d", i);
      if ( (shdr = elf64_getshdr(scn)) == NULL)
        err_elf("Cannot get section header %d", i);
      
      if (shdr->sh_type == SHT_SYMTAB)
        break;
    }
  if (i == ehdr->e_shnum)
    err_quit("SYMTAB section not found");

  if ( (str_link = shdr->sh_link) == 0)
    err_quit("No string section associated with SYMTAB section");
  if ( (str_scn = elf_getscn(elf, str_link)) == NULL)
    err_elf("Cannot get string section %d", str_link);

  if ( (symdata = elf_getdata(scn, NULL)) == NULL)
    err_elf("No data associated with SYMTAB");
  if ( (strdata = elf_getdata(str_scn, NULL)) == NULL)
    err_elf("No data associated with string table");

  nsyms = symdata->d_size / sizeof(Elf64_Sym);
  symarr = (Elf64_Sym *)symdata->d_buf;
  strs = strdata->d_buf;

  /* for convenience in some analyses, we'll also allocate
   * two guard entries in entrypoints ... */

  if ( (entrypoints = malloc((nsyms+2) * sizeof(struct elf_fn)))
       == NULL)
    err_quit("Memory exhausted in malloc");

  /* skip past first entry for now */
  entrypoints++;

  for (n_entrypoints = i = 0; i < nsyms; i++)
    {
      if (ELF64_ST_TYPE(symarr[i].st_info) != STT_FUNC ||
          ELF64_ST_BIND(symarr[i].st_info) != STB_GLOBAL)
        continue;

      if (symarr[i].st_value < TEXT_START ||
          (symarr[i].st_value & 0x3) != 0 ||
          (symarr[i].st_value + symarr[i].st_size) >= TEXT_STOP) 
	err_quit("Invalid placement of symbol %d", i);

      entrypoints[n_entrypoints].name
        = Strdup(strs + symarr[i].st_name);
      entrypoints[n_entrypoints].offset = symarr[i].st_value;
      entrypoints[n_entrypoints].len = symarr[i].st_size;
      n_entrypoints++;
    }

  /* guard entries: first and last dummies */
  entrypoints[-1].name = "dummy:start";
  entrypoints[-1].offset = text_start;
  entrypoints[-1].len = 0;
  entrypoints[n_entrypoints].name = "dummy:stop";
  entrypoints[n_entrypoints].offset = text_stop;
  entrypoints[n_entrypoints].len = 0;

  elf_end(elf);
}

static void
module_map(int fd)
{
  if (MAP_FAILED ==
        mmap((void *)(TEXT_START - GUARD_SIZE),
             DATA_STOP - TEXT_START + 2*GUARD_SIZE,
             0, MAP_FIXED | MAP_GUARD, -1, 0))
    err_sys("Failed to create guard mapping");

  if (MAP_FAILED ==
        mmap((void *)(TEXT_START),
             text_stop - text_start,
             PROT_READ | PROT_WRITE, 	/* will change below */
             MAP_FIXED | MAP_ANON | MAP_PRIVATE, -1, 0))
    err_sys("Failed to create text mapping");

  if (-1 == lseek(fd, text_start, SEEK_SET))
    err_sys("Failed to seek ELF file to text segment");

  if ( (text_stop - text_start) !=
         readn(fd, (void *)(TEXT_START),
               text_stop - text_start) )
    err_quit("Failed to read text segment from file");
  
  /* always map the whole data region, so we have a stack */
  if (MAP_FAILED ==
        mmap((void *)(DATA_START),
             DATA_STOP - DATA_START,
             PROT_READ | PROT_WRITE, 
             MAP_FIXED | MAP_ANON | MAP_PRIVATE, -1, 0))
    err_sys("Failed to create data mapping");

  if (rodata_start != 0 && rodata_stop != 0) {
    if (-1 == lseek(fd, rodata_start, SEEK_SET))
      err_sys("Failed to seek ELF file to read-only data segment");

    if ( (rodata_stop - rodata_start) !=
           readn(fd, (void *)(DATA_START),
                 rodata_stop - rodata_start) )
      err_quit("Failed to read read-only data segment from file");
  }

  if (data_start != 0 && data_stop != 0) {
    if (-1 == lseek(fd, data_start, SEEK_SET))
      err_sys("Failed to seek ELF file to data segment");

    if ( (data_stop - data_start) !=
           readn(fd, (void *)(DATA_START + rodata_stop - rodata_start),
                 data_stop - data_start) )
      err_quit("Failed to read data segment from file");
  }
}

void
module_load(char *filename)
{
  int fd;

  if ( (fd = open(filename, O_RDONLY, 0)) < 0)
    err_sys("Cannot open %s", filename);

  elf_read(fd);
  module_map(fd);

  if (-1 == close(fd))
    err_sys("Failed to close module file");
}

void
module_activate(void)
{
  if (-1 ==
        mprotect((void *)(TEXT_START),
                 text_stop - text_start,
                 PROT_READ | PROT_EXEC))
    err_sys("Failed to create text mapping");
}

#define NOTANINSN ((rv_inst)(UINT64_MAX))
size_t insnmap_len;
rv_inst *insnmap;

static void
create_instmap(void)
{
  size_t textlen = text_stop - text_start;
  uint8_t *textbase = (uint8_t *)(TEXT_START);
  size_t i;
  size_t insnlen;

  insnmap_len = textlen;
  if (NULL == (insnmap = malloc(textlen * sizeof(rv_inst))))
    err_quit("Could not allocate instruction map");

  for (i = 0; i < textlen; i++)
    insnmap[i] = NOTANINSN;

  for (i = 0; i < textlen; i += insnlen)
    fetch_inst(textbase + i, &insnmap[i], &insnlen);
}

static int
addr_to_insn(size_t addr, rv_inst *insn)
{
  /* out of bounds for spec */
  if (addr < TEXT_START || addr >= TEXT_STOP)
    return -1;

  /* out of bounds for specific module */
  if (addr - TEXT_START >= insnmap_len)
    return -1;

  /* not aligned */
  if (insnmap[addr - TEXT_START] == NOTANINSN)
    return -1;

  if (insn)
    *insn = insnmap[addr - TEXT_START];
  return 0;  
}

static void
check_entrypoints_aligned(void)
{
  for (int i = 0; i < n_entrypoints; i++)
    if (-1 == addr_to_insn(entrypoints[i].offset, NULL))
      err_quit("INVALID: entrypoint not aligned");
}

static int
not_compressed(size_t addr, rv_inst insn)
{
  if (inst_length(insn) != 4)
    return -1;

  return 0;
}

static int
opcode_allowed(size_t addr, rv_inst insn)
{
  rv_decode dec;
  decode_inst(&dec, rv64, addr, insn);

  switch(dec.op) {
    case rv_op_illegal:
    case rv_op_ecall:
    case rv_op_ebreak:
    case rv_op_ret:
      return -1;
  }

  return 0;
}

static int
direct_branch_allowed(size_t addr, rv_inst insn)
{
  rv_decode dec;
  decode_inst(&dec, rv64, addr, insn);

  switch(dec.op) {
    case rv_op_beq:
    case rv_op_beqz:
    case rv_op_bge:
    case rv_op_bgeu:
    case rv_op_bgez:
    case rv_op_bgt:
    case rv_op_bgtu:
    case rv_op_bgtz:
    case rv_op_ble:
    case rv_op_bleu:  /* sacre bleu! */
    case rv_op_blez:
    case rv_op_blt:
    case rv_op_bltz:
    case rv_op_bltu:
    case rv_op_bne:
    case rv_op_bnez:
    case rv_op_jal:
    case rv_op_j:
      if (addr + dec.imm < TEXT_START ||
          addr + dec.imm >= TEXT_STOP)
        return -1;	/* out of range */
  }

  return 0;
}

static void
check_each_insn(int (*checkfn)(size_t, rv_inst), char *msg)
{
  size_t addr;
  rv_inst insn;
  size_t insnlen;

  for (addr = TEXT_START;
       addr < TEXT_START+insnmap_len;
       addr += insnlen) {
    if (-1 == addr_to_insn(addr, &insn))
      err_quit("Could not get insn at %zx", addr);
    if (-1 == (*checkfn)(addr, insn)) {
      rv_decode dec;
      char fbuf[128];

      decode_inst(&dec, rv64, addr, insn);
      format_decoded_inst(fbuf, sizeof(fbuf), &dec);

      err_quit("INVALID: bad insn \"%s\" at 0x%zx: %s",
               fbuf, addr, msg);
    }
    insnlen = inst_length(insn);
    if (insnlen < 2 || insnlen > 8) 
      err_quit("INVALID: bad insn length at %zx", addr);
  }
}

static void
check_trampoline(void)
{
  if (n_entrypoints < 1 ||
      entrypoints[0].offset != TEXT_START ||
      0 != strcmp(entrypoints[0].name, "__trampoline"))
    err_quit("INVALID: trampoline missing");

  rv_inst insn;
  if (-1 == addr_to_insn(TEXT_START, &insn))
    err_quit("Could not get insn at %zx", TEXT_START);

  /* note: checks of rd and imm should be redundant
   * given jr mnemonic. */
  rv_decode dec;
  decode_inst(&dec, rv64, TEXT_START, insn);
  if (dec.op != rv_op_jr ||
      dec.rs1 != rv_ireg_t6 ||		/* aka x31 */
      dec.rd != rv_ireg_zero ||
      dec.imm != 0)
    err_quit("INVALID: malformed trampoline");
}

static int
no_modifying_ro_regs(size_t addr, rv_inst insn)
{
  rv_decode dec;
  decode_inst(&dec, rv64, addr, insn);

  switch (dec.rd) {
    case rv_ireg_t3:	/* aka x28 */
    case rv_ireg_t4:
    case rv_ireg_t5:
    case rv_ireg_t6:	/* aka x31 */
      return -1;
  }

  return 0;
}

static int
mask_after_code_ptr_set(size_t addr, rv_inst insn)
{
  rv_decode dec;
  decode_inst(&dec, rv64, addr, insn);

  if (dec.rd != rv_ireg_s10)	/* aka x26 */
    goto accept;

  /* or mask is okay by itself */
  if (dec.op == rv_op_or     &&
      dec.rd == rv_ireg_s10  &&
      dec.rs1 == rv_ireg_s10 &&
      dec.rs2 == rv_ireg_t3)
    goto accept;

  /* and mask must be followed by or mask */
  if (dec.op == rv_op_and    &&
      dec.rd == rv_ireg_s10  &&
      dec.rs1 == rv_ireg_s10 &&
      dec.rs2 == rv_ireg_t5)
    goto expect_or;

  /* for any other change, must see and mask then or mask */
  goto expect_and_then_or;
  
  /*
   * expect and cp,  cp,  AND_MASK
   *    aka and x26, x26, x30
   */
expect_and_then_or:

  addr += inst_length(insn);
  addr_to_insn(addr, &insn);
  decode_inst(&dec, rv64, addr, insn);

  if (dec.op != rv_op_and    ||
      dec.rd != rv_ireg_s10  ||
      dec.rs1 != rv_ireg_s10 ||
      dec.rs2 != rv_ireg_t5)
    return -1;

  /*
   * expect or cp, cp,   code OR_MASK 
   *    aka or x26, x26, x28
   */
expect_or:

  addr += inst_length(insn);
  addr_to_insn(addr, &insn);
  decode_inst(&dec, rv64, addr, insn);

  if (dec.op != rv_op_or     ||
      dec.rd != rv_ireg_s10  ||
      dec.rs1 != rv_ireg_s10 ||
      dec.rs2 != rv_ireg_t3)
    return -1;

accept:
  return 0;
}

static int
mask_after_data_ptr_set(size_t addr, rv_inst insn)
{
  rv_decode dec;
  decode_inst(&dec, rv64, addr, insn);

  if (dec.rd != rv_ireg_s11)	/* aka x27 */
    goto accept;

  /* or mask is okay by itself */
  if (dec.op == rv_op_or     &&
      dec.rd == rv_ireg_s11  &&
      dec.rs1 == rv_ireg_s11 &&
      dec.rs2 == rv_ireg_t4)
    goto accept;

  /* and mask must be followed by or mask */
  if (dec.op == rv_op_and    &&
      dec.rd == rv_ireg_s11  &&
      dec.rs1 == rv_ireg_s11 &&
      dec.rs2 == rv_ireg_t5)
    goto expect_or;

  /* for any other change, must see and mask then or mask */
  goto expect_and_then_or;

  /*
   * expect and dp,  dp,  AND_MASK
   *    aka and x27, x27, x30
   */
expect_and_then_or:

  addr += inst_length(insn);
  addr_to_insn(addr, &insn);
  decode_inst(&dec, rv64, addr, insn);

  if (dec.op != rv_op_and    ||
      dec.rd != rv_ireg_s11  ||
      dec.rs1 != rv_ireg_s11 ||
      dec.rs2 != rv_ireg_t5)
    return -1;

  /*
   * expect or dp, dp,   data OR_MASK 
   *    aka or x27, x27, x29
   */
expect_or:

  addr += inst_length(insn);
  addr_to_insn(addr, &insn);
  decode_inst(&dec, rv64, addr, insn);

  if (dec.op != rv_op_or     ||
      dec.rd != rv_ireg_s11  ||
      dec.rs1 != rv_ireg_s11 ||
      dec.rs2 != rv_ireg_t4)
    return -1;

accept:
  return 0;
}

static int
indirect_branch_uses_code_ptr(size_t addr, rv_inst insn)
{
  /*
   * the trampoline gets a special exception,
   * and was already checked in check_trampoline.
   */
  if (addr == TEXT_START)
    return 0;

  rv_decode dec;
  decode_inst(&dec, rv64, addr, insn);

  switch(dec.op) {
    case rv_op_jalr:
    case rv_op_jr:
      if (dec.rs1 != rv_ireg_s10)
        return -1;
      /*
       * we could also allow the immediate and rely on
       * the guard pages to keep us safe, but that would
       * be more work.
       */
      if (dec.imm != 0)
        return -1;
  }

  return 0;
}

static int
load_store_uses_data_ptr(size_t addr, rv_inst insn)
{
  rv_decode dec;
  decode_inst(&dec, rv64, addr, insn);

  switch(dec.op) {
    /* base isa */
    case rv_op_lb:
    case rv_op_lbu:
    case rv_op_lh:
    case rv_op_lhu:
    case rv_op_lw:
    case rv_op_lwu:
    case rv_op_ld:
    case rv_op_sb:
    case rv_op_sh:
    case rv_op_sw:
    case rv_op_sd:
    /* atomics */
    case rv_op_lr_w:
    case rv_op_lr_d:
    case rv_op_sc_w:
    case rv_op_sc_d:
    case rv_op_amoswap_w:
    case rv_op_amoadd_w:
    case rv_op_amoxor_w:
    case rv_op_amoor_w:
    case rv_op_amoand_w:
    case rv_op_amomin_w:
    case rv_op_amomax_w:
    case rv_op_amominu_w:
    case rv_op_amomaxu_w:
    case rv_op_amoswap_d:
    case rv_op_amoadd_d:
    case rv_op_amoxor_d:
    case rv_op_amoor_d:
    case rv_op_amoand_d:
    case rv_op_amomin_d:
    case rv_op_amomax_d:
    case rv_op_amominu_d:
    case rv_op_amomaxu_d:
      if (dec.rs1 != rv_ireg_s11)
        return -1;
  }

  return 0;
}

int
module_check(void)
{
  create_instmap();

  check_entrypoints_aligned();
  check_trampoline();
  check_each_insn(&not_compressed,
                  "compressed instructions not allowed");
  check_each_insn(&opcode_allowed,
                  "opcode is not allowed");
  check_each_insn(&no_modifying_ro_regs,
                  "modifies read-only register");
  check_each_insn(&mask_after_code_ptr_set,
                  "code pointer modified but not masked");
  check_each_insn(&mask_after_data_ptr_set,
                  "data pointer modified but not masked");
  check_each_insn(&direct_branch_allowed,
                  "direct branch target is out of bounds");
  check_each_insn(&indirect_branch_uses_code_ptr,
                  "indirect branch uses wrong register");
  check_each_insn(&load_store_uses_data_ptr,
                  "load/store uses wrong register");

  return 0;
}

struct pcb_s {
  uint64_t iregs[16];
} pcb;

uint64_t
sfi_call(uint64_t entrypoint, ...);

__asm__(
  "  .globl sfi_call\n"
  "  .p2align 2\n"
  "  .type sfi_call, STT_FUNC\n"
  "sfi_call:\n"
  "  .cfi_startproc\n"
  /* move call target to t0 */
  "  mv t0, a0\n"
  /* save caller registers */
  "  la x31, pcb\n"
  "  sd ra,    0(x31)\n"
  "  sd sp,    8(x31)\n"
  "  sd gp,   16(x31)\n"
  "  sd tp,   24(x31)\n"
  "  sd s0,   32(x31)\n"
  "  sd s1,   40(x31)\n"
  "  sd s2,   48(x31)\n"
  "  sd s3,   56(x31)\n"
  "  sd s4,   64(x31)\n"
  "  sd s5,   72(x31)\n"
  "  sd s6,   80(x31)\n"
  "  sd s7,   88(x31)\n"
  "  sd s8,   96(x31)\n"
  "  sd s9,  104(x31)\n"
  "  sd s10, 112(x31)\n"
  "  sd s11, 120(x31)\n"
  /* shift arguments */
  "  mv a0, a1\n"
  "  mv a1, a2\n"
  "  mv a2, a3\n"
  "  mv a3, a4\n"
  "  mv a4, a5\n"
  "  mv a5, a6\n"
  "  mv a6, a7\n"
  "  mv a7, zero\n"
  /* zero unused registers */
  "  mv gp, zero\n"
  "  mv tp, zero\n"
  "  mv t1, zero\n"
  "  mv t2, zero\n"
  "  mv s1, zero\n"
  "  mv s2, zero\n"
  "  mv s3, zero\n"
  "  mv s4, zero\n"
  "  mv s5, zero\n"
  "  mv s6, zero\n"
  "  mv s7, zero\n"
  "  mv s8, zero\n"
  "  mv s9, zero\n"
  /* set up sfi registers */
  "  li sp,  " DATA_STOP_STR "\n"
  "  li fp,  " DATA_STOP_STR "\n"
  "  li ra,  " TEXT_START_STR "\n"
  "  li x26, " TEXT_START_STR "\n"	/* code pointer */
  "  li x27, " DATA_START_STR "\n"	/* data pointer */
  "  li x28, " TEXT_START_STR "\n"	/* code OR_MASK */
  "  li x29, " DATA_START_STR "\n"	/* data OR_MASK */
  "  li x30, " AND_MASK_STR "\n"	/* code/data AND_MASK */
  "  la x31, sfi_return\n"		/* trampoline target */
  "  jr t0\n"
  ".Lsfi_call_end1:\n"
  "  .size sfi_call, .Lsfi_call_end1-sfi_call\n"
  "  .cfi_endproc"
);

__asm__(
  "  .globl sfi_return\n"
  "  .p2align 2\n"
  "  .type sfi_return, STT_FUNC\n"
  "sfi_return:\n"
  "  .cfi_startproc\n"
  /* restore caller registers */
  "  la x31, pcb\n"
  "  ld ra,    0(x31)\n"
  "  ld sp,    8(x31)\n"
  "  ld gp,   16(x31)\n"
  "  ld tp,   24(x31)\n"
  "  ld s0,   32(x31)\n"
  "  ld s1,   40(x31)\n"
  "  ld s2,   48(x31)\n"
  "  ld s3,   56(x31)\n"
  "  ld s4,   64(x31)\n"
  "  ld s5,   72(x31)\n"
  "  ld s6,   80(x31)\n"
  "  ld s7,   88(x31)\n"
  "  ld s8,   96(x31)\n"
  "  ld s9,  104(x31)\n"
  "  ld s10, 112(x31)\n"
  "  ld s11, 120(x31)\n"
  /* zero unused registers */
  "  mv a2, zero\n"
  "  mv a3, zero\n"
  "  mv a4, zero\n"
  "  mv a5, zero\n"
  "  mv a6, zero\n"
  "  mv a7, zero\n"
  "  mv t0, zero\n"
  "  mv t1, zero\n"
  "  mv t2, zero\n"
  "  mv t3, zero\n"
  "  mv t4, zero\n"
  "  mv t5, zero\n"
  "  mv t6, zero\n"
  "  ret\n"
  ".Lsfi_return_end1:\n"
  "  .size sfi_return, .Lsfi_return_end1-sfi_return\n"
  "  .cfi_endproc"
);

uint64_t
sfi_entrypoint(char *fname)
{
  int i;

  for (i = 1; i < n_entrypoints; i++)	/* skip trampoline */
    if (0 == strcmp(entrypoints[i].name, fname))
      break;
  if (i == n_entrypoints)
    err_quit("Could not find entrypoint %s", fname);

  return entrypoints[i].offset;
}
