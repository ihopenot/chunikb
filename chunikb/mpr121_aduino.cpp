/*!
 * @file Adafruit_MPR121.cpp
 *
 *  @mainpage Adafruit MPR121 arduino driver
 *
 *  @section intro_sec Introduction
 *
 *  This is a library for the MPR121 I2C 12-chan Capacitive Sensor
 *
 *  Designed specifically to work with the MPR121 sensor from Adafruit
 *  ----> https://www.adafruit.com/products/1982
 *
 *  These sensors use I2C to communicate, 2+ pins are required to
 *  interface
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing
 *  products from Adafruit!
 *
 *  @section author Author
 *
 *  Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 *  @section license License
 *
 *  BSD license, all text here must be included in any redistribution.
 */

#include "mpr121_arduino.h"
#include "mpr121_config.h"

// uncomment to use autoconfig !
//#define AUTOCONFIG // use autoconfig (Yes it works pretty well!)

/*!
 *  @brief      Default constructor
 */
Adafruit_MPR121::Adafruit_MPR121() {}

/*!
 *  @brief    Begin an MPR121 object on a given I2C bus. This function resets
 *            the device and writes the default settings.
 *  @param    i2caddr
 *            the i2c address the device can be found on. Defaults to 0x5A.
 *  @param    *theWire
 *            Wire object
 *  @param    touchThreshold
 *            touch detection threshold value
 *  @param    releaseThreshold
 *            release detection threshold value
 *  @returns  true on success, false otherwise
 */
