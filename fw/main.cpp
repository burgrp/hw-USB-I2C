int LED_PIN = 23;

enum LedMode { OFF, ON, BLINK };

LedMode ledMode = BLINK;

class ToggleTimer : public genericTimer::Timer {

  void onTimer() {
    if (ledMode == OFF) {
      target::PORT.OUTCLR.setOUTCLR(1 << LED_PIN);
    } else if (ledMode == ON) {
      target::PORT.OUTSET.setOUTSET(1 << LED_PIN);
    } else if (ledMode == BLINK) {
      target::PORT.OUTTGL.setOUTTGL(1 << LED_PIN);
    }
    start(10);
  }
};

ToggleTimer timer;

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

  virtual UsbEndpoint *getEndpoint(int index) { return index == 0 ? &irqEndpoint : NULL; }

  const char *getLabel() { return "USB-I2C bridge interface"; }

  void setup(SetupData *setup) {
    if (setup->bRequest = 0x10) {
      ledMode = (LedMode)setup->wValue;
      device->getControlEndpoint()->startTx(0);
    }
  }
};

class TestDevice : public atsamd::usbd::AtSamdUsbDevice {
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

TestDevice testDevice;

void interruptHandlerUSB() { testDevice.interruptHandlerUSB(); }

void initApplication() {

  atsamd::safeboot::init(9, false, LED_PIN);

  testDevice.useInternalOscilators();
  testDevice.init();

  timer.start(10);
}
