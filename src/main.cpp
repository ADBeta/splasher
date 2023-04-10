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
* v0.0.1
* 09 Apr 2023
*******************************************************************************/
#include <iostream>

#include <pigpio.h>

#include "CLIah.hpp"
#include "filemanager.hpp"
#include "hardware.hpp"

/*** Pre-defined output messages **********************************************/
namespace message {
const char *copyright = "\nsplasher 2023 ADBeta(c)";

const char *shortHelp = "Usage: splasher [binary file] [options]\n\
use --help for full help information\n";

const char *longHelp =  "Usage: splasher [binary file] [options]\n\n\
By default .......TODO \n\
Options:\n\
-h, --help\t\tShow this help message\n\
-b, --bytes\t\tHow many bytes to read from the device. Allows K and M suffix \
to specify KiB or MiB\n\
-s, --speed\t\t(in KHz) The speed of the CLK pin cycle, All other timings \
derive from this. (\"-s max\" unlimits the bus speed)\n";


const char *speedNotValid = "Speed (in KHz) input is invalid\n";
const char *speedTooHigh = "Speed (in KHz) is too high, Maximum is 1000KHz\n";
const char *speedDefault = "Speed not specified, using default of 100KHz\n";

const char *bytesNotValid = "Bytes argument input is invalid. valid input e.g. \
-b 100    -b 100K    -b 2M\n";
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

} //TODO



//Global BinFile object. Gets created via read or write command from CLI
BinFile *binFile; //TODO move to main

/*** Main *********************************************************************/
int main(int argc, char *argv[]){
	/*** Generic pigpio stuff *************************************************/
	if(gpioInitialise() < 0) { //TODO message and return exit_failure
		std::cerr << "Error: Failed to initialise the GPIO" << std::endl;
		return 1;
	}

	std::cout << "EVALUATION DEMO ONLY" << std::endl;

	/*** Define CLIah Arguments ***********************************************/
	CLIah::Config::verbose = true; //Set verbosity when match is found
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

	/***TODO User Argument handling ***********************************************/
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
	const char *filename = CLIah::stringVector.at(0).string.c_str();

	//Open a bin file, force write for now.
	binFile = new BinFile( filename, 'w' );
	
	
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
	
	/*** Get KHz speed of device **********************************************/
	if( CLIah::isDetected("Speed") ) {
		//Convert the KHz string into an int
		int KHzVal = convertKHz( CLIah::getSubstring("Speed") );
		
		//If the return int is one of the -x errors, exit
		if(KHzVal < 0) exit(EXIT_FAILURE);
		
		
		std::cout << "Running at " << KHzVal << "KHz\n";
		priDev.KHz = KHzVal;
		
	} else {
		//If no user input, warn that default is being used, and set it
		std::cout << message::speedDefault; //TODO Remove this message
		priDev.KHz = 100;
	}
	
	
		
		
	
	/*** Get bytes to read from device ****************************************/
	//Get bytes from user if specified TODO add auto detect
	if( CLIah::isDetected("Bytes") ) {
		//Copy the argVector string to its own string to allow faster testing
		std::string byteString = CLIah::getSubstring("Bytes");
		
		//Keep a multiplier, 1 by default for bytes, changes via 'K' or 'M'
		unsigned int multiplier = 1;
		
		/*** Detect Multiplier Char *******************************************/
		//Find the first non-numeral character in the string
		size_t notNumeral = byteString.find_first_not_of("0123456789");
		//Get the index of the last char in the string
		size_t lastIndx = byteString.length() - 1;
		
		//If there is a non-numeral char in the string, check it
		if(notNumeral != std::string::npos) {
			//if that non-numeral char is NOT the last char, error
			if(notNumeral != lastIndx) {
				std::cerr << message::bytesNotValid;
				exit(EXIT_FAILURE);
			}
			
			//If the last char is either 'K' or 'M' adjust the multiplyer
			char lastChar = byteString[lastIndx];
			if(lastChar == 'K') {
				multiplier = 1024; //1KiB
				
			} else if(lastChar == 'M') {
				multiplier = 1048576; //1MiB
			
			} else {
				//if the last char is NOT 'K' or 'M', Error and exit
				std::cerr << message::bytesNotValid;
				exit(EXIT_FAILURE);
			}
			
			//Remove the last char from the string, we are done with it
			byteString.pop_back();
		}
		
		/*** Convert and multiply the input number ****************************/
		//convert the passed string into an int and set device bytes
		priDev.bytes = std::stoi(byteString) * multiplier;
		
		//TODO Move this to the read funct - print byte size
		std::cout << priDev.bytes << std::endl;
		
	} else {
		std::cerr << message::bytesNotSpecified;
		exit(EXIT_FAILURE);
	}
	
	
	
	
	
	
	
	splasher::dumpFlashToFile(priDev, *binFile);
	
	
	
	
	
	
	//Delete the BinFile object to force its desturctor to call. Closes file
	delete binFile;

	//Terminate the GPIO handler and exit
	//gpioTerminate();
	return 0;
} 
