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
#include "helper.hpp"
#include "CLIah.hpp"

#include <string>
#include <cstdint>

/*** definitions and macros ***************************************************/
namespace error {
	//uint32_t max value to return on input error in functions that return uint32_t
	uint32_t bad_input = 0 - 1;
};

/*** CLIah setup (frees space in main) ****************************************/
void initCLIah() {
	//CLIah::Config::verbose = true; //Set verbosity when match is found
	CLIah::Config::stringsEnabled = true; //Set arbitrary strings allowed
	
	//Request help message
	CLIah::addNewArg(
		"Help",                 //Reference
		"--help",               //Primary match string
		CLIah::ArgType::flag,   //Argument type
		"-h"                    //Alias match string
	);
	
	//How many bytes to read from device
	CLIah::addNewArg(
		"Bytes",
		"--bytes",
		CLIah::ArgType::subcommand,
		"-b"
	);

	//Speed (in KHz) of the device
	CLIah::addNewArg(
		"Speed",
		"--speed",
		CLIah::ArgType::subcommand,
		"-s"
	);
	
	//How many bytes to read or write before giving a heartbeat
	CLIah::addNewArg(
		"Verbose",
		"--verbose",
		CLIah::ArgType::subcommand,
		"-v"
	);
	
	//Offset byte val to start reading or writing from
	CLIah::addNewArg(
		"Offset",
		"--offset",
		CLIah::ArgType::subcommand,
		"-o"
	);
	
	//Select the interface (SPI, I2C, DSPI etc)
	CLIah::addNewArg(
		"Interface",
		"--interface",
		CLIah::ArgType::subcommand,
		"-i"
	);
	
	//Select which protocol to use (W25, W24, uBUS etc)
	CLIah::addNewArg(
		"Protocol",
		"--protocol",
		CLIah::ArgType::subcommand,
		"-p"
	);
	
	
	//-m --mode (?) TODO
}

/*** Helper Functions *********************************************************/
uint32_t byteStringToInt(std::string byteString) {
	//Multiplier defaults to 1, but gets set when M or K is detected correctly
	unsigned int multiplier = 1;
	
	//Find the first non-numeral char, for detection later.
	size_t notNumeral = byteString.find_first_not_of("0123456789");
	
	//// If the string contains a non-numeral char /////////////////////////////
	if(notNumeral != std::string::npos) {
		//Find the last char ex in the string
		size_t lastCharIndx = byteString.length() - 1;
		
		//if that non-numeral char is NOT the last char, throw custom exception
		if(notNumeral != lastCharIndx) {
			return error::bad_input;
		}
		
		//If the last char is either 'K' or 'M' adjust the multiplier
		char lastChar = byteString[lastCharIndx];
		if(lastChar == 'K') {
			multiplier = 1024; //1KiB
			
		} else if(lastChar == 'M') {
			multiplier = 1048576; //1MiB
		
		//if the last char is NOT 'K' or 'M', throw exception
		} else {
			return error::bad_input;
		}
			
		//Remove the last char from the string, helps stoi
		byteString.pop_back();
	}
	
	//// If the string does not have a non-numeral in it, or after conversion to
	//// value and multiplier  /////////////////////////////////////////////////
	
	//convert the passed string into an int and multiply with the multiplier
	unsigned long bytes = std::stoi(byteString) * multiplier;
	
	//Return the converted value
	return bytes;
}

uint32_t intStringToInt(std::string intString) {
	//If the string contains non-numeral chars, error -1
	if(intString.find_first_not_of("0123456789") != std::string::npos) {
		return error::bad_input;
	}
	
	//Otherwise, convert it to an int
	return std::stoi(intString);
}
