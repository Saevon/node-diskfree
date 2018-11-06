#include <node.h>
#include <uv.h>
#include <v8.h>

#include <cmath>
#include <stdlib.h>
#include <string.h>
#include "disk.h"

#define DBL_MAX 0xFFFFFFFFFFFFF800


namespace diskfree {

    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::HandleScope;

    using v8::Local;
    using v8::Persistent;

    using v8::Undefined;
    using v8::Null;

    using v8::Object;
    using v8::Value;
    using v8::Exception;
    using v8::String;
    using v8::Number;
    using v8::Function;

    struct reqData_t {
        // v8 Worker requirements
        Persistent<Function> callback;

        // Input
        const char *diskPath;

        // Output
        uint64_t error;
        uint64_t available;
        uint64_t free;
        uint64_t total;
    };



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

    void DiskSpaceWorker(uv_work_t *request) {
        reqData_t *requestData = reinterpret_cast<reqData_t *>(request->data);

        // Call the Inner Function
        requestData->error = GetDiskSpace(
            requestData->diskPath,
            &(requestData->available),
            &(requestData->total),
            &(requestData->free)
         );
    }

    void AfterDiskSpaceWorker(uv_work_t *request, int status) {
        reqData_t *requestData = reinterpret_cast<reqData_t *>(request->data);
        Isolate *isolate = Isolate::GetCurrent();
        HandleScope handleScope(isolate);

        const unsigned argc = 2;
        Local<Value> argv[argc];

        // Format the Error
        if (requestData->error != 0) {
            argv[0] = Number::New(isolate, static_cast<double>(requestData->error));
            argv[1] = Undefined(isolate);
        } else {
            // Format the disk info
            Local<Object> diskInfo = Object::New(isolate);

            // Check for Overflows
            assignOverflowableUInt64(isolate, *diskInfo, "available", requestData->available);
            assignOverflowableUInt64(isolate, *diskInfo, "free", requestData->free);
            assignOverflowableUInt64(isolate, *diskInfo, "total", requestData->total);
            assignOverflowableUInt64(isolate, *diskInfo, "used", (requestData->total - requestData->free));
            assignOverflowableUInt64(isolate, *diskInfo, "locked", (requestData->free - requestData->available));

            // Setup return values
            argv[0] = Undefined(isolate);
            argv[1] = diskInfo;
        }

        Local<Function>::New(isolate, requestData->callback)->Call(isolate->GetCurrentContext()->Global(), argc, argv);

        // Cleanup
        requestData->callback.Reset();
        free((void *) requestData->diskPath);
        delete requestData;
        delete request;
    }

    bool checkArgs(const FunctionCallbackInfo<Value> &args) {
        Isolate *isolate = args.GetIsolate();

        // Check the args
        if (args.Length() != 2) {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "diskPath and callback are required")
            ));
            return false;
        } else if (!args[0]->IsString()) {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "diskPath should be a string")
            ));
            return false;
        } else if (!args[1]->IsFunction()) {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "diskPath should be a function")
            ));
            return false;
        }

        return true;
    }

    void DiskSpace(const FunctionCallbackInfo<Value> &args) {
        Isolate *isolate = args.GetIsolate();

        // Check the input args
        bool shouldRun = checkArgs(args);
        if (!shouldRun) {
            return;
        }

        // Get the Args
        String::Utf8Value diskPath(args[0]);
        Local<Function> callback = Local<Function>::Cast(args[1]);

        // Copy the string into a C String (+1 or the null char)
        char *diskPathC = (char *) calloc(diskPath.length() + 1, sizeof(char));
        strcpy(diskPathC, *diskPath);

        // Store the args in a request
        reqData_t *requestData = new reqData_t();
        requestData->callback.Reset(isolate, callback);
        requestData->diskPath = diskPathC;

        // Run the worker
        uv_work_t *request = new uv_work_t();
        request->data = requestData;

        uv_queue_work(uv_default_loop(), request, &DiskSpaceWorker, &AfterDiskSpaceWorker);

        // Return to the parent early
        args.GetReturnValue().Set(Undefined(isolate));
    }



    bool checkErrorcodeArgs(const FunctionCallbackInfo<Value> &args) {
        Isolate *isolate = args.GetIsolate();

        // Check the args
        if (args.Length() != 1) {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "errno is required")
            ));
            return false;
        } else if (!args[0]->IsNumber()) {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "errno should be a Number")
            ));
            return false;
        }

        return true;
    }

    void checkErr(const FunctionCallbackInfo<Value> &args, diskErrFunc_t errChecker) {
        Isolate *isolate = args.GetIsolate();

        // Check the input args
        bool shouldRun = checkErrorcodeArgs(args);
        if (!shouldRun) {
            return;
        }

        // Get the args
        double error = args[0]->ToNumber(isolate)->NumberValue();

        // Convert to proper type
        args.GetReturnValue().Set(
            static_cast<double>(errChecker(static_cast<uint64_t>(error)))
        );
    }

    void nodeIsErrDenied(const FunctionCallbackInfo<Value> &args) {
        checkErr(args, isErrDenied);
    }

    void nodeIsErrBadPath(const FunctionCallbackInfo<Value> &args) {
        checkErr(args, isErrBadPath);
    }

    void nodeIsErrIO(const FunctionCallbackInfo<Value> &args) {
        checkErr(args, isErrIO);
    }

    void init(Local<Object> exports) {
        NODE_SET_METHOD(exports, "check", DiskSpace);
        NODE_SET_METHOD(exports, "isErrDenied", nodeIsErrDenied);
        NODE_SET_METHOD(exports, "isErrBadPath", nodeIsErrBadPath);
        NODE_SET_METHOD(exports, "isErrIO", nodeIsErrIO);
    }

    NODE_MODULE(diskfree, init)
}
