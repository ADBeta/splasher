/*******************************************************************************
* This file is part of splasher, see the public GitHub for more information:
* https://github.com/ADBeta/splasher
*
* Splasher is a Raspberry Pi Program to flash, dump, clone and empty a large
* selection of NAND or NOR Flash chips, including the 24 & 25 Series. 
* With the ability to support many protocols, including SPI, DSPI, QSPI, I2C,
* and custom non-standard protocols certain manufacturers use.
*
* Notes: 
* Standard 24x series Flash chips all share 0x03, followed by 24bits
* for the read instruction and start address (offset)
*
* Dual Read seems to be standard DSPI, when running command 0x3B, followed 
* by 24bit address
*
*
*
*
*
*
*
* (c) ADBeta
*******************************************************************************/
#ifndef HARDWARE_H
#define HARDWARE_H

//TODO Namespace for common limits (bits/bytes, speed etc


//TODO Pass binary file class via pointer 



//List of supported interfaces, selected via cli.
enum class IFACE { 
	SPI, DSPI, QSPI, I2C
};

//List of supported protocols, e.g. 24 Series (I2C), 25 Series (SPI), Custom etc
enum class PROT {
	S24, S25
};

class SplasherSPI {
	public:
	//Constructor. Pass the pin numbers to the onject class
	SplasherSPI(int SCLK, int MOSI, int MISO, int CS, int WP);

	

	private:
	//hardware pins (Clock, M-Out, M-In, Chip Select, Write Protect)
	int SCLK, MOSI, MISO, CS, WP;


}; //SplasherSPI




#endif
