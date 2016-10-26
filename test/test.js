var assert = require('assert');

var disk = require('../build/release/win32Diskspace');


disk.check("C:", (err, data) => {
    assert.equal(err, undefined, 'Error recieved reading drive "C:": ' + err);

    assert.ok(data.available);
    assert.ok(data.free);
    assert.ok(data.total);

    assert.ok(data.total >= data.free);
    assert.ok(data.total >= data.available);
    assert.ok(data.free >= data.available);
});

disk.check("C", (err, data) => {
    assert.ok(err == 3);
});
