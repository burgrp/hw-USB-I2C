namespace atsamd::i2c {
class Master {
  volatile target::sercom::Peripheral *sercom;

public:
public:
  int rxLength;
  int rxLimit;

  int txLength;
  int txLimit;

  unsigned char *rxBufferPtr;
  unsigned int rxBufferSize;
  unsigned char *txBufferPtr;
  unsigned int txBufferSize;

  void init(volatile target::sercom::Peripheral *sercom, int genHz, int sclHz, unsigned char *rxBufferPtr,
            unsigned int rxBufferSize, unsigned char *txBufferPtr, unsigned int txBufferSize) {

    this->sercom = sercom;

    this->rxBufferPtr = rxBufferPtr;
    this->rxBufferSize = rxBufferSize;
    this->txBufferPtr = txBufferPtr;
    this->txBufferSize = txBufferSize;

    int baud = genHz / (2 * sclHz);
    sercom->I2CM.BAUD = sercom->I2CM.BAUD.bare().setBAUD(baud).setBAUDLOW(baud);

    sercom->I2CM.INTENSET = sercom->I2CM.INTENSET.bare().setMB(true).setSB(true); //.setERROR(true)

    sercom->I2CM.CTRLA = sercom->I2CM.CTRLA.bare()
                             .setMODE(target::sercom::I2CM::CTRLA::MODE::I2C_MASTER)
                             .setSCLSM(false)
                             .setENABLE(true);

    while (sercom->I2CM.SYNCBUSY)
      ;

    sercom->I2CM.STATUS.setBUSSTATE(1); // force bus idle state
  }

  void interruptHandlerSERCOM() {

    if (sercom->I2CM.INTFLAG.getMB()) {
      sercom->I2CM.INTFLAG.setMB(true);

      if (sercom->I2CM.STATUS.getBUSERR() || sercom->I2CM.STATUS.getRXNACK()) {

        if (sercom->I2CM.ADDR & 1) {
          rxComplete(txLength);
        } else {
          txComplete(txLength);
        }
        
        sercom->I2CM.CTRLB.setCMD(3);

      } else {

        if (txLength < txLimit) {
          sercom->I2CM.DATA = txBufferPtr[txLength++];
        } else {
          txComplete(txLength);
          sercom->I2CM.CTRLB.setCMD(3);
        }
      }
    }

    if (sercom->I2CM.INTFLAG.getSB()) {
      sercom->I2CM.INTFLAG.setSB(true);

      if (rxLength < rxLimit && !sercom->I2CM.STATUS.getRXNACK()) {
        rxBufferPtr[rxLength] = sercom->I2CM.DATA;
        rxLength++;
        if (rxLength < rxLimit) {
          sercom->I2CM.CTRLB.setCMD(2);
        } else {
          rxComplete(rxLength);
          sercom->I2CM.CTRLB.setCMD(3);
        }
      } else {
        rxComplete(rxLength);
        sercom->I2CM.CTRLB.setCMD(3);
      }
    }

    // if (sercom->I2CM.INTFLAG.getERROR()) {
    //   sercom->I2CM.INTFLAG.setERROR(true);

    // }
  }

  virtual void rxComplete(int length){};
  virtual void txComplete(int length){};

  virtual void startRx(int address, int length) {
    rxLength = 0;
    rxLimit = length > rxBufferSize ? rxBufferSize : length;
    sercom->I2CM.ADDR.setADDR(address << 1 | 1);
  }

  virtual void startTx(int address, int length) {
    txLength = 0;
    txLimit = length > txBufferSize ? txBufferSize : length;
    sercom->I2CM.ADDR.setADDR(address << 1);
  }
};
} // namespace atsamd::i2c
