/**
 * \file I2CDevice.h
 *
 *  Created by Scott Erholm on 6/14/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 */

#ifndef __I2CDevice__
#define __I2CDevice__

#include <iostream>
#include <sstream>
#include <string>
#include <fcntl.h>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define HEX(x) std::setw(2) << std::setfill('0') << std::hex << (int)(x)

namespace i2cbus {
    
    /**
     * @class I2CDevice
     * @brief Generic I2C Device class that can be used to connect to any type of I2C device and read or write to its registers
     */
    class I2CDevice {
        
    public:
        I2CDevice();
        I2CDevice(std::string devfile, uint32_t addr);
        ~I2CDevice();
        
        void setDevfile(std::string devfile);
        void setAddr(uint32_t addr);
        int open();
        int write(unsigned char value);
        unsigned char readRegister(uint32_t registerAddress);
        unsigned char* readRegisters(uint32_t number, uint32_t fromAddress=0);
        int writeRegister(uint32_t registerAddress, unsigned char value);
        void debugDumpRegisters(uint32_t number = 0xff);
        void close();
        
    protected:
        std::string devfile = "";
        uint32_t addr = 0;
        int file;
    };
    
} /* namespace i2cbus */

#endif /* __I2CDevice__ */
