
This implements a shared interface to the unix-like and windows disk space readers.

Unix runs on the [`statvfs`](https://linux.die.net/man/2/statvfs) systemcall
Windows runs on the [`GetDiskFreeSpaceEx`](https://msdn.microsoft.com/en-us/library/windows/desktop/aa364937/) systemcall

This utility will return the correct error number from the respective systemcall.

## Installation

Install with npm

```bash
npm install disk-space
```

## Usage

```js
var diskfree = require('diskfree');
diskfree.check('C:', function onDiskInfo(error, info) {
    if (error) {
        // You can see if its a known error
        if (diskfree.isErrBadPath(err)) {
            throw new Error('Path is Wrong');
        } else if (diskfree.isErrDenied(error)) {
            throw new Error('Permission Denied');
        } else if (diskfree.isErrIO(error)) {
            throw new Error('IO Error');
        }

        throw new Error('Unknown error: ' + error);
    }

    info.free == "Free Space";
    info.available == "User Available Space";
    info.total == "Total Space"
});
```

### diskfree.check(diskPath, callback):

Grabs disk space information

**Arguments**

| Name        | Type   | Description           |
|-------------|--------|-----------------------|
| diskPath   | `String`   | Directory to check  |
| callback   | `function` | `callback(error, diskInfo)` |

**`diskInfo`**

All space values are in bytes. If the value exceeds the maximum size of a double `0xFFFF FFFF FFFF F800` then this returns `Infinity`

| Name        | Type     | Description           |
|-------------|----------|-----------------------|
| free        | `Number` | Free space  |
| available   | `Number` | User Available free space  |
| total       | `Number` | Total space  |

**`error`**

Error will be `undefined` if the operation succeeds, otherwise check the system error list to find the problem.

Note that your specific system will might have its own error codes

Windows [`System Error Codes`](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381)
Unix-Like [`errno.h`](http://www.virtsync.com/c-error-codes-include-errno)


### diskfree.isErrBadPath(error):

Returns `true` if the errorcode is one of the windows/unix errorcodes that indicate the path was wrong.

Note: this might only identify some pathing errors.

### diskfree.isErrDenied(error):

Returns `true` if the errorcode is one of the windows/unix errorcodes that indicate permission was denied

Note: this might only identify some pathing errors.

### diskfree.isErrDenied(error):

Returns `true` if the errorcode is one of the windows/unix errorcodes that indicate there was an io error

Note: this might only identify some pathing errors.



## Testing

To test just the base C code, run the `test.exe`

``` bash
make test-c
```

To run the node test
``` bash
make test-js
```



## Compiling


To compile just the C code run the following

``` bash
make
```

Reminder: You need make on windows, in MingW you need to adds it binary path `/c/MingW/bin/mingw32-make.exe`


## Installation

```bash
npm install git@github.com:Saevon/node-diskfree.js.git --save
```

## License

[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/Saevon/bytes.js/blob/master/LICENSE)

