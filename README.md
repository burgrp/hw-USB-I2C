# USB-I2C bridge

Another USB to I2C bridge implemented on ATSAMD11C14A MCU.

![board](board.png)

## Build and flash firmware

Install [node.js](https://nodejs.org) and [OpenOCD](http://openocd.org/).

Install [silicon](https://nodejs.org) build system:
```
sudo npm install --global @si14/silicon
```

Run openocd with proper interface config file:
```
openocd -f interface/cmsis-dap.cfg -f target/at91samdXX.cfg
```

Build and flash from [fw](fw) directory:
```
silicon build -df
```

## Connector pinout
TBD

## node.js binding
TBD

## USB protocol
TBD

## License
This work is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.