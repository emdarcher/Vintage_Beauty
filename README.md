# Vintage Beauty

Driving vintage HP HDSP-2000 Alphanumeric LED displays with a retro EPROM 8051 Microcontroller.

![Vintage Beauty Demo](demo/vintage_beauty_demo.jpg)

I made this project because I love Vintage Hardware and wanted to make something cool with the LED displays and an old 8051 that I had. End goal is to implement Character LCD emulation on the GPIO so it can act as a drop in HD44780/HD66702 based Character LCD replacement. 

I also wanted to take on the challenge of optimizing the code to run fast enough with the limited speed and peripherals of the 8051 Microcontroller. As a result, I got to learn the 8051's memory map with more detail in order take advantage of the bit-addressable RAM on the chip for a fast bit-banged SPI output, which was pretty fun. 

Pulished on Hackaday Projects as an entry into the Beautiful Hardware Contest [here](https://hackaday.io/project/167033-vintagebeauty).

## Demo:
Check out the Vintage Beauty Demo program in the [demo folder](demo/)

## Features:
- Can display the US ASCII character set, as well as Japanese Katakana and Hiragana using a custom 5x7 pixel font stored in the program ROM. (I love Japanese, so had to get it in there!)
- The ROM character sets are based off the those in the HD66702 LCD driver chip found in many Character LCDs. 
- Communicates with the displays through an optimized Bit-Banged SPI-like protocol utilizing the the 8051's bit-addressable memory for speed. Based on code found [here](https://www.maximintegrated.com/en/app-notes/index.mvp/id/3524) but modified for SDCC.
- Written in C and compiled using the Open Source [Small Device C Compiler (sdcc)](http://sdcc.sourceforge.net/)

## Work in Progress:
- Full HD44780/HD66702 LCD Driver emulation via GPIO ports.
- Serial UART control.

## References and Datasheets:
- [HD66702 LCD Driver Datasheet (For ROM Reference)](http://www.farnell.com/datasheets/31212.pdf)
- [HDSP-2000 Datasheet](http://www.hparchive.com/PARTS/HP-Displays-HDSP-2010.pdf)
- [Intel 87C51 Datasheet](https://www.keil.com/dd/docs/datashts/intel/80x1bh_87c51_ds.pdf)


