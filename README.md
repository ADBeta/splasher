# splasher

splasher, Software Pi Flasher, is a Raspberry Pi program with the ability to 
flash, dump, clone and erase SPI/I2C/DSPI/QSPI flash chips, including the 24 and
25 series NAND and NOR Flash Chips without the need for any external devices
such as the CH341A etc. Written in C++ using pigpio, it is easy to use,
reliable and designed around compatibilty with any protocol or interface.

splasher allows interface speed selection down to 1KHz, for chips with slow
interfaces, or damaged chips, up to 1MHz limited interface for older but regular 
chips. It also allows "max" to be passed to the speed flag, in order to delimit
the interface (This varies wildly between 2MHz to 50MHz depending on model. be
aware of this before use)

WORK IN PROGRESS

## How To Use
When running splasher, you will need to use `sudo`, this is normal, and is a
side-effect of using pigpio.

To use, call splasher, pass a file to output to, and how many bytes to read (required)  
e.g `splasher output.bin -b 16M`  

The Default Pinout is as follows:
[https://github.com/ADBeta/splasher/blob/main/Pinout.png]

```
SCLK		2
MOSI		3
MISO		4

CS		14
WP		15
```

It sould be idential between Pi1, Pi2, Pi3, Pi4, PiZero etc.


The inbuilt help (`splasher --help`) will have more info, but for a basic 
rundown of the programs arguments:  
* -b or --bytes		This can be followed with 123456, 10K (10 KiB) or 16M (16 MiB)
* -s or --speed		This allows you to specify the speed of splasher in KHz 
(Not yet working). You can pass any number up to 1000, or max to delimit the bus
speed and read/write faster. Be aware this can change drastically per Pi model.

## Notes
I have not been able to validate the output of splasher yet, but i will soon

(DSPI, QSPI and I2C are under construction. Only spi mode is working)

----
## Dependencies
This program requires that you have the pigpio library istalled.  
A tutorial of how to do this is [here](https://abyz.me.uk/rpi/pigpio/download.html)

## Compilation
#### Linux
After installing pigpio, the compilation instructions are as follows:  
```
git clone https://github.com/ADBeta/splasher.git
unzip ./splasher-master.zip
cd ./splasher-master
make
sudo make install
```

## Usage
TODO

## TODO
* JEDEC ID
* auto bytes to read detect if not specified
* decide upon standard for default input
* DSPI
* QSPI
* I2C
* pinout art

----
## Licence
This software is under the GPL 2.0 Licence, please see LICENCE for information  
<b>(c) 2023 ADBeta </b>
