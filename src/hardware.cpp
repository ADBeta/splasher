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

#include <iostream>
#include <pigpio.h>

/*** Hardware SPI Interface ***************************************************/
hwSPI::hwSPI(int SCLK, int MOSI, int MISO, int CS, int WP) {
	//Set the object pins to the passed pins
	io_SCLK = SCLK;
	io_MOSI = MOSI;
	io_MISO = MISO;
	io_CS = CS;
	io_WP = WP;
	
	//Set the GPIO pinout to idle the interface
	init();
	
}

void hwSPI::init() {
	//Set the output pins
	gpioSetMode(io_SCLK, PI_OUTPUT);
	gpioSetMode(io_MOSI, PI_OUTPUT);
	gpioSetMode(io_CS, PI_OUTPUT);
	gpioSetMode(io_WP, PI_OUTPUT);
	
	//MISO is an input (Master In)
	gpioSetMode(io_MISO, PI_INPUT);
	
	//Set MOSI and SCLK low to idle
	gpioWrite(io_SCLK, 0);
	gpioWrite(io_MOSI, 0);
	//MISO LOW to pulldown
	gpioWrite(io_MISO, 0);
	
	stop(); //Pulls the CS pin high and waits
	//TODO Write Protect enable/disable function
}

void hwSPI::setTiming(unsigned int KHz) {
	wait_byte = 1;
	wait_bit = 0;
	
	wait_clk = 1;
}

void hwSPI::start() {
	//Pull CS low to initilaise the SPI interface
	gpioWrite(io_CS, 0);
	//wait for the byte delay if set, allows the chip to wake up
	if(wait_byte != 0) gpioDelay(wait_byte);	
}

void hwSPI::stop() {
	//Push CS high to stop the SPI comms interface
	gpioWrite(io_CS, 1);
	//wait for the byte delay if set, allows the chip to wake up
	if(wait_byte != 0) gpioDelay(wait_byte);	
}
	
void hwSPI::tx_byte(const char byte) {
	//TX Bits, data clocked in on the rising edge of CLK, MSBFirst
	for(signed char bitIndex = 7; bitIndex >= 0; bitIndex--) {
		//Write the current bit (input byte shifted x to the right, AND 0x01)
		gpioWrite(io_MOSI, (byte >> bitIndex) & 0x01);
		//Wait for the bit delay
		if(wait_bit != 0) gpioDelay(wait_bit);
		
		
		gpioWrite(io_SCLK, 1);                    //Set the clock pin HIGH
		if(wait_clk != 0) gpioDelay(wait_clk); //Delay if selected
		gpioWrite(io_SCLK, 0);                    //Set the clock pin LOW
		if(wait_clk != 0) gpioDelay(wait_clk); //Delay if selected
	}

	//Wait for the byte delay if selected
	if(wait_byte != 0) gpioDelay(wait_byte);	
}

char hwSPI::rx_byte(void) {
	char data = 0;
	
	//RX Bits into data, bit present on falling edge, MSBFirst
	for(unsigned char bitIndex = 0; bitIndex < 8; bitIndex++) {
		//shift the data byte 1 position to the left
		data = data << 1;
		
		bool cBit = gpioRead(io_MISO);
		
		//Set the LSB of data to read from gpio
		if(cBit != 0) data = data | 0x01;
		
		//Wait for the bit delay
		if(wait_bit != 0) gpioDelay(wait_bit);
		
		gpioWrite(io_SCLK, 1);                 //Set the clock pin HIGH
		if(wait_clk != 0) gpioDelay(wait_clk); //Delay if selected
		gpioWrite(io_SCLK, 0);                 //Set the clock pin LOW
		if(wait_clk != 0) gpioDelay(wait_clk); //Delay if selected
	}
	
	//Wait for the byte delay if selected
	if(wait_byte != 0) gpioDelay(wait_byte);	
	
	return data;
}

/*** Hardware I2C Interface ***************************************************/



/*** Splasher specific functions **********************************************/
namespace splasher {

void dumpFlashToFile(Device &dev, BinFile &file) {
	//TODO This is forced to use SPI for the moment. Fix this
	
	//create a new SPI interface (25 series) with forced pinout
	hwSPI dut(2, 3, 4, 14, 15);
	
	dut.setTiming(0);
	
	dut.start();
	
	//Read SPI command
	dut.tx_byte(0x03);
	
	//Address (forced to start from 0)
	dut.tx_byte(0x00);
	dut.tx_byte(0x00);
	dut.tx_byte(0x00);
	
	unsigned long KiBDone = 0;

	for(unsigned long cByte = 0; cByte < dev.bytes; cByte++) {
		//Push the read byte to the file
		file.pushByteToArray( dut.rx_byte() );

		//Update user every 1024 bytes
		if(cByte % 1024 == 0) {
			++KiBDone;
			std::cout << "Copied " << KiBDone << " KiB s\r";
		}
	}
	
	dut.stop();
	
}




}; //namespace splasher
