#include "stm32f4xx.h"

void BlinkyPeripheral(void) 
{
    //Peripheral Clock Enable Register AHB1ENR
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 

    //GPIO Mode Register (No need to clear for now, but should be in the future for register)
    GPIOA-> MODER |= (1 << 2); //Enable 01 of MODER1 for output mode

    /*
    PB7 - I2C1_SDA
    PB8 - I2C1_SCL
    */
}