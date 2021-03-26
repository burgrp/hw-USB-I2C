# USB-I2C bridge

Another USB to I2C bridge implemented on ATSAMD11C14A MCU.

![board](board.png)

## I2C bus

There is one I2C supported. I2C operations are controlled by USB bulk endpoint 1 on interface 0.

## GPIO
Although the board has only one GPIO connected to ports (it is pin 5 for Alert/IRQ signal),
the firmware allows arbitrary GPIO configurations. This allows reuse of the firmware on other boards with ATSAMD11C14A chips. 

Each pin can be configured for output or input with interrupt support.

GPIO operations are controlled by interface 0 vendor requests.

Interrupts are read by interrupt endpoint 2 on interface 0.

## Connector pinout
1. GND
2. SCL
3. SDA
4. +3.3 V
5. +5V
6. ALERT (IRQ)

Alert/IRQ pin is GPIO 5 and needs to be configured by software, see [USB protocol](#usbprotocol).

## Binding

For Node.js there is a library [i2c-js](https://github.com/burgrp/i2c-js) published on NPM as [@burgrp/i2c](https://www.npmjs.com/package/@burgrp/i2c).

## USB protocol

- VID: 0x1209
- PID: 0x7070

### Device descriptors

```

Bus 003 Device 011: ID 1209:7070 Generic eGalaxTouch EXC7903-66v03_T1
Device Descriptor:
  bLength                18
  bDescriptorType         1
  bcdUSB               2.00
  bDeviceClass            0 
  bDeviceSubClass         0 
  bDeviceProtocol         0 
  bMaxPacketSize0        64
  idVendor           0x1209 Generic
  idProduct          0x7070 
  bcdDevice            0.00
  iManufacturer           1 Drake
  iProduct                2 USB-I2C bridge
  iSerial                 3 (device unique)
  bNumConfigurations      1
  Configuration Descriptor:
    bLength                 9
    bDescriptorType         2
    wTotalLength       0x0027
    bNumInterfaces          1
    bConfigurationValue     1
    iConfiguration          0 
    bmAttributes         0x80
      (Bus Powered)
    MaxPower              100mA
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        0
      bAlternateSetting       0
      bNumEndpoints           3
      bInterfaceClass       255 Vendor Specific Class
      bInterfaceSubClass      0 
      bInterfaceProtocol      0 
      iInterface              4 USB-I2C bridge interface
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x01  EP 1 OUT
        bmAttributes            2
          Transfer Type            Bulk
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0040  1x 64 bytes
        bInterval              10
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x81  EP 1 IN
        bmAttributes            2
          Transfer Type            Bulk
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0040  1x 64 bytes
        bInterval              10
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x82  EP 2 IN
        bmAttributes            3
          Transfer Type            Interrupt
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0004  1x 4 bytes
        bInterval              10
Device Status:     0x0000
  (Bus Powered)
```

### Interface 0 vendor requests

#### REQUEST_GPIO_CONFIGURE

Configure GPIO for output or input with optional interrupt.

- direction: OUT
- bmRequestType: 0x41
- bRequest: 1
- wIndex: 0
- wValue:
  - bit 0-7: pin number
  - bit 8 = 0 to configure pin as **input**
    - bit 9: pull-down
    - bit 10: pull-up
    - bit 11: enable irq on rising edge
    - bit 12: enable irq on falling edge
  - bit 8 = 1 to configure pin as **output**
    - bit 9: open drain
    - bit 10: pull-up (only valid for open drain)

#### REQUEST_GPIO_READ

- direction: IN
- bmRequestType: 0xC1
- bRequest: 2
- wIndex: 0
- wValue:
  - bit 0-7: pin number

#### IN payload
single byte 0 or 1 matching the pin state

#### REQUEST_GPIO_WRITE

- direction: OUT
- bmRequestType: 0x41
- bRequest: 3
- wIndex: 0
- wValue:
  - bit 0-7: pin number
  - bit 8: pin state

### Endpoint 1

Endpoint 1 is used for I2C operations as follows:

#### I2C write

Host writes data to the endpoint in format:

| byte | meaning                          |
|:----:|:---------------------------------|
| 0    | bit 0: 0; bits 1-7: I2C address |
| 1...end | data to be transmitted        |

Host reads data from endpoint in format:

| byte | meaning                                  |
|:----:|:-----------------------------------------|
| 0    | number of bytes successfully transmitted |

#### I2C read

Host writes data to the endpoint in format:

| byte | meaning                          |
|:----:|:---------------------------------|
| 0    | bit 0: 1; bits 1-7: I2C address |
| 1    | number of bytes to be received   |

Host reads data from endpoint in format:

| byte    | meaning                          |
|:-------:|:---------------------------------|
| 0...end | received data                    |

### Endpoint 2

Endpoint 2 is used to receive GPIO interrupt.

Host reads data from endpoint in format:

| byte | meaning                          |
|:----:|:---------------------------------|
| 0-3  | 32 bits of interrupt flags; bit 0 matches interrupt on pin 0 etc... |

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

## License
This work is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.