# splasher

splasher, Software Pi Flasher, is a Raspberry Pi program with the ability to 
flash, dump, clone and erase SPI/I2C/DSPI/QSPI flash chips, including the 24 and
25 series NAND and NOR Flash Chips.

WORK IN PROGRESS

Features of this program include the ability to specify protocol speeds, for when
certain chips use non-standard timings or have degraded to the point of needing 
more time to latch in the data.  
It also allows for the integration of multiple non-standard protocols, like DSPI
and QSPI, and more (see splasher -h for more information)

It has a very easy to use interface, and is written in C++ using pigpio to make
it fast and reliable.

(DSPI, QSPI and I2C are under construction. Only spi mode is working)

NOTES:
-s, --speed		Set the speed of the chip, in KHz. (-s max doesn't delay at all)

----
## Dependencies
This program requires that you have the pigpio library istalled.  
A tutorial of how to do this is [here](https://abyz.me.uk/rpi/pigpio/download.html)

## Compilation
#### Linux
After installing pigpio, the compilation instructions are as follows:  
```
git clone https://github.com/ADBeta/splasher.git
cd ./splasher-master
make
sudo make install
```

## Usage
TODO

## TODO
* SPI
* DSPI
* QSPI
* I2C
* pinout art
* size input - bytes and eg 16M (megabit)
* pass timing and speed to the spi funct

----
## Licence
This software is under the GPL 2.0 Licence, please see LICENCE for information  
<b>(c) 2023 ADBeta </b>
