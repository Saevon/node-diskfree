.PHONY: default
default: test-all



build:
	mkdir build

./build/test.exe: test/test.cpp disk.cpp build
	gcc disk.cpp test/test.cpp -I. -o build/test.exe

./build/release/win32Diskspace.node: test/test.cpp disk.cpp disk-gyp.cpp build
	node_modules/node-gyp/bin/node-gyp.js configure build

.PHONY: test-all
test-all: test-c test-js

.PHONY: test-c
test-c: ./build/test.exe
	./build/test.exe

.PHONY: test-js
test-js: ./build/release/win32Diskspace.node
	node ./test/test.js
