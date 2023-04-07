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
	//TX Bits, data clocked in on the rising edge of CLK, MSBFirst
	for(char bitIndex = 7; bitIndex >= 0; --bitIndex) {
		//Write the current bit (input byte shifted x to the right, AND 0x01)
		gpioWrite(MOSI, (byte >> bitIndex) & 0x01);
		//Wait for the bit delay
		if(wait_bit != 0) gpioDelay(wait_bit);
		
		
		gpioWrite(SCLK, 1);                    //Set the clock pin HIGH
		if(wait_clk != 0) gpioDelay(wait_clk); //Delay if selected
		gpioWrite(SCLK, 0);                    //Set the clock pin LOW
		if(wait_clk != 0) gpioDelay(wait_clk); //Delay if selected
	}
	
	//Wait for the byte delay if selected
	if(wait_byte != 0) gpioDelay(wait_byte);	
}

char hwSPI::rx_byte(void) {
	char data = 0;
	
	//RX Bits into data, bit present on falling edge, MSBFirst
	for(char bitIndex = 0; bitIndex < 8; ++bitindex) {
		//shift the data byte 1 position to the left
		data = data << 1;
		
		//Set the LSB of data to read from gpio
		data = data | gpioRead(MISO);	
		//Wait for the bit delay
		if(wait_bit != 0) gpioDelay(wait_bit);
		
		
		gpioWrite(SCLK, 1);                    //Set the clock pin HIGH
		if(wait_clk != 0) gpioDelay(wait_clk); //Delay if selected
		gpioWrite(SCLK, 0);                    //Set the clock pin LOW
		if(wait_clk != 0) gpioDelay(wait_clk); //Delay if selected
	}
	
	//Wait for the byte delay if selected
	if(wait_byte != 0) gpioDelay(wait_byte);	
	
	return data;
}

/*** Hardware I2C Interface ***************************************************/



/*** Splasher generic functions ***********************************************/

