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

#include "filemanager.hpp"

#ifndef HARDWARE_H
#define HARDWARE_H

//TODO Namespace for common limits (bits/bytes, speed etc


//TODO Pass binary file class via pointer 

//TODO add protocol specific bytes to run commands

//List of supported interfaces, selected via cli.
enum class IFACE { 
	SPI, DSPI, QSPI, I2C
};

//List of supported protocols, e.g. 24 Series (I2C), 25 Series (SPI), etc
enum class PROT {
	S24, S25
};


/*** Device Specific Struct ***************************************************/
//Each device has a struct with data about itself, eg the size (bytes),
//Interface, Protocol, Speed, offset
struct Device {
	IFACE interface;      //What interface is the device using?
	PROT protocol;        //What protocol is the device compat with?
	unsigned int KHz;     //How fast the device is in KHz (0 = max speed)
	unsigned long bytes;  //How many bytes does the device store
	unsigned long offset; //How many bytes to offset the read position
}; //struct Device

/*** Hardware I2C Interface ***************************************************/
class hwI2C {


}; //class hwI2C

/*** Hardware SPI Interface ***************************************************/
class hwSPI {
	public:
	//Constructor. Pass the pin numbers to the onject class
	hwSPI(int SCLK, int MOSI, int MISO, int CS, int WP);
	
	//Initialise the interface to basic non-selected idle state
	void init();
	
	//Set the internal delay times for key aspects of the interface
	void setTiming(unsigned int KHz);
	
	//Start and stop the SPI comms
	void start();
	void stop();
	
	//Transmit a byte using the SPI interface
	void tx_byte(const char byte);
	//Receive a byte using the SPI interface
	char rx_byte(void);
	
	private:
	//hardware pins (Clock, M-Out, M-In, Chip Select, Write Protect)
	int io_SCLK, io_MOSI, io_MISO, io_CS, io_WP;
	
	//Key timing delay values
	unsigned int wait_clk, wait_byte, wait_bit;


}; //class hwSPI

/*** Hardware Dual SPI Interface **********************************************/
class hwDSPI {



}; //class heDSPI


/*** Splasher hardware namespace **********************************************/
namespace splasher {

//Dump bytes from the Flash to the File. Pass file and device.
//This function honours offset, KHz, size etc
void dumpFlashToFile(Device &dev, BinFile &file);




}; //namespace splasher




#endif
