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
};
