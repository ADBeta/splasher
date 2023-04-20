/*******************************************************************************
* This file is part of splasher, see the public GitHub for more information:
* https://github.com/ADBeta/splasher
*
* Splasher is a Raspberry Pi Program to flash, dump, clone and empty a large
* selection of NAND or NOR Flash chips, including the 24 & 25 Series. 
* With the ability to support many protocols, including SPI, DSPI, QSPI, I2C,
* and custom non-standard protocols certain manufacturers use.
*

TODO unsigned long to uin32_t etc
#exception stuff

error return eqiv of std::string::npos



* (c) ADBeta
* v0.1.4
* 20 Apr 2023
*******************************************************************************/
#include <iostream>

#include <pigpio.h>

#include "helper.hpp"
#include "CLIah.hpp"
#include "filemanager.hpp"
#include "hardware.hpp"

/*** Pre-defined output messages **********************************************/
namespace message {
const char *copyright = "\nSplasher(c) 2023 ADBeta";

const char *shortHelp = "Usage: splasher [binary file] [options]\n\
use --help for full help information\n";

const char *longHelp =  "Usage: splasher [binary file] [options]\n\n\
By default .......TODO \n\n\
Options:\n\
-h, --help\t\tShow this help message\n\
-b, --bytes\t\tHow many bytes to read from the device. Allows K and M suffix\n\
-s, --speed\t\tThe speed of the interface in KHz, \"-s max\" delimits the bus\n\
-v, --verbose\t\tNumber of bytes to read/write before a heartbeat.\n\
\t\t\tAllows K and M suffix, \"-v 0\" disables verbosity (slightly faster)\n\
\nPlease see /docs/Usage.txt for more help";

//// warning messages ////
const char *gpioFailed = "pigpio Failed to initilaise the GPIO...\n";


const char *speedNotValid = "Speed (in KHz) input is invalid\n";
const char *speedTooHigh = "Speed (in KHz) is too high, Maximum is 1000KHz\n";
const char *speedDefault = "Speed not specified, using default of 100KHz\n";


const char *bytesNotValid = "Bytes input is invalid. example valid input: \
-b 1024  -b 2M\n";
const char *bytesTooLarge = "Bytes is too large, byte limit is 256MiB\n";
const char *bytesNotSpecified = "Bytes to read has not been specified\n";

} //namespace message

/*** Helper functions *********************************************************/
//converts a string into a KHz value - for user argument handling
//Negative values are coded errors (-1 not valid   -2 too large input)
int convertKHz(std::string speedString) {
	//First check if the input is "max"
	if(speedString.compare("max") == 0) {
		//Unlimit the KHz
		return 0;
	}
	
	//If the string contains non-numeral chars, error -1
	if(speedString.find_first_not_of("0123456789") != std::string::npos) {
		std::cerr << message::speedNotValid;
		return -1;
	}
	
	//Otherwise, convert it to an int
	int speedInt = std::stoi(speedString);
	
	//Detect if the input value is too high, if so return -2
	if(speedInt > 1000) {
		std::cerr << message::speedTooHigh;
		return -2;
	}
	
	//If no errors, return the speed int
	return speedInt;
}

unsigned long convertBytes(std::string byteString) {
	//Keep a multiplier, 1 by default for bytes, changes via 'K' or 'M'
	unsigned int multiplier = 1;
	
	/*** Detect Multiplier Char ***********************************************/
	//Find the first non-numeral character in the string
	size_t notNumeral = byteString.find_first_not_of("0123456789");
	//Get the index of the last char in the string
	size_t lastIndx = byteString.length() - 1;
	
	//If there is a non-numeral char in the string, check it
	if(notNumeral != std::string::npos) {
		//if that non-numeral char is NOT the last char, error
		if(notNumeral != lastIndx) {
			std::cerr << message::bytesNotValid;
			return 0;
		}
		
		//If the last char is either 'K' or 'M' adjust the multiplier
		char lastChar = byteString[lastIndx];
		if(lastChar == 'K') {
			multiplier = 1024; //1KiB
			
		} else if(lastChar == 'M') {
			multiplier = 1048576; //1MiB
		
		} else {
			//if the last char is NOT 'K' or 'M', Error and exit
			std::cerr << message::bytesNotValid;
			return 0;
		}
		
		//Remove the last char from the string, we are done with it
		byteString.pop_back();
	}
	
	/*** Convert and multiply the input number ********************************/
	//convert the passed string into an int and set device bytes
	unsigned long bytes = std::stoi(byteString) * multiplier;
	
	
	//If everything is good, return the value
	return bytes;

}

