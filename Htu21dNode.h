/**
 * \file Htu21dNode.h
 *
 *  Created by Scott Erholm on 6/17/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 */

#ifndef __HTU21D__Htu21dNode__
#define __HTU21D__Htu21dNode__

#include <node.h>
#include <node_object_wrap.h>
#include <uv.h>
#include <iostream>
#include <cmath>
#include <string>
#include <thread>
#include "Htu21dDrv.h"

namespace htu21d {
    
class Htu21dNode : public node::ObjectWrap {
 
public:
    static void Init(v8::Local<v8::Object> exports);
    
    static void getDeviceName(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getDeviceType(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getDeviceVersion(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getDeviceNumValues (const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getTypeAtIndex (const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getNameAtIndex (const v8::FunctionCallbackInfo<v8::Value>& args);
    static void isDeviceActive (const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getValueAtIndexSync (const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getValueAtIndex (const v8::FunctionCallbackInfo<v8::Value>& args);
    
private:
    
    explicit Htu21dNode(std::string devfile = "/dev/i2c-1", uint32_t addr = 0x40) {}
    
    ~Htu21dNode() {}
    
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    
    static void WorkAsync(uv_work_t *req);
    static void WorkAsyncComplete(uv_work_t *req,int status);
    
    static v8::Persistent<v8::Function> constructor;
    
    static Htu21dDrv *driver;
    
    uint32_t bus_;
    
    struct Work {
        uv_work_t  request;
        v8::Persistent<v8::Function> callback;
        
        int valueIndex;
        std::string value;
    };

    
};

    
} // namespace


#endif /* defined(__HTU21D__Htu21dNode__) */