bool Adafruit_MPR121::begin(uint8_t i2caddr, TwoWire *theWire,
                            uint8_t touchThreshold, uint8_t releaseThreshold) {

  if (i2c_dev) {
    delete i2c_dev;
  }
  i2c_dev = new Adafruit_I2CDevice(i2caddr, theWire);

  if (!i2c_dev->begin()) {
    return false;
  }

  // soft reset
  writeRegister(MPR121_SOFTRESET, 0x63);
  delay(1);
  for (uint8_t i = 0; i < 0x7F; i++) {
    //  Serial.print("$"); Serial.print(i, HEX);
    //  Serial.print(": 0x"); Serial.println(readRegister8(i));
  }

  writeRegister(MPR121_ECR, 0x0);

  uint8_t c = readRegister8(MPR121_CONFIG2);

  if (c != 0x24)
    return false;

  // writeRegisterRaw(MPR121_MHDR, MPR121CONF_MHDR, false);
  // writeRegisterRaw(MPR121_NHDR, MPR121CONF_NHDR, false);
  // writeRegisterRaw(MPR121_NCLR, MPR121CONF_NCLR, false);
  // writeRegisterRaw(MPR121_FDLR, MPR121CONF_FDLR, false);
  // writeRegisterRaw(MPR121_MHDF, MPR121CONF_MHDF, false);
  // writeRegisterRaw(MPR121_NHDF, MPR121CONF_NHDF, false);
  // writeRegisterRaw(MPR121_NCLF, MPR121CONF_NCLF, false);
  // writeRegisterRaw(MPR121_FDLF, MPR121CONF_FDLF, false);
  // writeRegisterRaw(MPR121_NHDT, MPR121CONF_NHDT, false);
  // writeRegisterRaw(MPR121_NCLT, MPR121CONF_NCLT, false);
  // writeRegisterRaw(MPR121_FDLT, MPR121CONF_FDLT, false);
  // // Skip ELEPROX baseline controls
  // for (int i = 0; i < 12; i++) {//设置触发阈值和充放电流时间
  //   writeRegisterRaw(MPR121_TOUCHTH(i), MPR121CONF_TTH, false);
  //   writeRegisterRaw(MPR121_RELEASETH(i), MPR121CONF_RTH, false);
  //   writeRegisterRaw(MPR121_CDC(i), 0, false);
  //   writeRegisterRaw(MPR121_CDT(i), 0, false);
  // }
  // writeRegisterRaw(MPR121_DEBOUNCE, (MPR121CONF_DR << 4) | MPR121CONF_DT, false); //设置采样数
  // writeRegisterRaw(MPR121_AFE1, (MPR121CONF_FFI << 6) | MPR121CONF_CDC, false);
  // writeRegisterRaw(MPR121_AFE2, (MPR121CONF_CDT << 5) | (MPR121CONF_SFI << 3) | MPR121CONF_ESI, false);
  // // _ecr = (MPR121CONF_CL << 6) | (MPR121CONF_ELEPROX << 4) | MPR121CONF_ELE; // Write when start
  // // Skip GPIO configurations
  // writeRegisterRaw(MPR121_AUTOCONF0,
  //                  (MPR121CONF_AFES << 6) | (MPR121CONF_RETRY << 4) | (MPR121CONF_BVA << 2) | (MPR121CONF_ARE << 1) | MPR121CONF_ACE,
  //                  false);
  // writeRegisterRaw(MPR121_AUTOCONF1,
  //                  (MPR121CONF_SCTS << 7) | (MPR121CONF_OORIE << 2) | (MPR121CONF_ARFIE << 1) | MPR121CONF_ACFIE,
  //                  false);
  // writeRegisterRaw(MPR121_USL, MPR121CONF_USL, false);
  // writeRegisterRaw(MPR121_LSL, MPR121CONF_LSL, false);
  // writeRegisterRaw(MPR121_TL, MPR121CONF_TL, false);

  setThresholds(touchThreshold, releaseThreshold);
  writeRegister(MPR121_MHDR, 0x01);
  writeRegister(MPR121_NHDR, 0x08);
  writeRegister(MPR121_NCLR, 0x01);
  writeRegister(MPR121_FDLR, 0x00);

  writeRegister(MPR121_MHDF, 0x01);
  writeRegister(MPR121_NHDF, 0x01);
  writeRegister(MPR121_NCLF, 0x10);
  writeRegister(MPR121_FDLF, 0x04);

  writeRegister(MPR121_NHDT, 0x00);
  writeRegister(MPR121_NCLT, 0x00);
  writeRegister(MPR121_FDLT, 0x00);

  writeRegister(MPR121_DEBOUNCE, 0);
  writeRegister(MPR121_CONFIG1, 0x10); // default, 16uA charge current
  writeRegister(MPR121_CONFIG2, 0x20); // 0.5uS encoding, 1ms period

#define AUTOCONFIG
#ifdef AUTOCONFIG
  writeRegister(MPR121_AUTOCONFIG0, 0x0B);

  // correct values for Vdd = 3.3V
  writeRegister(MPR121_UPLIMIT, 200);     // ((Vdd - 0.7)/Vdd) * 256
  writeRegister(MPR121_TARGETLIMIT, 180); // UPLIMIT * 0.9
  writeRegister(MPR121_LOWLIMIT, 130);    // UPLIMIT * 0.65
#endif

  // enable X electrodes and start MPR121
  byte ECR_SETTING = // (MPR121CONF_CL << 6) | (MPR121CONF_ELEPROX << 4) | (MPR121CONF_ELE); // 0x0
      B10000000 + 12; // 5 bits for baseline tracking & proximity disabled + X
                      // amount of electrodes running (12)
  writeRegister(MPR121_ECR, ECR_SETTING); // start with above ECR setting

  return true;
}

/*!
 *  @brief      DEPRECATED. Use Adafruit_MPR121::setThresholds(uint8_t touch,
 *              uint8_t release) instead.
 *  @param      touch
 *              see Adafruit_MPR121::setThresholds(uint8_t touch, uint8_t
 * *release)
 *  @param      release
 *              see Adafruit_MPR121::setThresholds(uint8_t touch, *uint8_t
 * release)
 */
void Adafruit_MPR121::setThreshholds(uint8_t touch, uint8_t release) {
  setThresholds(touch, release);
}

/*!
 *  @brief      Set the touch and release thresholds for all 13 channels on the
 *              device to the passed values. The threshold is defined as a
 *              deviation value from the baseline value, so it remains constant
 * even baseline value changes. Typically the touch threshold is a little bigger
 * than the release threshold to touch debounce and hysteresis. For typical
 * touch application, the value can be in range 0x05~0x30 for example. The
 * setting of the threshold is depended on the actual application. For the
 * operation details and how to set the threshold refer to application note
 * AN3892 and MPR121 design guidelines.
 *  @param      touch
 *              the touch threshold value from 0 to 255.
 *  @param      release
 *              the release threshold from 0 to 255.
 */
