##Node addon for I2C hardware HTU21D Humidity and Temperature sensor

#####Theoretically, this addon should work on any Linux platform, but has only been tested on BBB

###Install

```
npm install @agilatech/htu21d
```
```
git clone https://github.com/Agilatech/htu21d.git
node-gyp configure build
```

###Usage
#####Load the module and create and instance
```
const addon = require('htu21d');

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
####Get paramter info and values
```
const paramName0 = htu21d.nameAtIndex(0);
const paramType0 = htu21d.typeAtIndex(0);
const paramVal0  = htu21d.valueAtIndexSync(0);
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

###Dependencies
* node-gyp

