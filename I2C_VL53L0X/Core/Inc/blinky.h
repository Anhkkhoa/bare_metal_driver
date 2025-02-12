#ifndef BLINKY_H
#define BLINKY_H

void BlinkyPeripheral(void);

/* Code for testing blinky
    GPIOA-> BSRR = (1 << 1);
    for (int x = 0; x < 0xFF; x++)
    {
      for (int y = 0; y < 0xFF; y++) {
      }
    }

    GPIOA-> BSRR = (1 << 17);
    for (int x = 0; x < 0xFF; x++)
    {
      for (int y = 0; y < 0xFF; y++) {
      }
    }
*/
#endif