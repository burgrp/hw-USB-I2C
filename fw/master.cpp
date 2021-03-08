namespace atsamd::i2c {
class Master {
  volatile target::sercom::Peripheral *sercom;

public:
  void init(volatile target::sercom::Peripheral *sercom, int genHz, int sclHz) {

    this->sercom = sercom;

    int baud = genHz / (2 * sclHz);
    sercom->I2CM.BAUD = sercom->I2CM.BAUD.bare().setBAUD(baud).setBAUDLOW(baud);

    sercom->I2CM.INTENSET = sercom->I2CM.INTENSET.bare().setMB(true).setSB(true);

    sercom->I2CM.CTRLA = sercom->I2CM.CTRLA.bare()
                             .setMODE(target::sercom::I2CM::CTRLA::MODE::I2C_MASTER)
                             .setSCLSM(false)
                             .setENABLE(true);

    while (sercom->I2CM.SYNCBUSY)
      ;

    sercom->I2CM.STATUS.setBUSSTATE(1); // force bus idle state
  }
};
} // namespace atsamd::i2c
