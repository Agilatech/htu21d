/**
 * \file Htu21dDrv.h
 *
 *  Created by Scott Erholm on 6/14/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __HTU21D__Htu21dDrv__
#define __HTU21D__Htu21dDrv__

#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include "I2CDevice.h"
#include "DataManip.h"

#ifdef DEBUG
#  define DPRINT(x) do { std::cerr << x; std::cerr << std::endl; } while (0)
#else
#  define DPRINT(x) do {} while (0)
#endif

#define HTDU21D_READTEMP    0xE3
#define HTDU21D_READHUM     0xE5
#define HTDU21D_WRITEREG    0xE6
#define HTDU21D_READREG     0xE7
#define HTDU21D_RESET       0xFE

#define HTDU21D_ADDR 0x40  //Unshifted 7-bit I2C address for the sensor

#define HTDU21D_TRIGGER_TEMP_MEASURE_HOLD  0xE3
#define HTDU21D_TRIGGER_HUMD_MEASURE_HOLD  0xE5
#define HTDU21D_TRIGGER_TEMP_MEASURE_NOHOLD  0xF3
#define HTDU21D_TRIGGER_HUMD_MEASURE_NOHOLD  0xF5

static const std::string name = "HTU21D";
static const std::string type = "sensor";

static const std::string version = "0.9.0";

static const int numValues = 2;

static const std::string valueNames[numValues] = {"humidity", "temperature"};
static const std::string valueTypes[numValues] = {"float", "float"};


class Htu21dDrv : public i2cbus::I2CDevice {
    
public:
    Htu21dDrv(std::string devfile, uint32_t addr);
    
    static std::string getVersion();
    static std::string getDeviceName();
    static std::string getDeviceType();
    static int getNumValues();
    static std::string getTypeAtIndex(int index);
    static std::string getNameAtIndex(int index);
    
    bool isActive();
    std::string getValueByName(std::string name);
    std::string getValueAtIndex(int index);
    
protected:
    
    bool initialize();
    std::string readValue0();
    std::string readValue1();
    
private:
    
    void reset();
    unsigned char* read3Bytes(uint32_t fromAddress);
    
    bool active = false;
    
};

#endif /* defined(__HTU21D__Htu21dDrv__) */
