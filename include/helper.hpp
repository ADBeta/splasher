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
#include <string>
#include <exception>

#ifndef SPLASHER_HELPER_H
#define SPLASHER_HELPER_H


/*** Exception declarations ***************************************************/
class SplasherException : public std::exception {
	public:
	SplasherException(const char* msg) {
		errMsg = msg;
	}

	private:
	const char *errMsg;
	
	const char *what () const throw() {
		return errMsg;
	}
};

extern SplasherException invalid_input;

/*** Helper Functions *********************************************************/
//Converts a string to an int, with 'K' or 'M' support (KiB or Mib).
//Throws exception (invalid_input) if input is not valid
unsigned long stringToInt( std::string );



#endif
