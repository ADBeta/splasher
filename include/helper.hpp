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
#include <cstdint>

#ifndef SPLASHER_HELPER_H
#define SPLASHER_HELPER_H


/*** definitions and macros ***************************************************/
namespace error {
	//Keep a uint32_t max value to return on input error in functions
	extern uint32_t bad_input;
}; //namespace error

/*** Helper Functions *********************************************************/
//Converts a string to an int, with 'K' or 'M' support (KiB or Mib).
//Returns uint32_t max if invalid input. like std::string::npos
uint32_t stringToInt( std::string );



#endif
