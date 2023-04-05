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

#include "CLIah.hpp"
#include "filemanager.hpp"

//Global BinFile object. Gets created via read or write command from CLI
BinFile *binFile;

/*** Main *********************************************************************/
int main(int argc, char *argv[]){
	/*** Define CLIah Arguments ***********************************************/
	CLIah::Config::verbose = true; //Set verbosity when match is found
	CLIah::Config::stringsEnabled = true; //Set arbitrary strings allowed
	
	CLIah::addNewArg(
		"Help",                 //Reference
		"--help",               //Primary match string
		CLIah::ArgType::flag,   //Argument type
		"-h"                    //Alias match string
	);

	/** User Argument handling ************************************************/
	//Get CLIah to scan the CLI Args
	CLIah::analyseArgs(argc, argv);
	
	//If help was requested, print the long help message then exit.
	if( CLIah::isDetected("Help") ) {
		//std::cout << message::longHelp << message::copyright << std::endl;
		return 0;
	}

	binFile = new BinFile("./test.bin", 'w');
	
	
	char x = 0;
	
	for (int y = 0; y < 1005; ++y) {
		binFile->pushByteToArray(x);
	
		++x;
	}
	
	
	//tfile.flushArrayToFile();
	


	return 0;
} 
