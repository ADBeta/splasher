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

//TODO add protocol specific bytes to run commands

//List of supported interfaces, selected via cli.
enum class Interface { 
	SPI, DSPI, QSPI, I2C
};

//List of supported protocols, e.g. 24 Series (I2C), 25 Series (SPI), etc
enum class Protocol {
	W24, W25 //TODO name these
};


/*** Device Specific Struct ***************************************************/
//Each device has a struct with data about itself, eg the size (bytes),
//Interface, Protocol, Speed, offset
class Device {
	public:
	Interface iface;      //What interface is the device using?
	Protocol prot;        //What protocol is the device compat with?
	unsigned int KHz;     //How fast the device is in KHz (0 = max speed)
	unsigned long bytes;  //How many bytes does the device store
	unsigned long offset; //How many bytes to offset the read position
	
	//Print the device info to cout
	void print();
	
}; //class Device

/*** SPI Interface ************************************************************/
class ifaceSPI {
	public:
	//Constructor. Pass the pin numbers to the onject class
	ifaceSPI(int SCLK, int MISO, int MOSI, int HOLD, int CS, int WP);
	
	//Initialise the interface to basic non-selected idle state
	void init();
	
	//TODO make generic KHZ to us converter, then impliment it for this.
	//Maybe inherit?
	
	//Set the internal delay times for key aspects of the interface
	void setTiming(unsigned int KHz);
	
	//Start and stop the SPI bus
	void start();
	void stop();
	
	//Transmit a byte using the SPI interface
	void tx_byte(const char byte);
	//Receive a byte using the SPI interface
	char rx_byte(void);
	
	private:
	//hardware pins (Clock, M-Out, M-In, Chip Select, Write Protect)
	int io_SCLK, io_MISO, io_MOSI, io_HOLD, io_CS, io_WP;
	
	//Key timing delay values. Default 0, full speed
	unsigned int wait_clk = 0, wait_byte = 0, wait_bit = 0;
}; //class ifaceSPI


/*** DSPI Interface, deerived from SPI ****************************************/
class ifaceDSPI : public ifaceSPI {
	//Inherit the wait funct and timers, read & write in SPI mode the io_ pins,
	//stop, start and the constructor.
	//Redefine init, add new readDual and writeDual functions TODO Name 
	public:
	//Initialise the interface to basic idle state
	void init();
	
	//Initialise the interface into Dual SPI idle mode (More PI_INPUTs)
	void init_dual();
	
	void tx_byte_dual(const char byte);
	char rx_byte_dual();
	

}; //class ifaceDSPI


/*** General Splasher Namespace ***********************************************/
namespace Splasher {
	//Verbose flag, and how many bytes per CLI Notification. 0 = disabled
	extern unsigned long verboseBytes;

	
	
	//Dump n bytes from offset from the Flash to the File.
	void dumpFlashToFile(Device &dev, BinFile &file);




}; //namespace Splasher


#endif
