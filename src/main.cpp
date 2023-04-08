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
* 03 Apr 2023
*******************************************************************************/
#include <iostream>

#include <pigpio.h>

#include "CLIah.hpp"
#include "filemanager.hpp"
#include "hardware.hpp"

//Global BinFile object. Gets created via read or write command from CLI
BinFile *binFile;

/*** Pre-defined output messages **********************************************/
namespace message {
const char *copyright = "\nsplasher 2023 ADBeta(c)";
const char *malformed = "TODO......... must be the first argument\n";

const char *shortHelp = "Usage: splasher [binary file] [options]\n\
use --help for full help information\n";

const char *longHelp = "Usage: splasher [binary file] [options]\n\n\
By default .......todo \n\
Options:\n\
-h, --help\t\tShow this help message\n\
-b, --bytes\t\tHow many bytes to read from the device\n";
} //namespace message

/*** Main *********************************************************************/
int main(int argc, char *argv[]){
	/*** Generic pigpio stuff *************************************************/
	if(gpioInitialise() < 0) {
		std::cerr << "Error: Failed to initialise the GPIO" << std::endl;
		return 1;
	}

	std::cout << "EVALUATION DEMO ONLY" << std::endl;

	/*** Define CLIah Arguments ***********************************************/
	CLIah::Config::verbose = true; //Set verbosity when match is found
	CLIah::Config::stringsEnabled = true; //Set arbitrary strings allowed
	
	CLIah::addNewArg(
		"Help",                 //Reference
		"--help",               //Primary match string
		CLIah::ArgType::flag,   //Argument type
		"-h"                    //Alias match string
	);

	CLIah::addNewArg(
		"Bytes",                      //Reference
		"--bytes",                    //Primary match string
		CLIah::ArgType::subcommand,   //Argument type
		"-b"                          //Alias match string
	);

	/*** User Argument handling ***********************************************/
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
		unsigned int KHz;     //How fast the device is in KHz (0 = max speed)
		unsigned long bytes;  //How many bytes does the device store
		unsigned long offset; //How many bytes to offset the read position
	}; //struct Device
	*/
	
	//Primary Device (will add secDev
	Device priDev;
	
	//Find the --bytes or -b flag, MUST BE DETECTED (TODO)
	if( CLIah::isDetected("Bytes") ) {
		priDev.bytes = std::stoi(CLIah::getSubstring("Bytes"));
	} else {
		std::cerr << "Error: No byte size provided" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	
	splasher::dumpFlashToFile(priDev, *binFile);
	
	//Delete the BinFile object to force its desturctor to call. Closes file
	delete binFile;

	//Terminate the GPIO handler and exit
	//gpioTerminate();
	return 0;
} 
