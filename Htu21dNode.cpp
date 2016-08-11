/**
 * \file Htu21dNode.cpp
 *
 *  Created by Scott Erholm on 6/17/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 */

#include "Htu21dNode.h"

namespace htu21d {
    
    using v8::FunctionCallbackInfo;
    using v8::FunctionTemplate;
    using v8::Function;
    using v8::Persistent;
    using v8::Isolate;
    using v8::Context;
    using v8::Local;
    using v8::Handle;
    using v8::Object;
    using v8::String;
    using v8::Value;
    using v8::Number;
    using v8::Boolean;
    
    Persistent<Function> Htu21dNode::constructor;
    Htu21dDrv* Htu21dNode::driver = 0;
    
    void Htu21dNode::Init(Local<Object> exports) {
        Isolate* isolate = exports->GetIsolate();
        
        // prep the constructor template
        Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
        
        // associates the New function with the class named Htu21d
        tpl->SetClassName(String::NewFromUtf8(isolate, "Htu21d"));
        
        // InstanceTemplate is the ObjectTemplate assocated with the function New
        tpl->InstanceTemplate()->SetInternalFieldCount(1);
        
        NODE_SET_PROTOTYPE_METHOD(tpl, "deviceName", getDeviceName);
        NODE_SET_PROTOTYPE_METHOD(tpl, "deviceVersion", getDeviceVersion);
        NODE_SET_PROTOTYPE_METHOD(tpl, "deviceNumValues", getDeviceNumValues);
        NODE_SET_PROTOTYPE_METHOD(tpl, "typeAtIndex", getTypeAtIndex);
        NODE_SET_PROTOTYPE_METHOD(tpl, "nameAtIndex", getNameAtIndex);
        NODE_SET_PROTOTYPE_METHOD(tpl, "deviceActive", isDeviceActive);
        NODE_SET_PROTOTYPE_METHOD(tpl, "valueAtIndexSync", getValueAtIndexSync);
        NODE_SET_PROTOTYPE_METHOD(tpl, "valueAtIndex", getValueAtIndex);

        // store a reference to this constructor
        constructor.Reset(isolate, tpl->GetFunction());
        
        exports->Set(String::NewFromUtf8(isolate, "Htu21d"), tpl->GetFunction());
    }
    
    void Htu21dNode::getDeviceName(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        
        std::string name = Htu21dDrv::getDeviceName();
        Local<String> deviceName = String::NewFromUtf8(isolate, name.c_str());
        
        args.GetReturnValue().Set(deviceName);
    }
    
    void Htu21dNode::getDeviceVersion(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        
        std::string ver = Htu21dDrv::getVersion();
        Local<String> deviceVer = String::NewFromUtf8(isolate, ver.c_str());
        
        args.GetReturnValue().Set(deviceVer);
    }

    void Htu21dNode::getDeviceNumValues (const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        
        int value = Htu21dDrv::getNumValues();
        Local<Number> deviceNumVals = Number::New(isolate, value);
        
        args.GetReturnValue().Set(deviceNumVals);
    }
    
    void Htu21dNode::getTypeAtIndex (const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        
        std::string type = driver->getTypeAtIndex(args[0]->NumberValue());
        Local<String> valType = String::NewFromUtf8(isolate, type.c_str());
        
        args.GetReturnValue().Set(valType);
    }
    
    void Htu21dNode::getNameAtIndex (const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        
        std::string name = driver->getNameAtIndex(args[0]->NumberValue());
        Local<String> valName = String::NewFromUtf8(isolate, name.c_str());
        
        args.GetReturnValue().Set(valName);
    }
    
    void Htu21dNode::isDeviceActive (const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        
        bool active = driver->isActive();
        Local<Boolean> deviceActive = Boolean::New(isolate, active);
        
        args.GetReturnValue().Set(deviceActive);
    }
    
    void Htu21dNode::getValueAtIndexSync (const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        
        std::string value = driver->getValueAtIndex(args[0]->NumberValue());
        Local<String> retValue = String::NewFromUtf8(isolate, value.c_str());
        
        args.GetReturnValue().Set(retValue);
    }
    
    void Htu21dNode::getValueAtIndex (const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        
        Work * work = new Work();
        work->request.data = work;
        
        // get the desired value index from the first param in the JS call
        work->valueIndex = args[0]->NumberValue();
        
        // store the callback from JS in the work package so we can invoke it later
        Local<Function> callback = Local<Function>::Cast(args[1]);
        work->callback.Reset(isolate, callback);
        
        // kick of the worker thread
        uv_queue_work(uv_default_loop(),&work->request,WorkAsync,WorkAsyncComplete);
        
        args.GetReturnValue().Set(Undefined(isolate));
    }
    
    void Htu21dNode::New(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        
        String::Utf8Value param0(args[0]->ToString());
        std::string devfile = std::string(*param0);
        
        uint32_t addr = args[1]->IsUndefined() ? 0x40 : args[1]->NumberValue();
        
        // if invoked as costructor: 'new Htu21d(...)'
        if (args.IsConstructCall()) {
            
            Htu21dNode* obj = new Htu21dNode();
            
            obj->Wrap(args.This());
            
            args.GetReturnValue().Set(args.This());
        }
        // else invoked as plain function 'Htu21d(...)' -- turn into construct call
        else {
            const int argc = 2;
            Local<Value> argv[argc] = { args[0], args[1] };
            
            Local<Function> cons = Local<Function>::New(isolate, constructor);
            Local<Context> context = isolate->GetCurrentContext();
            Local<Object> instance =
            cons->NewInstance(context, argc, argv).ToLocalChecked();
            args.GetReturnValue().Set(instance);

        }
        
        if (!driver) {
            driver = new Htu21dDrv(devfile, addr);
        }
        
    }
    
    // called by libuv worker in separate thread
    void Htu21dNode::WorkAsync(uv_work_t *req) {
        Work *work = static_cast<Work *>(req->data);
        
        work->value = driver->getValueAtIndex(work->valueIndex);
    }
    
    // called by libuv in event loop when async function completes
    void Htu21dNode::WorkAsyncComplete(uv_work_t *req, int status) {
        Isolate * isolate = Isolate::GetCurrent();
        
        v8::HandleScope handleScope(isolate);
        
        Work *work = static_cast<Work *>(req->data);
        
        // the work has been done, and now we store the value as a v8 string
        
        Local<String> retValue = String::NewFromUtf8(isolate, work->value.c_str());
        
        // set up return arguments: 0 = error, 1 = returned value
        Handle<Value> argv[] = { Null(isolate) , retValue };
        
        // execute the callback
        Local<Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext()->Global(), 2, argv);
        
        // Free up the persistent function callback
        work->callback.Reset();
        delete work;
        
    }

    void init(Local<Object> exports) {
        
        Htu21dNode::Init(exports);
        
    }
    
    NODE_MODULE(htu21d, init)
    
}  // namespace htu21d