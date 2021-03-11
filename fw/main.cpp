int LED_PIN = 23;

const int REQUEST_GPIO_CONFIGURE_INPUT = 2;
const int REQUEST_GPIO_READ_INPUT = 3;
const int REQUEST_GPIO_CONFIGURE_OUTPUT = 4;
const int REQUEST_GPIO_WRITE_OUTPUT = 5;

class LedPulseTimer : public genericTimer::Timer {

  void onTimer() { target::PORT.OUTCLR.setOUTCLR(1 << LED_PIN); }

public:
  void pulse() {
    target::PORT.OUTSET.setOUTSET(1 << LED_PIN);
    start(1);
  }
};

class I2CEndpoint;

class I2CMaster : public atsamd::i2c::Master {
public:
  I2CEndpoint *endpoint;
  void rxComplete(int length);
};

class I2CEndpoint : public usbd::UsbEndpoint {
public:
  unsigned char rxBuffer[256];
  unsigned char txBuffer[256];

  LedPulseTimer ledPulseTimer;
  I2CMaster i2cMaster;

  void init() {
    rxBufferPtr = rxBuffer;
    rxBufferSize = sizeof(rxBuffer);
    txBufferPtr = txBuffer;
    txBufferSize = sizeof(txBuffer);
    transferType = BULK;
    usbd::UsbEndpoint::init();

    i2cMaster.endpoint = this;
    i2cMaster.init(&target::SERCOM0, 8000000, 100000, txBuffer, sizeof(txBuffer), rxBuffer + 1,
                   sizeof(rxBuffer) - 1);
  }

  void rxComplete(int length) {
    ledPulseTimer.pulse();
    if (length > 0) {
      int address = rxBuffer[0] >> 1;
      if (rxBuffer[0] & 1) {        
        if (length > 1) {
          i2cMaster.startRx(address, rxBuffer[1]);
        }
      } else {
        i2cMaster.startTx(address, length - 1);
      }
    }
  }
};

void I2CMaster::rxComplete(int length) { endpoint->startTx(length); }

class IrqEndpoint : public usbd::UsbEndpoint {
public:
  unsigned char txBuffer[8];

  void init() {
    txBufferPtr = txBuffer;
    txBufferSize = sizeof(txBuffer);
    usbd::UsbEndpoint::init();
  }
};

class BridgeInterface : public usbd::UsbInterface {
public:
  IrqEndpoint irqEndpoint;
  I2CEndpoint i2cEndpoint;

  virtual UsbEndpoint *getEndpoint(int index) {
    return index == 0 ? (UsbEndpoint *)&i2cEndpoint : index == 1 ? (UsbEndpoint *)&irqEndpoint : NULL;
  }

  const char *getLabel() { return "USB-I2C bridge interface"; }

  void setup(SetupData *setup) {
    if (setup->bmRequestType.type == VENDOR) {
      // switch (setup->bRequest) {
      // case REQUEST_I2C_READ:
      //   ledPulseTimer.pulse();
      //   device->getControlEndpoint()->startTx(setup->wLength);
      //   // device->getControlEndpoint()->stall();
      //   break;
      // case REQUEST_I2C_WRITE:
      //   ledPulseTimer.pulse();
      //   device->getControlEndpoint()->startTx(0);
      //   // device->getControlEndpoint()->stall();
      //   break;
      // }
    }
  }
};

class BridgeDevice : public atsamd::usbd::AtSamdUsbDevice {
public:
  BridgeInterface bridgeInterface;

  UsbControlEndpoint controlEndpoint;

  UsbInterface *getInterface(int index) { return index == 0 ? &bridgeInterface : NULL; };

  UsbEndpoint *getControlEndpoint() { return &controlEndpoint; };

  void checkDescriptor(DeviceDescriptor *deviceDesriptor) {
    deviceDesriptor->idVendor = 0x1209;
    deviceDesriptor->idProduct = 0x7070;
  };

  const char *getManufacturer() { return "Drake"; }
  const char *getProduct() { return "USB-I2C bridge"; }
};

BridgeDevice bridgeDevice;

void interruptHandlerUSB() { bridgeDevice.interruptHandlerUSB(); }

void interruptHandlerSERCOM0() { bridgeDevice.bridgeInterface.i2cEndpoint.i2cMaster.interruptHandlerSERCOM(); }

void initApplication() {

  atsamd::safeboot::init(9, false, LED_PIN);

  bridgeDevice.useInternalOscilators();

  target::PM.APBCMASK.setSERCOM(0, true);

  target::GCLK.CLKCTRL = target::GCLK.CLKCTRL.bare()
                             .setID(target::gclk::CLKCTRL::ID::SERCOM0_CORE)
                             .setGEN(target::gclk::CLKCTRL::GEN::GCLK0)
                             .setCLKEN(true);

  target::PORT.PMUX[7].setPMUXE(target::port::PMUX::PMUXE::C);
  target::PORT.PMUX[7].setPMUXO(target::port::PMUX::PMUXO::C);

  target::PORT.PINCFG[14].setPMUXEN(true);
  target::PORT.PINCFG[15].setPMUXEN(true);

  target::NVIC.ISER.setSETENA(1 << target::interrupts::External::SERCOM0);

  bridgeDevice.init();
  
}
