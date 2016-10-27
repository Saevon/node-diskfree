var assert = require('assert');

var disk;
try {
    disk = require('diskfree');
} catch (err) {
    disk = require('../build/Release/diskfree');
}

var first = false;


var isWin = /^win/.test(process.platform);
var VALID_DRIVE = isWin ? 'C:' : '/';


disk.check(VALID_DRIVE, (err, data) => {
    assert.equal(err, undefined, 'Error recieved reading drive "C:": ' + err);

    assert.ok(data.available);
    assert.ok(data.free);
    assert.ok(data.total);

    assert.ok(data.total >= data.free);
    assert.ok(data.total >= data.available);
    assert.ok(data.free >= data.available);

    assert.ok(first, 'This function should be async');

    console.log('Test1 Done');
});

// Mark that the main javascript thread is free
first = true;

disk.check('C', (err, data) => {
    assert.ok(disk.isErrBadPath(err), 'Should be a Drive not Found Error Got(' + err + ')');

    console.log('Test2 Done');
});

// Try all the error cases
assert.throws(() => {
    disk.check();
}, TypeError);

assert.throws(() => {
    disk.check(VALID_DRIVE);
}, TypeError);

assert.throws(() => {
    disk.check(1, () => {});
}, TypeError);

assert.throws(() => {
    disk.check(VALID_DRIVE, 1);
}, TypeError);

assert.throws(() => {
    disk.check(1, 1);
}, TypeError);

