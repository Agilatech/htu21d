/**
 * \file DataManip.h
 *
 *  Created by Scott Erholm on 6/14/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 */

#ifndef __DataManip__
#define __DataManip__

#include <string>
#include <math.h>

class DataManip {

public:
    
    static std::string dataToString(int data);
    static std::string dataToString(float data, int numDecimals);
    static std::string dataToString(bool data);
    
protected:
    
private:
    
};

#endif /* __DataManip__ */