void Adafruit_MPR121::setThresholds(uint8_t touch, uint8_t release) {
  // set all thresholds (the same)
  for (uint8_t i = 0; i < 12; i++) {
    writeRegister(MPR121_TOUCHTH_0 + 2 * i, touch);
    writeRegister(MPR121_RELEASETH_0 + 2 * i, release);
  }
}

/*!
 *  @brief      Read the filtered data from channel t. The ADC raw data outputs
 *              run through 3 levels of digital filtering to filter out the high
 * frequency and low frequency noise encountered. For detailed information on
 * this filtering see page 6 of the device datasheet.
 *  @param      t
 *              the channel to read
 *  @returns    the filtered reading as a 10 bit unsigned value
 */
uint16_t Adafruit_MPR121::filteredData(uint8_t t) {
  if (t > 12)
    return 0;
  return readRegister16(MPR121_FILTDATA_0L + t * 2);
}

/*!
 *  @brief      Read the baseline value for the channel. The 3rd level filtered
 *              result is internally 10bit but only high 8 bits are readable
 * from registers 0x1E~0x2A as the baseline value output for each channel.
 *  @param      t
 *              the channel to read.
 *  @returns    the baseline data that was read
 */
uint16_t Adafruit_MPR121::baselineData(uint8_t t) {
  if (t > 12)
    return 0;
  uint16_t bl = readRegister8(MPR121_BASELINE_0 + t);
  return (bl << 2);
}

/**
 *  @brief      Read the touch status of all 13 channels as bit values in a 12
 * bit integer.
 *  @returns    a 12 bit integer with each bit corresponding to the touch status
 *              of a sensor. For example, if bit 0 is set then channel 0 of the
 * device is currently deemed to be touched.
 */
uint16_t Adafruit_MPR121::touched(void) {
  uint16_t t = readRegister16(MPR121_TOUCHSTATUS_L);
  return t & 0x0FFF;
}

/*!
 *  @brief      Read the contents of an 8 bit device register.
 *  @param      reg the register address to read from
 *  @returns    the 8 bit value that was read.
 */
uint8_t Adafruit_MPR121::readRegister8(uint8_t reg) {
  Adafruit_BusIO_Register thereg = Adafruit_BusIO_Register(i2c_dev, reg, 1);

  return (thereg.read());
}

/*!
 *  @brief      Read the contents of a 16 bit device register.
 *  @param      reg the register address to read from
 *  @returns    the 16 bit value that was read.
 */
uint16_t Adafruit_MPR121::readRegister16(uint8_t reg) {
  Adafruit_BusIO_Register thereg =
      Adafruit_BusIO_Register(i2c_dev, reg, 2, LSBFIRST);

  return (thereg.read());
}

/*!
    @brief  Writes 8-bits to the specified destination register
    @param  reg the register address to write to
    @param  value the value to write
*/
void Adafruit_MPR121::writeRegisterRaw(uint8_t reg, uint8_t value, bool _) {
    writeRegister(reg, value);
}
void Adafruit_MPR121::writeRegister(uint8_t reg, uint8_t value) {
  // MPR121 must be put in Stop Mode to write to most registers
  bool stop_required = true;

  // first get the current set value of the MPR121_ECR register
  Adafruit_BusIO_Register ecr_reg =
      Adafruit_BusIO_Register(i2c_dev, MPR121_ECR, 1);

  uint8_t ecr_backup = ecr_reg.read();
  if ((reg == MPR121_ECR) || ((0x73 <= reg) && (reg <= 0x7A))) {
    stop_required = false;
  }

  if (stop_required) {
    // clear this register to set stop mode
    ecr_reg.write(0x00);
  }

  Adafruit_BusIO_Register the_reg = Adafruit_BusIO_Register(i2c_dev, reg, 1);
  the_reg.write(value);

  if (stop_required) {
    // write back the previous set ECR settings
    ecr_reg.write(ecr_backup);
  }
}