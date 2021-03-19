int LED_PIN = 23;

const int REQUEST_GPIO_CONFIGURE = 1;
const int REQUEST_GPIO_READ = 2;
const int REQUEST_GPIO_WRITE = 3;

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
  void txComplete(int length);
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
    i2cMaster.init(&target::SERCOM0, 8000000, 100000, txBuffer, sizeof(txBuffer), rxBuffer + 1, sizeof(rxBuffer) - 1);
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
void I2CMaster::txComplete(int length) {
  endpoint->txBuffer[0] = length;
  endpoint->startTx(1);
}

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

  bool openDrain[32];

  void setup(SetupData *setup) {
    if (setup->bmRequestType.type == VENDOR) {
      switch (setup->bRequest) {
      case REQUEST_GPIO_CONFIGURE: {
        int wValue = setup->wValue;
        int pin = wValue & 0xFF;

        target::PORT.PINCFG[pin].setINEN(true);

        if ((wValue >> 8) & 1) {
          // output
          bool openDrain = (wValue >> 9) & 1;
          bool pullUp = (wValue >> 10) & 1;

          this->openDrain[pin] = openDrain;

          if (openDrain) {
            target::PORT.PINCFG[pin].setPULLEN(pullUp);
            target::PORT.DIRCLR = 1 << pin;
            target::PORT.OUTSET = 1 << pin;
          } else {
            target::PORT.OUTCLR = 1 << pin;
            target::PORT.PINCFG[pin].setPULLEN(false);
            target::PORT.DIRSET = 1 << pin;
          }

        } else {
          // input
          target::PORT.DIRCLR = 1 << pin;
          bool pullDown = (wValue >> 9) & 1;
          bool pullUp = (wValue >> 10) & 1;
          target::PORT.PINCFG[pin].setPULLEN(pullDown || pullUp);
          if (pullDown) {
            target::PORT.OUTCLR = 1 << pin;
          }
          if (pullUp) {
            target::PORT.OUTSET = 1 << pin;
          }
        }

        device->getControlEndpoint()->startTx(0);
        break;
      }
      case REQUEST_GPIO_WRITE: {
        int wValue = setup->wValue;
        int pin = wValue & 0xFF;
        int state = (wValue >> 8) & 1;
        if (openDrain[pin]) {
          if (state) {
            target::PORT.OUTSET = 1 << pin;
            target::PORT.DIRCLR = 1 << pin;
          } else {
            target::PORT.OUTCLR = 1 << pin;
            target::PORT.DIRSET = 1 << pin;
          }
        } else {
          if (state) {
            target::PORT.OUTSET = 1 << pin;
          } else {
            target::PORT.OUTCLR = 1 << pin;
          }
        }
        device->getControlEndpoint()->startTx(0);
        break;
      }
      }
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
