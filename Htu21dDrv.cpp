/**
 * \file Htu21dDrv.cpp
 *
 *  Created by Scott Erholm on 6/14/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 */

#include "Htu21dDrv.h"

Htu21dDrv::Htu21dDrv(std::string devfile, uint32_t addr):i2cbus::I2CDevice(devfile,addr) {
    
    if (initialize()) {
        this->active = true;
    }
    else {
        std::cerr << name << " did not initialize. " << name << " is inactive" << std::endl;
    }
    
}

std::string Htu21dDrv::getVersion() {
    return name + " " + version;
}

std::string Htu21dDrv::getDeviceName() {
    return name;
}

int Htu21dDrv::getNumValues() {
    return numValues;
}

std::string Htu21dDrv::getTypeAtIndex(int index) {
    if ((index < 0) || (index > (numValues - 1))) {
        return "none";
    }
    
    return valueTypes[index];
}

std::string Htu21dDrv::getNameAtIndex(int index) {
    if ((index < 0) || (index > (numValues - 1))) {
        return "none";
    }
    
    return valueNames[index];
}

bool Htu21dDrv::isActive() {
    return this->active;
}

std::string Htu21dDrv::getValueAtIndex(int index) {
    
    if (!this->active) {
        return "none";
    }

    if (index == 0) {
        return this->readValue0();
    }
    else if (index == 1) {
        return this->readValue1();
    }
    else {
        return "none";
    }
    
}

bool Htu21dDrv::initialize() {
    
    this->reset();
    
    /* Make sure we're actually connected */
    uint8_t x = readRegister(HTDU21D_READREG);
    if (!(x & 0x2)) {
        return false;
    }
    
    return true;
}

std::string Htu21dDrv::readValue0() {
    
    if (!this->active) {
        return "none";
    }
    
    unsigned char *hum = read3Bytes(HTDU21D_READHUM);
    
    unsigned int rawHumidity = ((unsigned int) hum[0] << 8) | (unsigned int) hum[1];
    
    // crc is hum[2]
    
    float retHum = rawHumidity;
    retHum *= 125;
    retHum /= 65536;
    retHum -= 6;
    
    // convert the float to a string with 1 decimal place
    return DataManip::dataToString(retHum, 1);
}

std::string Htu21dDrv::readValue1() {
    
    if (!this->active) {
        return "none";
    }
    
    unsigned char *tmp = read3Bytes(HTDU21D_READTEMP);
    
    unsigned int temp = ((unsigned int) tmp[0] << 8) | (unsigned int) tmp[1];
    
    // crc is tmp[2]
    
    float retTemp = temp;
    retTemp *= 175.72;
    retTemp /= 65536;
    retTemp -= 46.85;
    
    // convert the float to a string with 1 decimal place
    return DataManip::dataToString(retTemp, 1);
}

void Htu21dDrv::reset(void) {
    
    write(HTDU21D_RESET);
    
    usleep(15000);
}

unsigned char* Htu21dDrv::read3Bytes(uint32_t fromAddress){
    this->write(fromAddress);
    unsigned char* data = new unsigned char[3];
    
    // Wait 50ms for the measurement to be taken
    usleep(50000);
    
    if(::read(this->file, data, 3)!=(int)3){
        DPRINT("Htu21dDrv: Failed to read in the full buffer.\n");
        return NULL;
    }
    return data;
}






