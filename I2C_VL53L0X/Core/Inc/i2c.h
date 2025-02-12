#ifndef I2C_H
#define I2C_H

void I2CPeripheral(void);
void I2CStart(void);
void I2CStop(void);
void I2CWrite(uint8_t txdata);
void I2CRead(uint8_t ack);
void I2CReadRegister(uint8_t reg, uint8_t value);
void I2CWriteRegister(uint8_t reg);

#endif