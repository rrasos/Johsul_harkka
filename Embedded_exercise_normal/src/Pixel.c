/*
 * Pixel.c
 *
 *  Created on: -----
 *      Author: -----
 */

#include "Pixel.h"

//define control and chanel signals
#define CONTROL_SIGNAL   *(uint32_t*) 0x41220008
#define CHANNEL_SIGNAL  *(uint32_t*) 0x41220000


//Table for pixel dots.
//				 dots[X][Y][COLOR]
volatile uint8_t dots[8][8][3]={0};


// Here the setup operations for the LED matrix will be performed
void setup(){

	//1. Initialize control and chanel signals to 0
	CONTROL_SIGNAL = 0;
	CHANNEL_SIGNAL = 0;

	//reseting screen at start is a MUST to operation (Set RST-pin to 1).
	CONTROL_SIGNAL |= (1 << 0); // bit 0 to 1
	usleep(500); // wait 500myS
	CONTROL_SIGNAL &= ~(1 << 0); //clear bit 0 
	usleep(500); // wait 500myS
	CONTROL_SIGNAL |= (1 << 0); // bit 0 to 1

	//Set sda bit to 1
	CHANNEL_SIGNAL |= (1 << 4);
	

	//Write code that sets 6-bit values in register of DM163 chip. Recommended that every bit in that register is set to 1. 6-bits and 24 "bytes", so some kind of loop structure could be nice.
	//24*6 bits needs to be transmitted
	 uint8_t gamma_vector[3] = {0x3F, 0x3F, 0x3F}; // Gamma correction values (all bits set to 1)

    // Transmit data to the 8x8 LED matrix (8 rows, 3 colors per LED)
    for (int i = 0; i < 8; i++) { // Loop over 8 rows
        for (int color = 0; color < 3; color++) { // Loop over RGB colors
            uint8_t t = gamma_vector[color]; // Get the color value

            // Send 6-bit value (0b111111 = 0x3F)
            for (int bit = 5; bit >= 0; bit--) {   // Loop over 6 bits
                if (t & (1 << bit)) {
                    CHANNEL_SIGNAL = 1; // Write 1 to the register
                }else {
                    CONTROL_SIGNAL &=~0x10; //SET only BIT4 to 0 in control signal (SDA bit)
                    CONTROL_SIGNAL &=~0x08; //SET only BIT3 to 0 in control signal (CLK bit)
                    t <<= 1; //shift one to left
                    CONTROL_SIGNAL|=0x08; //SET only BIT3 to 1 in control signal (CLK bit)
        		}
    		}
		}
	}
	

	//Final thing in this function is to set SB-bit to 1 to enable transmission to 8-bit register.
	CONTROL_SIGNAL |= (1 << 2); //set bit2 (SB) to 1

}

//Change value of one pixel at led matrix. This function is only used for changing values of dots array
void SetPixel(uint8_t x,uint8_t y, uint8_t r, uint8_t g, uint8_t b){

	//Hint: you can invert Y-axis quite easily with 7-y
	dots[x][y][0]=b;
	//Write rest of two lines of code required to make this function work properly (green and red colors to array).


}


//Put new data to led matrix. Hint: This function is supposed to send 24-bytes and parameter x is for channel x-coordinate.
void run(uint8_t x){



	//Write code that writes data to led matrix driver (8-bit data). Use values from dots array
	//Hint: use nested loops (loops inside loops)
	//Hint2: loop iterations are 8,3,8 (pixels,color,8-bitdata)


}

//Latch signal. See colorsshield.pdf or DM163.pdf in project folder on how latching works
void latch(){


}


//Set one line (channel) as active, one at a time.
void open_line(uint8_t x){


}
