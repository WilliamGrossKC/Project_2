/*
 * SFI setup.  Call these in order.
 */
void module_load(char *filename);
int module_check(void);
void module_activate(void);

/*
 * SFI interface.  First argument to sfi_call should be
 * a value returned by sfi_entrypoint.  Only integer arguments
 * are supported, and only six or fewer.
 */
uint64_t sfi_entrypoint(char *fname);
uint64_t sfi_call(uint64_t entrypoint, ...);
