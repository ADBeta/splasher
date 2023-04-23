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
* v0.3.0
* 23 Apr 2023
*******************************************************************************/
#include <iostream>

#include <pigpio.h>

#include "helper.hpp"
#include "CLIah.hpp"
#include "filemanager.hpp"
#include "hardware.hpp"

/*** Const values and macros **************************************************/
#define GPIO_MAX_SPEED 1000

/*** Pre-defined output messages **********************************************/
namespace message {
const char *copyright = "\nSplasher(c) 2023 ADBeta\n";

const char *shortHelp = "Usage: splasher [binary file] [options]\n\
use --help for full help information\n\n";

const char *longHelp =  "Usage: splasher [binary file] [options]\n\n\
By default splasher REQUIRES a binary file as its first argument and at least\n\
the --bytes argument set, everything else is optional.\n\
If no further arguments are passed, splasher will assume a speed of 100KHz,\n\
an SPI Interface - W25 Series Flash Protocol and a read operation to the file\n\
\n\
Options:\n\
-h, --help\t\tShow this help message\n\
-b, --bytes\t\tHow many bytes to read from the device. Allows K and M suffix\n\
-o, --offset\t\tOffset byte to start from. Allows K and M Suffix like -b\n\
-s, --speed\t\tThe speed of the interface in KHz, \"-s 0\" is fastest.\n\
-v, --verbose\t\tNumber of bytes to read/write before a heartbeat.\n\
\t\t\tAllows K and M suffix, \"-v 0\" disables verbosity (slightly faster)\n\
\nPlease see /docs/Usage.txt for more help";

//// warning messages ////
const char *warn = "Warning: ";
const char *speedDefault = "Speed not specified, using default of 100KHz\n";


//// error messages ////
const char *error = "Error: ";
const char *gpioFailed = "pigpio Failed to initilaise the GPIO...\n";

const char *bytesRequired = "The Bytes argument is required \n";

const char *noFilename = "Filename to read or write was not given.\n";

const char *speedNotValid = "Speed (in KHz) input is invalid\n";
const char *speedTooHigh = "Speed (in KHz) is too high, Maximum is 1000KHz\n";

const char *bytesNotValid = "Bytes input is invalid. example valid input: \
-b 1024  -b 2M\n";
const char *bytesTooLarge = "Bytes is too large, byte limit is 256MiB\n";

} //namespace message

/*** Main *********************************************************************/
int main(int argc, char *argv[]){
	/*** pigpio Setup *********************************************************/
	if(gpioInitialise() < 0) {
		std::cerr << message::error << message::gpioFailed;
		exit(EXIT_FAILURE);
	}

	std::cout << "EVALUATION DEMO ONLY" << std::endl;
	
	//Initialise CLIah
	initCLIah();
	
	//Get CLIah to scan the CLI Args
	CLIah::analyseArgs(argc, argv);
	
	//If no arguments or strings were given, error with usage
	if( argc == 1 ) {
		std::cout << message::shortHelp;
		exit(EXIT_FAILURE);
	}
	
	//If help was requested, print the long help message then exit.
	if( CLIah::isDetected("Help") ) {
		std::cout << message::longHelp << message::copyright;
		exit(EXIT_SUCCESS);
	}
	
	/*** Required input detection *********************************************/
	if( CLIah::isDetected("Bytes") == false ) {
		std::cerr << message::error << message::bytesRequired;
		exit(EXIT_FAILURE);
	}
	//TODO
	
	/*** Filename handling ****************************************************/	
	//Get the user input filename, error if one is not provided
	if( CLIah::stringVector.size() == 0 ) {
		std::cerr << message::error << message::noFilename;
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
	Device priDev; //TODO rename

	/*** Get bytes to read from device ****************************************/
	//Get bytes from user. Must be specified for program to run, so will exist
	unsigned long byteVal = byteStringToInt( CLIah::getSubstring("Bytes") );
	
	//If byteVal is 0, or bad input exit
	if(byteVal == 0 || byteVal == error::bad_input) {
		std::cerr << message::error << message::bytesNotValid;
		exit(EXIT_FAILURE);
	}
	
	//Make sure the bytes selected are not too big (Limit to 256MB)
	if(byteVal > 268435456) {
		std::cerr << message::error << message::bytesTooLarge;
		exit(EXIT_FAILURE);
	}
	
	//if no errors, set the device bytes value
	priDev.bytes = byteVal;
	

	
	/*** Get KHz speed of device **********************************************/
	if( CLIah::isDetected("Speed") ) {
		//Convert the speed string into an int
		unsigned int speedVal = intStringToInt( CLIah::getSubstring("Speed") );
		
		//If speedVal returned an error, exit
		if(speedVal == error::bad_input) {
			std::cerr << message::error << message::speedNotValid;
			exit(EXIT_FAILURE);
		}
		
		//If the speedVal is too high, print error, but continue //TODO exits atm
		if(speedVal > GPIO_MAX_SPEED) {
			std::cerr << message::error << message::speedTooHigh;
			exit(EXIT_FAILURE); //TODO
			speedVal = GPIO_MAX_SPEED;
		}
		
		//If everything is okay, set the device speed
		priDev.KHz = speedVal;
		
	} else {
		//If no user input, warn that default is being used, and set it
		std::cout << message::warn << message::speedDefault; //TODO Remove this message to funct
		priDev.KHz = 100;
	}
	
	
	
	
	
	
	
	Splasher::dumpFlashToFile(priDev, binFile);
	
	
	
	
	//Terminate the GPIO handler and exit
	gpioTerminate();
	return 0;
}
