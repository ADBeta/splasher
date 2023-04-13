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
#include <fstream>

#ifndef FILEMAN_H
#define FILEMAN_H

//Object containing the filename, data pointers, functions etc for binary files
class BinFile {
	public:
	//Constructor sets the filename string, creates the byteArray heap alloc
	//and opens the input file as input or output
	//mode:   'r' read file          'w' write file
	BinFile(const char *inptFN, const char mode);
	
	//Destructor flushes the byteArray to the file, deletes the byteArray and
	//closes the file
	~BinFile();
	
	/*** File Metadata Functions **********************************************/
	std::string getFilename();
	
	/*** File Reading Functions ***********************************************/
	//Push a byte to the byteArray. This will automatically flush the array to 
	//the binary file when the array becomes full.
	void pushByteToArray(const char byte);
	
	//Flushes the byteArray to the file, returns exit status.
	//0 = success		1 = failure
	int flushArrayToFile();
	
	
	/*** File Writing Functions ***********************************************/
	


	private:
	std::fstream file; //File object
	char *filename; //Filename string
	
	//How many bytes can be be in the byteArray before it flushes
	#define MAX_RAM_BYTES 10485760
	//Pointer to heap array where data will be stored before pushing to a file.
	char *byteArrayPtr;
	//Current byte address to read or write in the 
	unsigned int byteArrayPos = 0;
}; //class BinFile


#endif
