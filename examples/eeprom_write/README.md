EEPROM I2C access
========
This folder contains the source code for the MSP430 as well as for the PC counterpart written with Processing.

MSP430 part (eeprom.c):
----------------------------
256*8Bit will be received via UART and written to the EEPROM on the I2C interface on address 0x50.
The register on the EEPROM will be incremented from 0x00 to 0xFF.


PC part (eeprom_write.pde)
----------------------------
The application will send an array of 8Bit values via UART to the MSP430.
