/**
 * \file DataManip.cpp
 *
 *  Created by Scott Erholm on 6/14/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 */

#include "DataManip.h"

std::string DataManip::dataToString(int data) {
    return std::to_string(data);
}

std::string DataManip::dataToString(float data, int numDecimals) {
    int whole = floor(data);
    int large1 = round(data * pow(10, numDecimals));
    int large2 = round(whole * pow(10, numDecimals));
    int fraction = large1 - large2;
    
    return std::to_string(whole) + "." + std::to_string(fraction);
}

std::string DataManip::dataToString(bool data) {
    
    if (data) {
        return "true";
    }
    else {
        return "false";
    }
}
