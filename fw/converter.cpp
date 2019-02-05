target::gpio_b_f::Peripheral *LED_PORT = &target::GPIOB;
int LED_PIN = 7;

class TurnLedOffTimer : public genericTimer::Timer
{

	void onTimer()
	{
		LED_PORT->ODR.setODR(LED_PIN, 0);
	}
};

TurnLedOffTimer turnLedOffTimer;

class DataEndpoint;

class ConverterI2C : public i2c::hw::Master
{
  public:
	DataEndpoint *endpoint;

	virtual void onRx(int byte, int index);
	virtual int onTx(int index);
	virtual void onStop(bool read, int error);
};

class DataEndpoint : public usbd::AbstractBulkEndpoint
{

  public:
	ConverterI2C i2c;

	unsigned char i2cBuffer[64];

	void init()
	{
		rxBufferSize = 64;
		txBufferSize = 64;
		AbstractBulkEndpoint::init();

		this->i2c.endpoint = this;
		this->i2c.init(&target::I2C1);
	}

	void correctTransferIn()
	{
	}

	void correctTransferOut(unsigned char *data, int len)
	{

		LED_PORT->ODR.setODR(LED_PIN, 1);
		turnLedOffTimer.start(10);

		if (len > 0)
		{

			bool doRead = data[0] & 1;
			int address = data[0] >> 1;

			if (doRead)
			{
				int readLen = data[1];
				i2c.read(address, readLen);
			}
			else
			{
				for (int c = 0; c < len - 1; c++)
				{
					i2cBuffer[c] = data[c + 1];
				}
				i2c.write(address, len - 1);
			}
		}

		statRx(usbd::EP_VALID);
	}
};

class AlertEndpoint : public usbd::AbstractInterruptEndpoint
{
  public:
	void handleInterrupt()
	{
		unsigned char buffer[1] = {1};
		send(buffer, sizeof(buffer));
	}
};

void ConverterI2C::onRx(int byte, int index)
{
	endpoint->i2cBuffer[index] = byte;
}

int ConverterI2C::onTx(int index)
{
	return endpoint->i2cBuffer[index];
}

void ConverterI2C::onStop(bool read, int error)
{

	int dataLen = read ? length : 0;

	unsigned char buffer[dataLen + 1];
	buffer[0] = error;
	for (int c = 0; c < dataLen; c++)
	{
		buffer[c + 1] = endpoint->i2cBuffer[c];
	}
	endpoint->send(buffer, dataLen + 1);
}

class DefaultInterface : public usbd::UsbInterface
{
  public:
	DataEndpoint dataEndpoint;
	AlertEndpoint alertEndpoint;

	void init()
	{
		endpoints[0] = &dataEndpoint;
		endpoints[1] = &alertEndpoint;
		UsbInterface::init();
	}
};

class DefaultConfiguration : public usbd::UsbConfiguration
{
  public:
	DefaultInterface interface;

	void init()
	{
		interfaces[0] = &interface;
		maxPower = 500;
		UsbConfiguration::init();
	}
};

class ConverterDevice : public usbd::UsbDevice
{
  public:
	DefaultConfiguration configuration;

	void init()
	{
		strProduct = "USB to I2C converter";
		strVendor = "DEVICE.FARM";
		configurations[0] = &configuration;
		UsbDevice::init();
	}
};

ConverterDevice converterDevice;

void interruptHandlerI2C1()
{
	converterDevice.configuration.interface.dataEndpoint.i2c.handleInterrupt();
}

void interruptHandlerEXTI4_15()
{
	if (target::EXTI.PR.getPIF(8))
	{
		target::EXTI.PR.setPIF(8, 1);
		converterDevice.configuration.interface.alertEndpoint.handleInterrupt();
	}
}

void initApplication()
{

	genericTimer::clkHz = 48E6;

	target::RCC.AHBENR.setIOPAEN(true);
	target::RCC.AHBENR.setIOPBEN(true);

	// LED
	LED_PORT->MODER.setMODER(LED_PIN, 1);
	LED_PORT->ODR.setODR(LED_PIN, 1);
	turnLedOffTimer.start(100);

	// Alert
	target::GPIOA.MODER.setMODER(8, 0);
	target::GPIOA.PUPDR.setPUPDR(8, 1);
	target::EXTI.FTSR.setTR(8, 1);
	target::EXTI.IMR.setMR(8, 1);

	// I2C
	target::GPIOA.AFRH.setAFRH(9, 4);
	target::GPIOA.AFRH.setAFRH(10, 4);
	target::GPIOA.MODER.setMODER(9, 2);
	target::GPIOA.MODER.setMODER(10, 2);
	target::RCC.APB1ENR.setC_EN(1, 1);

	converterDevice.init();

	// interrupts
	target::SYSCFG_COMP.SYSCFG_EXTICR1.setEXTI(8, 0);
	target::NVIC.ISER.setSETENA(1 << target::interrupts::External::EXTI4_15);
	target::NVIC.ISER.setSETENA(1 << target::interrupts::External::I2C1);
}
