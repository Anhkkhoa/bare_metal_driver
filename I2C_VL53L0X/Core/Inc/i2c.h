#ifndef I2C_H
#define I2C_H

void I2CPeripheral(void);
void I2CStart(void);
void I2CStop(void);
void I2CWrite(uint8_t txdata);
uint8_t I2CRead(uint8_t ack);
void I2CWriteRegister(uint8_t reg, uint8_t value, uint8_t address);
uint8_t I2CReadRegister(uint8_t reg, uint8_t address);

#endif