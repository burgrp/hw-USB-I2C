int LED_PIN = 2;
int SAFEBOOT_PIN = 8;

const int REQUEST_GPIO_CONFIGURE = 1;
const int REQUEST_GPIO_READ = 2;
const int REQUEST_GPIO_WRITE = 3;

const int PIN_TO_EXTINT[32] = {
    -1, // PA00
    -1, // PA01
    2,  // PA02
    3,  // PA03
    4,  // PA04
    5,  // PA05
    6,  // PA06
    7,  // PA07
    6,  // PA08
    7,  // PA09
    2,  // PA10
    3,  // PA11
    -1, // PA12
    -1, // PA13
    -1, // PA14
    1,  // PA15
    0,  // PA16
    1,  // PA17
    -1, // PA18
    -1, // PA19
    -1, // PA20
    -1, // PA21
    6,  // PA22
    7,  // PA23
    4,  // PA24
    5,  // PA25
    -1, // PA26
    7,  // PA27
    -1, // PA28
    -1, // PA29
    2,  // PA30
    3,  // PA31
};

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
  unsigned int txBuffer;

  void init() {
    txBufferPtr = (unsigned char *)&txBuffer;
    txBufferSize = sizeof(txBuffer);
    usbd::UsbEndpoint::init();
  }

  void signal(int pinFlags) {
    txBuffer = pinFlags;
    startTx(sizeof(txBuffer));
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
          bool irqRisingEdge = (wValue >> 11) & 1;
          bool irqFallingEdge = (wValue >> 12) & 1;

          target::PORT.PINCFG[pin].setPULLEN(pullDown || pullUp);
          if (pullDown) {
            target::PORT.OUTCLR = 1 << pin;
          }
          if (pullUp) {
            target::PORT.OUTSET = 1 << pin;
          }

          int extInt = PIN_TO_EXTINT[pin];
          if (irqRisingEdge || irqFallingEdge) {
            target::EIC.CONFIG.setFILTEN(extInt, true);
            if (pin & 1) {
              target::PORT.PMUX[pin >> 1].setPMUXO(target::port::PMUX::PMUXO::A);
            } else {
              target::PORT.PMUX[pin >> 1].setPMUXE(target::port::PMUX::PMUXE::A);
            }
            target::PORT.PINCFG[pin].setPMUXEN(true);
            target::EIC.INTENSET.setEXTINT(extInt, true);
          } else {
            target::EIC.INTENCLR.setEXTINT(extInt, false);
            target::PORT.PINCFG[pin].setPMUXEN(false);
          }
          target::EIC.CONFIG.setSENSE(extInt, irqRisingEdge ? irqFallingEdge ? target::eic::CONFIG::SENSE::BOTH
                                                                             : target::eic::CONFIG::SENSE::RISE
                                                            : irqFallingEdge ? target::eic::CONFIG::SENSE::FALL
                                                                             : target::eic::CONFIG::SENSE::NONE);
        }

        device->getControlEndpoint()->startTx(0);
        break;
      }
      case REQUEST_GPIO_READ: {
        int pin = setup->wValue & 0xFF;
        device->getControlEndpoint()->txBufferPtr[0] = target::PORT.IN.getIN() >> pin & 1;
        device->getControlEndpoint()->startTx(8);
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

void interruptHandlerEIC() {
  int intFlags = target::EIC.INTFLAG;

  int pinFlags = 0;
  for (int i = 0; i < 8; i++) {
    if ((intFlags >> i) & 1) {
      for (int p = 0; p < 32; p++) {
        if (PIN_TO_EXTINT[p] == i) {
          pinFlags |= 1 << p;
        }
      }
    }
  }

  bridgeDevice.bridgeInterface.irqEndpoint.signal(pinFlags);
  target::EIC.INTFLAG = 0xFF;
}

void initApplication() {

  // enable safeboot
  atsamd::safeboot::init(SAFEBOOT_PIN, false, LED_PIN);

  // 48MHz by internal RC oscillator
  bridgeDevice.useInternalOscillators();

  // clock to SERCOM0

  target::PM.APBCMASK.setSERCOM(0, true);

  target::GCLK.CLKCTRL = target::GCLK.CLKCTRL.bare()
                             .setID(target::gclk::CLKCTRL::ID::SERCOM0_CORE)
                             .setGEN(target::gclk::CLKCTRL::GEN::GCLK0)
                             .setCLKEN(true);

  // I2C pins

  target::PORT.PMUX[7].setPMUXE(target::port::PMUX::PMUXE::C);
  target::PORT.PMUX[7].setPMUXO(target::port::PMUX::PMUXO::C);

  target::PORT.PINCFG[14].setPMUXEN(true);
  target::PORT.PINCFG[15].setPMUXEN(true);

  // enable EIC

  target::PM.APBAMASK.setEIC(true);

  target::GCLK.CLKCTRL = target::GCLK.CLKCTRL.bare()
                             .setID(target::gclk::CLKCTRL::ID::EIC)
                             .setGEN(target::gclk::CLKCTRL::GEN::GCLK0)
                             .setCLKEN(true);

  target::EIC.CTRL.setENABLE(true);

  // set interrupt priorities and endable

  target::NVIC.IPR[target::interrupts::External::EVSYS >> 2].setPRI(target::interrupts::External::SERCOM0 & 0x3, 0);
  target::NVIC.IPR[target::interrupts::External::USB >> 2].setPRI(target::interrupts::External::USB & 0x3, 2);
  target::NVIC.IPR[target::interrupts::External::EIC >> 2].setPRI(target::interrupts::External::EIC & 0x3, 3);

  target::NVIC.ISER.setSETENA(1 << target::interrupts::External::SERCOM0);
  target::NVIC.ISER.setSETENA(1 << target::interrupts::External::EIC);

  bridgeDevice.init();
}
