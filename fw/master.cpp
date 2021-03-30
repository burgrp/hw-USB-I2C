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

  const int CMD_ACK_AND_READ = 2;
  const int CMD_ACK_AND_STOP = 3;

  void interruptHandlerSERCOM() {

    if (sercom->I2CM.INTFLAG.getMB() || sercom->I2CM.INTFLAG.getSB()) {

      bool read = sercom->I2CM.ADDR & 1;

      if (sercom->I2CM.STATUS.getBUSERR() || sercom->I2CM.STATUS.getRXNACK()) {
        // errors

        if (read) {
          // read
          rxComplete(rxLength);
        } else {
          // write
          txComplete(txLength && txLength - 1);
        }

        sercom->I2CM.CTRLB.setCMD(CMD_ACK_AND_STOP);

      } else {
        if (read) {

          // read
          if (rxLength < rxLimit) {
            rxBufferPtr[rxLength++] = sercom->I2CM.DATA;
            sercom->I2CM.CTRLB.setCMD(CMD_ACK_AND_READ);
          } else {
            rxComplete(rxLength);
            sercom->I2CM.CTRLB.setCMD(CMD_ACK_AND_STOP);
          }

        } else {
          // write
          
          if (txLength < txLimit) {
            sercom->I2CM.DATA = txBufferPtr[txLength++];
          } else {
            txComplete(txLength);
            sercom->I2CM.CTRLB.setCMD(CMD_ACK_AND_STOP);
          }
          
        }
      }
    }

    // if (sercom->I2CM.INTFLAG.getMB()) {
    //   //sercom->I2CM.INTFLAG.setMB(true);

    //   if (sercom->I2CM.STATUS.getBUSERR() || sercom->I2CM.STATUS.getRXNACK()) {

    //     if (sercom->I2CM.ADDR & 1) {
    //       rxComplete(rxLength);
    //     } else {
    //       txComplete(txLength);
    //     }

    //     sercom->I2CM.CTRLB.setCMD(3);

    //   } else {

    //     if (txLength < txLimit) {
    //       sercom->I2CM.DATA = txBufferPtr[txLength++];
    //     } else {
    //       txComplete(txLength);
    //       sercom->I2CM.CTRLB.setCMD(3);
    //     }

    //   }
    // }

    // if (sercom->I2CM.INTFLAG.getSB()) {
    //   //sercom->I2CM.INTFLAG.setSB(true);

    //   if (rxLength < rxLimit && !sercom->I2CM.STATUS.getRXNACK()) {
    //     rxBufferPtr[rxLength] = sercom->I2CM.DATA;
    //     rxLength++;
    //     if (rxLength < rxLimit) {
    //       sercom->I2CM.CTRLB.setCMD(2);
    //     } else {
    //       rxComplete(rxLength);
    //       sercom->I2CM.CTRLB.setCMD(3);
    //     }
    //   } else {
    //     rxComplete(rxLength);
    //     sercom->I2CM.CTRLB.setCMD(3);
    //   }
    // }
  }

  virtual void rxComplete(int length){};
  virtual void txComplete(int length){};

  virtual void startRx(int address, int length) {
    rxLength = 0;
    rxLimit = length > rxBufferSize ? rxBufferSize : length;
    sercom->I2CM.ADDR.setADDR(address << 1 | 1);
    while (sercom->I2CM.SYNCBUSY)
      ;    
  }

  virtual void startTx(int address, int length) {
    txLength = 0;
    txLimit = length > txBufferSize ? txBufferSize : length;
    sercom->I2CM.ADDR.setADDR(address << 1);
    while (sercom->I2CM.SYNCBUSY)
      ;    
  }
};
} // namespace atsamd::i2c
