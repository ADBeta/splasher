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
#include <iostream>
#include <string>
#include <cstring>

#include "filemanager.hpp"

//Constructor: Input Filename. Opens the file ready for read or write
BinFile::BinFile(const char *inptFN, const char mode) {
	//Create a char array at filename the size of the input string.
	this->filename = new char[ strlen(inptFN) + 1 ];
	//Copy the input string to the new char array at filename
	strcpy(this->filename, inptFN);
	
	//if mode is 'r' (read) open file in read mode
	if(mode == 'r') {
		file.open(filename, std::ios::in | std::ios::binary);
	} else if(mode == 'w') {
	//if mode is 'w' (write) open file in output mode, truncate
		file.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
	} else {
	//If the mode is unrecognised, error
		std::cerr << "Error: BinFile mode is not recognised\n";
		exit(EXIT_FAILURE);
	}
	
	//Make sure file is open and exists
	if(file.is_open() == 0) {
		std::cerr << "Error: Cannot open or create file: " << filename << "\n";
		exit(EXIT_FAILURE);
	}
	
	//Create the RAM Byte array, of size defined in header
	byteArrayPtr = new char[MAX_RAM_BYTES];
}

//Destructor deletes the Byte Array from RAM and closes the file
BinFile::~BinFile() {
	//Safety flush the byteArray
	if(byteArrayPos != 0) flushArrayToFile();

	//Close the binary file
	this->file.close();

	//Delete the RAM array
	delete byteArrayPtr;
}

void BinFile::pushByteToArray(const char byte) {
	//Check if the current byte pos equals the MAX_RAM_BYTES
	if(byteArrayPos == MAX_RAM_BYTES) {
		//Flush the array to the file
		flushArrayToFile();
	}
	
	//Set the byte in the array at current pos to the passed byte value
	byteArrayPtr[byteArrayPos] = byte;
	
	//Incriment the byte pos
	++byteArrayPos;
}

//Flushes the byteArray to the file, returns exit status.
//0 = success		1 = failure
int BinFile::flushArrayToFile() {
	//Check the file is open
	
	//Check the array is OK
	
	//Dump the array, of arrayPos bytes
	this->file.write(byteArrayPtr, byteArrayPos);
	
	//Reset the byte array pos
	byteArrayPos = 0;
	
	//Successful operation
	return 0;
}
