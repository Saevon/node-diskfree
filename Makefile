# The name of the library
MODULE_NAME = diskspace

# Build directories
BUILD_DIR = ./build
TEST_DIR = ./test
RELEASE_DIR = ${BUILD_DIR}/Release

# Library Files
BASE_HEADERS = os_detect.h disk.h
BASE_LIB = disk.cpp
NODE_LIB = disk-gyp.cpp ${BASE_LIB}
TEST_LIB = ${TEST_DIR}/test.cpp

# Test & Generated Files
TEST_C_FILE = test.exe
TEST_JS_FILE = test.js
NODE_MODULE = ${MODULE_NAME}.node

# Executables
GCC = gcc
SHELL := /usr/bin/env bash
PATH := ./node_modules/.bin:$(PATH)




#################################
# Helper targets

.PHONY: default
default: test-all

.PHONY: help
help:
	@echo "clean"
	@echo ""
	@echo "test-all  Runs all the tests"
	@echo "test-c    Runs the pure c++ tests"
	@echo "test-js   Runs the pure full node tests"

.PHONY:
clean:
	rm -rf ${BUILD_DIR}


####################
# Tests

.PHONY: test-all
test-all: test-c test-js

.PHONY: test-c
test-c: ${BUILD_DIR}/${TEST_C_FILE}
	${BUILD_DIR}/${TEST_C_FILE}

.PHONY: test-js
test-js: ${RELEASE_DIR}/${NODE_MODULE}
	node ${TEST_DIR}/${TEST_JS_FILE}


##################################
# Compilation

build:
	mkdir ${BUILD_DIR}

# C++ Compilation
${BUILD_DIR}/${TEST_C_FILE}: ${TEST_LIB} ${BASE_LIB} ${BASE_HEADERS} ${BUILD_DIR}
	${GCC} ${BASE_LIB} ${TEST_LIB} -I. -o $@

# Node Compilation
${RELEASE_DIR}/${NODE_MODULE}: ${NODE_LIB} ${BASE_HEADERS} ${BUILD_DIR}
	echo ${PATH}
	node-gyp configure build

