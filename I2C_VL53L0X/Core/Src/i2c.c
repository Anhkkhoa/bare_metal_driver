#include "stm32f4xx.h"

void I2CPeripheral(void) 
{
    //Enable APB1ENR for I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; 

    //Enable AHB1 for GPIOB, PB7 - I2C1_SDA, PB8 - I2C1_SCL
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; 

    //GPIOB Config
    GPIOB->MODER |= (2 << 14) | (2 << 16); //GPIO Mode - Alternate Function, 10 which clear previous bit, i can just asign 1 to necessary bit but not a good practice
    GPIOB->AFR[0] |= (1 << 30); //Alternate Function mode - AF4, rather AFRL or AFRH, use [0][1]
    GPIOB->AFR[1] |= (1 << 2); 
    GPIOB->OTYPER |= (1 << 7) | (1 << 8); //Output Type - Open-drain
    GPIOB->OSPEEDR |= (3 << 14) | (3 << 16); //Output Speed Register - High Speed
    

    //I2C Config 
    I2C1->CR1 = I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST; //Reset then turn off reset I2C software

    I2C1->CR1 &= ~I2C_CR1_PE; //Disable I2C for config

    //I2C1->CCR |= (0 << 15); //Standard Mode Spd (If clock run match)
    I2C1->CCR = 0x50; //T_High and T_Low are equal so be 5us, and then with the previous peripheral clock is 16MHz => T_PCLK1 is 62.5ns, 5us/62.5ns = 80
    I2C1->TRISE = 0x10;

    I2C1->CR1 |= I2C_CR1_PE; //Renable I2C

}

void I2CStart(void)
{
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB)); //Waiting for Start Condition Flag (SB)
}

void I2CStop(void)
{
    I2C1->CR1 |= I2C_CR1_STOP;
}

void I2CWrite(uint8_t txdata) //Sending Register and Address before dasta byte
{
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = txdata; //Sending Byte that we want to send
    while (!(I2C1->SR1 & I2C_SR1_BTF));
}

uint8_t I2CRead(uint8_t ack)
{
    if (ack)
    {
        I2C1->CR1 |= I2C_CR1_ACK; //Return ACK 
    } 
    else 
    {
        I2C1->CR1 &= ~I2C_CR1_ACK; //Return NACK
    }
    while (!(I2C1->SR1 & I2C_SR1_RXNE));
    return I2C1->DR;
}

uint8_t I2CWriteRegister(uint8_t reg, uint8_t value, uint8_t address)
{
    //Write to Register (required what register you sending too and the data)
    I2CStart();
    I2CWrite(address << 1); //Shift to the left with 0 bit for write
    I2CWrite(reg);
    I2CWrite(value);
    I2CStop();

}

void I2CReadRegister(uint8_t reg, uint8_t address) 
{
    uint8_t rxdata;
    I2CStart();
    I2CWrite(address << 1); //Write Mode
    I2CWrite(reg);
    I2CStart();
    I2CWrite((address<< 1)| 1);
    rxdata = I2CRead(1); //NACK sending data to signify completed sending the data
    I2CStop();
    return rxdata;
}