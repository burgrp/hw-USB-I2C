int LED_PIN = 23;

const int REQUEST_I2C_READ = 0;
const int REQUEST_I2C_WRITE = 1;
const int REQUEST_GPIO_CONFIGURE_INPUT = 2;
const int REQUEST_GPIO_READ_INPUT = 3;
const int REQUEST_GPIO_CONFIGURE_OUTPUT = 4;
const int REQUEST_GPIO_WRITE_OUTPUT = 5;

class TestTimer : public genericTimer::Timer {

  void onTimer() {
    target::PORT.OUTTGL.setOUTTGL(1 << LED_PIN);
    start(10);

    target::SERCOM0.I2CM.ADDR.setADDR(0x4F << 1);
    //target::SERCOM0.I2CM.DATA = 0xFF;

  }
};

class LedPulseTimer : public genericTimer::Timer {

  void onTimer() { target::PORT.OUTCLR.setOUTCLR(1 << LED_PIN); }

public:
  void pulse() {
    target::PORT.OUTSET.setOUTSET(1 << LED_PIN);
    start(100);
  }
};

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
  LedPulseTimer ledPulseTimer;

  virtual UsbEndpoint *getEndpoint(int index) { return index == 0 ? &irqEndpoint : NULL; }

  const char *getLabel() { return "USB-I2C bridge interface"; }

  void setup(SetupData *setup) {
    if (setup->bmRequestType.type == VENDOR) {
      switch (setup->bRequest) {
      case REQUEST_I2C_READ:
        ledPulseTimer.pulse();
        device->getControlEndpoint()->startTx(setup->wLength);
        // device->getControlEndpoint()->stall();
        break;
      case REQUEST_I2C_WRITE:
        ledPulseTimer.pulse();
        device->getControlEndpoint()->startTx(0);
        // device->getControlEndpoint()->stall();
        break;
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

TestTimer testTimer;

void interruptHandlerUSB() { bridgeDevice.interruptHandlerUSB(); }

void initApplication() {

  atsamd::safeboot::init(9, false, LED_PIN);

  bridgeDevice.useInternalOscilators();
  bridgeDevice.init();

  target::PM.APBCMASK.setSERCOM(0, true);

  target::GCLK.CLKCTRL = target::GCLK.CLKCTRL.bare()
                             .setID(target::gclk::CLKCTRL::ID::SERCOM0_CORE)
                             .setGEN(target::gclk::CLKCTRL::GEN::GCLK0)
                             .setCLKEN(true);

  target::GCLK.CLKCTRL = target::GCLK.CLKCTRL.bare()
                             .setID(target::gclk::CLKCTRL::ID::SERCOMX_SLOW)
                             .setGEN(target::gclk::CLKCTRL::GEN::GCLK0)
                             .setCLKEN(true);

  target::PORT.PMUX[7].setPMUXE(target::port::PMUX::PMUXE::C);
  target::PORT.PMUX[7].setPMUXO(target::port::PMUX::PMUXO::C);

  target::PORT.PINCFG[14].setPMUXEN(true);
  target::PORT.PINCFG[15].setPMUXEN(true);

  //target::SERCOM0.I2CM.SYNCBUSY.setENABLE(true);

  // SCL at 100kHz for GCLK0 at 8MHz 
  target::SERCOM0.I2CM.BAUD = target::SERCOM0.I2CM.BAUD.bare().setBAUD(35).setBAUDLOW(35);

  target::SERCOM0.I2CM.CTRLB = target::SERCOM0.I2CM.CTRLB.bare().setSMEN(true);

  target::SERCOM0.I2CM.CTRLA = target::SERCOM0.I2CM.CTRLA.bare()
                                   .setSDAHOLD(2)
                                   .setMODE(target::sercom::I2CM::CTRLA::MODE::I2C_MASTER)
                                   .setENABLE(true);

  target::SERCOM0.I2CM.STATUS.setBUSSTATE(1);

  testTimer.start(100);
}
