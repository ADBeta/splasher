/*******************************************************************************
* This file is part of splasher, see the public GitHub for more information:
* https://github.com/ADBeta/splasher
*
* Splasher is a Raspberry Pi Program to flash, dump, clone and empty a large
* selection of NAND or NOR Flash chips, including the 24 & 25 Series. 
* With the ability to support many protocols, including SPI, DSPI, QSPI, I2C,
* and custom non-standard protocols certain manufacturers use.
*
* (c) ADBeta
*******************************************************************************/

#include "hardware.hpp"

/*** Hardware SPI Interface ***************************************************/
hwSPI::hwSPI(int SCLK, int MOSI, int MISO, int CS, int WP) {
	
	
	
	
}

void hwSPI::setTiming(unsigned int KHz) {
	
	
	
	
}
	
void hwSPI::tx_byte(const char byte) {
	
	
	
}

char hwSPI::rx_byte(void) {
	char data = 0;
	
	return data;
}

