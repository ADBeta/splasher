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

/*** Device Class Functions ***************************************************/
void Device::print() {
	//TODO device string selectable from array or group
	std::cout << "Reading -an SPI- deivce: " << this->bytes << " bytes at " 
	          << this->KHz << "KHz" << std::endl;
	          
	//TODO offset print
}


/*** SPI Interface (((((((((***************************************************/
ifaceSPI::ifaceSPI(int SCLK, int MISO, int MOSI, int HOLD, int CS, int WP) {
	//Set the object pins to the passed pins
	io_SCLK = SCLK;
	io_MISO = MISO;
	io_MOSI = MOSI;
	io_HOLD = HOLD;
	io_CS = CS;
	io_WP = WP;
	
	//Set the GPIO pinout to idle the interface
	init();
}

void ifaceSPI::init() {
	//Set the output pins
	gpioSetMode(io_SCLK, PI_OUTPUT);
	gpioSetMode(io_MOSI, PI_OUTPUT);
	gpioSetMode(io_HOLD, PI_OUTPUT);
	gpioSetMode(io_CS, PI_OUTPUT);
	gpioSetMode(io_WP, PI_OUTPUT);
	
	//MISO is an input (Master In)
	gpioSetMode(io_MISO, PI_INPUT);
	
	//Set pins to idle state
	gpioWrite(io_SCLK, 0);
	gpioWrite(io_MOSI, 0);
	gpioWrite(io_MISO, 0);
	
	//Pull CS high so it is idle
	gpioWrite(io_CS, 1);
	
	//Pull HOLD high, so chips is active, not reset
	gpioWrite(io_HOLD, 1);
	
	//leave write protect alone until read/write functions
}

void ifaceSPI::setTiming(unsigned int KHz) { //TODO
	//if the input is 0, max was selected, set no delay
	if(KHz == 0) {
		wait_byte = 0;
		wait_bit = 0;
		wait_clk = 0;
	}
	
	//otherwise TODO
	wait_byte = 1;
	wait_bit = 1;
	wait_clk = 1;
}

void ifaceSPI::start() {
	//Pull CS low to initilaise the SPI interface
	gpioWrite(io_CS, 0);
	//wait for the byte delay if set, allows the chip to wake up
	if(wait_byte != 0) gpioDelay(wait_byte);	
}

void ifaceSPI::stop() {
	//Push CS high to stop the SPI comms interface
	gpioWrite(io_CS, 1);
	//wait for the byte delay if set, allows the chip to wake up
	if(wait_byte != 0) gpioDelay(wait_byte);
}
	
void ifaceSPI::tx_byte(const char byte) {
	//TX Bits, data clocked in on the rising edge of CLK, MSBFirst
	for(signed char bitIndex = 7; bitIndex >= 0; bitIndex--) {
		//Write the current bit (input byte shifted x to the right, AND 0x01)
		gpioWrite(io_MOSI, (byte >> bitIndex) & 0x01);
		//Wait for the bit delay
		if(wait_bit != 0) gpioDelay(wait_bit);
		
		//Cycle the SCLK pin, and delay if selected
		gpioWrite(io_SCLK, 1);
		if(wait_clk != 0) gpioDelay(wait_clk);
		gpioWrite(io_SCLK, 0);
		if(wait_clk != 0) gpioDelay(wait_clk);
	}

	//Wait for the byte delay if selected
	if(wait_byte != 0) gpioDelay(wait_byte);	
}

char ifaceSPI::rx_byte(void) {
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
		
		//Cycle the SCLK pin, and delay if selected
		gpioWrite(io_SCLK, 1);
		if(wait_clk != 0) gpioDelay(wait_clk);
		gpioWrite(io_SCLK, 0);
		if(wait_clk != 0) gpioDelay(wait_clk);
	}
	
	//Wait for the byte delay if selected
	if(wait_byte != 0) gpioDelay(wait_byte);	
	
	return data;
}

/*** Hardware I2C Interface ***************************************************/



//TODO move this?

/*** Splasher namespace variables (extern declares) ***************************/
namespace Splasher {
	//Verbose flag, and how many bytes per CLI Notification. 0 = disabled
	unsigned long verboseBytes;
	
}; //namespace Splasher

void Splasher::dumpFlashToFile(Device &dev, BinFile &file) {
	//TODO This is forced to use SPI for the moment. Fix this
	
	//Print info for the user
	dev.print();
	
	//create a new SPI interface (25 series) with forced pinout
	ifaceSPI dut(2, 3, 4, 17, 27, 22);
	
	dut.setTiming(0);
	
	dut.start();
	
	//SPI 25 Series "Read" command
	dut.tx_byte(0x03);
	
	
	//TODO Array, make tx [array],size          maybe rx [array], size
	//Address (forced to start from 0) for now TODO
	dut.tx_byte(0x00);
	dut.tx_byte(0x00);
	dut.tx_byte(0x00);
	
	//Init some values for the loop (Temporary)
	unsigned long KiBDone = 0;
	unsigned long maxByte = dev.bytes + 1;

	unsigned int follower = 0;

	for(unsigned long cByte = 1; cByte < maxByte; cByte++) {
		//Push the read byte to the file
		file.pushByteToArray( dut.rx_byte() );
		
		//Update user every 1024 bytes
		if(++follower == 102400) {
			++KiBDone;
			
			follower = 0;
			//Go to beginning of line (Requires \n) and print status
			std::cout << "\rDumped " << KiBDone * 100 << "KiB" << std::flush;
		}
	}
	
	dut.stop();
	
	//Print finished message
	std::cout << "\n\nFinished dumping to " << file.getFilename() << std::endl;
	
}

