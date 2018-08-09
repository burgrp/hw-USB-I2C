target::gpio_b_f::Peripheral* LED_PORT = &target::GPIOB;
int LED_PIN = 7;
target::i2c::Peripheral* I2C = &target::I2C1;

class TurnLedOffTimer : public genericTimer::Timer {

	void onTimer() {
		LED_PORT->ODR.setODR(LED_PIN, 0);
	}

};

TurnLedOffTimer turnLedOffTimer;

class DataEndopoint : public usbd::AbstractBulkEndpoint {
    
public:
    
	void init() {
        rxBufferSize = 64;
        txBufferSize = 64;
        AbstractBulkEndpoint::init();
    }

    void correctTransferIn() {
    }
    
	void i2cRead(int address, unsigned char* data, int len) {
		data[0] = 5;
	}

	void i2cWrite(int address, unsigned char* data, int len) {
		volatile int x = len + data[0];
	}

    void correctTransferOut(unsigned char* data, int len) {

		LED_PORT->ODR.setODR(LED_PIN, 1);
		turnLedOffTimer.start(10);

		if (len > 0) {
			
			bool doRead = data[0] & 1;
			int address = data[0] >> 1;
			
			if (doRead) {
				int readLen = data[1];
				unsigned char buffer[readLen];
				i2cRead(address, buffer, readLen);
				send(buffer, readLen);
			} else {
				i2cWrite(address, &data[1], len - 1);
			}
		}
			
        statRx(usbd::EP_VALID);
    }

};

class DefaultInterface : public usbd::UsbInterface {
public:
		DataEndopoint endpoint;

		void init() {
			endpoints[0] = &endpoint;
			UsbInterface::init();
		}
};

class DefaultConfiguration : public usbd::UsbConfiguration {
public:
	DefaultInterface interface;

	void init() {
		interfaces[0] = &interface;
		maxPower = 500;
		UsbConfiguration::init();
	}
};

class ConverterDevice : public usbd::UsbDevice {
public:
	DefaultConfiguration configuration;

	void init() {		
		strProduct = "USB to I2C converter";
		strVendor ="DEVICE.FARM";
		configurations[0] = &configuration;		
		UsbDevice::init();
	}
};

ConverterDevice converterDevice;

void initApplication() {

	genericTimer::clkHz = 48E6;

	target::RCC.AHBENR.setIOPBEN(true);
	LED_PORT->MODER.setMODER(LED_PIN, 1);
	LED_PORT->ODR.setODR(LED_PIN, 1);
	turnLedOffTimer.start(100);

	target::RCC.AHBENR.setIOPAEN(true);
	target::GPIOA.AFRH.setAFRH(9, 4);
	target::GPIOA.AFRH.setAFRH(10, 4);
	target::GPIOA.MODER.setMODER(9, 2);
	target::GPIOA.MODER.setMODER(10, 2);

	target::RCC.APB1ENR.setC_EN(1, 1);
	I2C->TIMINGR.setPRESC(1);
	I2C->TIMINGR.setSCLL(0xC7);
	I2C->TIMINGR.setSCLH(0xC3);
	I2C->TIMINGR.setSDADEL(0x02);
	I2C->TIMINGR.setSCLDEL(0x04);
	I2C->CR2.setAUTOEND(1);
	I2C->CR1.setPE(1);

while(true) {
	//I2C->CR2.setSADD(0xFE);
	I2C->CR2.setNBYTES(3);
	I2C->CR2.setRD_WRN(0);	
	I2C->CR2.setSTART(1);

	for (volatile int c = 0; c < 100000; c++);
}

	converterDevice.init();
}