/******************************************************************************/

void printVal( std::string input ) {
	unsigned long output = stringToInt(input);
	
	if( output == error::bad_input) {
		std::cout << "error with conversion\n";
	} else {
		std::cout << output << std::endl;
	}	
}


/*** Main *********************************************************************/
int main(int argc, char *argv[]){
	/*** pigpio Setup *********************************************************/
	if(gpioInitialise() < 0) {
		std::cerr << message::gpioFailed;
		exit(EXIT_FAILURE);
	}

	std::cout << "EVALUATION DEMO ONLY" << std::endl;
	
	printVal("6969");
	printVal("420");
	printVal("1M");
	printVal("ff");
	
	

	/*** Define CLIah Arguments ***********************************************/
	//CLIah::Config::verbose = true; //Set verbosity when match is found
	CLIah::Config::stringsEnabled = true; //Set arbitrary strings allowed
	
	//Request help message
	CLIah::addNewArg(
		"Help",                 //Reference
		"--help",               //Primary match string
		CLIah::ArgType::flag,   //Argument type
		"-h"                    //Alias match string
	);

	//Speed (in KHz) of the device
	CLIah::addNewArg(
		"Speed",
		"--speed",
		CLIah::ArgType::subcommand,
		"-s"
	);

	//How many bytes to read from device
	CLIah::addNewArg(
		"Bytes",
		"--bytes",
		CLIah::ArgType::subcommand,
		"-b"
	);

	/*** Basic User CLI Arg Handling ******************************************/
	//Get CLIah to scan the CLI Args
	CLIah::analyseArgs(argc, argv);
	
	//If no arguments or strings were given, error with usage
	if( argc == 1 ) {
		std::cout << message::shortHelp << std::endl;
		exit(EXIT_FAILURE);
	}
	
	//If help was requested, print the long help message then exit.
	if( CLIah::isDetected("Help") ) {
		std::cout << message::longHelp << message::copyright << std::endl;
		exit(EXIT_SUCCESS);
	}
	
	/*** Filename handling ****************************************************/	
	//Get the user input filename, error if one is not provided
	if( CLIah::stringVector.size() == 0 ) {
		std::cerr << "Error: No filename provided" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	//Open the passed filename
	const char *filenameStr = CLIah::stringVector.at(0).string.c_str();

	//Open a bin file, force write for now.
	//BinFile object decelaration //TODO make read/write flag
	BinFile binFile(filenameStr, 'w');
	
	
	/*** Device creation ******************************************************/
	/*
	struct Device {
		IFACE interface;      //What interface is the device using?
		PROT protocol;        //What protocol is the device compat with?
		int KHz;     //How fast the device is in KHz (0 = max speed)
		unsigned long bytes;  //How many bytes does the device store
		unsigned long offset; //How many bytes to offset the read position
	}; //struct Device
	*/
	
	
	
	
	
	
	
	
	
	//Primary Device object created
	Device priDev;
	
	
	
	//TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
	/*** Get KHz speed of device **********************************************/
	if( CLIah::isDetected("Speed") ) {
		//Convert the KHz string into an int
		int KHzVal = convertKHz( CLIah::getSubstring("Speed") );
		
		//If the return int is one of the -x errors, exit
		if(KHzVal < 0) exit(EXIT_FAILURE);
		
		priDev.KHz = KHzVal;
		
	} else {
		//If no user input, warn that default is being used, and set it
		std::cout << message::speedDefault; //TODO Remove this message to funct
		priDev.KHz = 100;
	}
	
	/*** Get bytes to read from device ****************************************/
	//Get bytes from user if specified TODO add auto detect
	if( CLIah::isDetected("Bytes") ) {
		unsigned long byteVal = stringToInt( CLIah::getSubstring("Bytes") );
		
		//If byteVal is 0, or bad input exit
		if(byteVal == 0 || byteVal == error:bad_input) {
			std::cerr << message::bytesNotValid;
			
		}
		
		//Make sure the bytes selected are not too big (Limit to 256MB)
		if(bytes > 268435456) {
			std::cerr << message::bytesTooLarge;
			exit(EXIT_FAILURE);
		}
		
		//if no errors, set the device bytes value
		priDev.bytes = byteVal;
		
	} else {
		//TODO If no input bytes given, exit
		std::cerr << message::bytesNotSpecified;
		exit(EXIT_FAILURE);
	}
	
	
	
	
	
	Splasher::dumpFlashToFile(priDev, binFile);
	
	
	
	
	//Terminate the GPIO handler and exit
	gpioTerminate();
	return 0;
} 
