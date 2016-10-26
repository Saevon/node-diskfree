

## Usage

```js
var win32-diskspace = require('win32-diskspace');
win32-diskspace.check('C:', function onDiskInfo(error, info) {
    if (error) throw error;

    info.free == "Free Space";
    info.available == "User Available Space";
    info.total == "Total Space"
});
```


### win32-diskspace.check(diskPath, callback):

Grabs disk information from windows using `GetDiskFreeSpaceEx`.

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

Error will be `undefined` if the operation succeeds, otherwise check the windows [`System Error Codes`](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381) to find the problem (The link might change, but looking for `System Error Codes` in the windows docs should yield the codes).



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
npm install git@github.com:Saevon/node-win32-diskspace.js.git --save
```

## License

[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/Saevon/bytes.js/blob/master/LICENSE)

