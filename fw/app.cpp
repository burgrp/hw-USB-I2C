target::gpio_b_f::Peripheral* LED_PORT = &target::GPIOB;
int LED_PIN = 7;

namespace i2c {
	class I2C: public applicationEvents::EventHandler {

	protected:
		target::i2c::Peripheral* peripheral;
		int length = 0;
		int index = 0;
		int stopEventId;

		void start(int address, int length, int rdWrn) {
			this->length = length;
			this->index = 0;
			peripheral->CR2.setSADD(address << 1);
			peripheral->CR2.setNBYTES(length);
			peripheral->CR2.setRD_WRN(rdWrn);
			peripheral->CR2.setSTART(1);
		}

	public:
		
		void init(target::i2c::Peripheral* peripheral) {
			this->peripheral = peripheral;
			peripheral->TIMINGR.setPRESC(1);
			peripheral->TIMINGR.setSCLL(0xC7);
			peripheral->TIMINGR.setSCLH(0xC3);
			peripheral->TIMINGR.setSDADEL(0x02);
			peripheral->TIMINGR.setSCLDEL(0x04);
			peripheral->CR2.setAUTOEND(1);
			peripheral->CR1.setRXIE(1);
			peripheral->CR1.setTXIE(1);
			peripheral->CR1.setSTOPIE(1);
			peripheral->CR1.setPE(1);
			stopEventId = applicationEvents::createEventId();
			handle(stopEventId);
		}
		
		virtual void onEvent() {
			onStop(peripheral->CR2.getRD_WRN());
		}

		void handleInterrupt() {

			if (peripheral->ISR.getRXNE()) {
				int byte = this->peripheral->RXDR.getRXDATA();
				onRx(byte, this->index++);
			}

			if (peripheral->ISR.getTXIS()) {
				peripheral->TXDR.setTXDATA(onTx(index));
			}

			if (peripheral->ISR.getSTOPF()) {				
				peripheral->ICR.setSTOPCF(1);
				applicationEvents::schedule(stopEventId);
			}
			
		}
		
		virtual void onRx(int byte, int index) {
		}

		virtual int onTx(int index) {
			return 0;
		}

		virtual void onStop(bool read) {
		}

		void read(int address, int length) {
			this->start(address, length, 1);
		}

		void write(int address, int length) {
			this->start(address, length, 0);
		}
	};

}

class TurnLedOffTimer : public genericTimer::Timer {

	void onTimer() {
		LED_PORT->ODR.setODR(LED_PIN, 0);
	}

};

TurnLedOffTimer turnLedOffTimer;

class DataEndopoint;

class ConverterI2C: public i2c::I2C {
public:

	DataEndopoint* endpoint;

	virtual void onRx(int byte, int index);
	virtual int onTx(int index);
	virtual void onStop(bool read);
};

class DataEndopoint : public usbd::AbstractBulkEndpoint {
    
public:
    
	ConverterI2C i2c;

	unsigned char i2cBuffer[64];

	void init() {
        rxBufferSize = 64;
        txBufferSize = 64;
        AbstractBulkEndpoint::init();

		this->i2c.endpoint = this;
		this->i2c.init(&target::I2C1);		
    }

    void correctTransferIn() {
    }
    
    void correctTransferOut(unsigned char* data, int len) {

		LED_PORT->ODR.setODR(LED_PIN, 1);
		turnLedOffTimer.start(10);

		if (len > 0) {
			
			bool doRead = data[0] & 1;
			int address = data[0] >> 1;
			
			if (doRead) {
				int readLen = data[1];				
				i2c.read(address, readLen);
				
			} else {
				for (int c = 0; c < len - 1; c++) {
					i2cBuffer[c] = data[c + 1];
				}
				i2c.write(address, len - 1);
			}
		}
			
        statRx(usbd::EP_VALID);		
    }

};

void ConverterI2C::onRx(int byte, int index) {
	endpoint->i2cBuffer[index] = byte;
}

int ConverterI2C::onTx(int index) {
	return endpoint->i2cBuffer[index];
}

void ConverterI2C::onStop(bool read) {
	if (read) {
		endpoint->send(endpoint->i2cBuffer, length);
	}
}


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

void interruptHandlerI2C1() {
	converterDevice.configuration.interface.endpoint.i2c.handleInterrupt();
}

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
	target::NVIC.ISER.setSETENA(1 << target::interrupts::External::I2C1);
	
	converterDevice.init();
}
