#include <node.h>
#include <v8.h>
#include <cmath>

#include "disk.h"

#define DBL_MAX 0xFFFFFFFFFFFFF800


namespace win32Diskspace {

  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::Value;
  using v8::Exception;

  using v8::Undefined;
  using v8::Null;
  using v8::String;
  using v8::Number;
  using v8::Function;



  const char * ToCString(const String::Utf8Value &value) {
    return *value;
  }

  void assignOverflowableUInt64(Isolate *isolate, Object *obj, const char *key, uint64_t val) {
    // Check for Overflows
    if (val > DBL_MAX) {
        obj->Set(String::NewFromUtf8(isolate, key), Number::New(isolate, INFINITY));
    } else {
        obj->Set(
	    String::NewFromUtf8(isolate, key),
	    Number::New(isolate, static_cast<double>(val))
	);
    }
  }

  void Method(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    // Check the args
    if (args.Length() != 2) {
        isolate->ThrowException(Exception::TypeError(
	    String::NewFromUtf8(isolate, "drivePath and callback are required")
	));
    } else if (!args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(
	    String::NewFromUtf8(isolate, "drivePath should be a string")
	));
    } else if (!args[1]->IsFunction()) {
        isolate->ThrowException(Exception::TypeError(
	    String::NewFromUtf8(isolate, "callback should be a function")
	));
    }

    // Get the Args
    String::Utf8Value str(args[0]);
    Local<Function> callback = Local<Function>::Cast(args[1]);

    // Finish the
    args.GetReturnValue().Set(Undefined(isolate));

    // Call the Inner Function
    const char *drivePath = ToCString(str);
    uint64_t available = 0, free = 0, total = 0;
    uint64_t errorValue = GetDiskSpace(drivePath, &available, &total, &free);

    // Format the Error
    if (errorValue != 0) {
    	Local<Number> error = Number::New(isolate, static_cast<double>(errorValue));
	const unsigned argc = 1;
	Local<Value> argv[argc] = {error};
        callback->Call(Null(isolate), argc, argv);
	return;
    }

    // Format the disk info
    Local<Object> diskInfo = Object::New(isolate);

    // Check for Overflows
    assignOverflowableUInt64(isolate, *diskInfo, "available", available);
    assignOverflowableUInt64(isolate, *diskInfo, "free", free);
    assignOverflowableUInt64(isolate, *diskInfo, "total", total);

    // Format the return value
    const unsigned argc = 2;
    Local<Value> argv[argc] = {
      Undefined(isolate),
      diskInfo,
    };
    callback->Call(Null(isolate), argc, argv);
  }

  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "check", Method);
  }

  NODE_MODULE(win32Diskspace, init)

}
