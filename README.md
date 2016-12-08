##Node addon for I2C hardware HTU21D Humidity and Temperature sensor

#####This addon should work on any Linux platform, and has been thoroughly tested on BeagleBone Black

###Install

```
npm install @agilatech/htu21d
```
```
git clone https://github.com/Agilatech/htu21d.git
node-gyp configure build
```

###Usage
#####Load the module and create an instance

```
const addon = require('@agilatech/htu21d');

// create an instance on the I2C bus 2 at address 0x40
const htu21d = new addon.Htu21d('/dev/i2c-2', 0x40);
```

#####Get basic device info
```
const name = htu21d.deviceName();  // returns string with name of device
const version = htu21d.deviceVersion(); // returns this software version
const active = htu21d.deviceActive(); // true if active, false if inactive
const numVals =  htu21d.deviceNumValues(); // returns the number of paramters sensed
```

####Get parameter info and values
The HTU21D Sensor has two parameters, so there are two corresponding indicies.
```
const paramName0 = htu21d.nameAtIndex(0);
const paramType0 = htu21d.typeAtIndex(0);
const paramVal0  = htu21d.valueAtIndexSync(0);
const paramName1 = htu21d.nameAtIndex(1);
const paramType1 = htu21d.typeAtIndex(1);
const paramVal1  = htu21d.valueAtIndexSync(1);
```

####Asynchronous value collection is also available
```
htu21d.valueAtIndex(0, function(err, val) {
    if (err) {
        console.log(err);
    }
    else {
        console.log(`Asynchronous call return: ${val}`);
    }
});
```

###Operation Notes
The driver is specific to the HTU21D humidity and temperature sensor manufactured by Measurement Specialties. 
It will output both relative humidity RH% as well as tempurature in degrees celsius.  The humidity sensor has a 
3% tolerance accuracy, measuring 0-100% RH. The temperature sensor is accurate to ±0.3°C within the range of 
5-60°C, degrading to a max of ±1.6°C lower than 0°C and greater than 60°C, measuring -40 to +125 degrees C.


###Dependencies
* node-gyp

###Copyright
Copyright © 2016 Agilatech. All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

