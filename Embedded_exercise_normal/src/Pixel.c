/*
 * Pixel.c
 *
 *  Created on: -----
 *      Author: -----
 */

#include "Pixel.h"

//define control and chanel signals

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
	 uint8_t gamma_vector[3] = {63,63,63}; // Gamma correction values (all bits set to 1)

    // Transmit data to the 8x8 LED matrix (8 rows, 3 colors per LED)
    for (int i = 0; i < 8; i++) { // Loop over 8 rows
        for (int color = 0; color < 3; color++) { // Loop over RGB colors
            uint8_t t = gamma_vector[color]; // Get the color value

            // Send 6-bit value (0b111111 = 0x3F)
            for (int bit =0 ; bit < 6 ;bit++) {   // Loop over 6 bits
                if (t &  0x80) {
                    CONTROL_SIGNAL |= 0x10; // Write 1 to the register
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
	dots[x][7-y][0]=b;
    dots[x][7-y][1]=g;
	dots[x][7-y][2]=r;
	//Write rest of two lines of code required to make this function work properly (green and red colors to array).


}


//Put new data to led matrix. Hint: This function is supposed to send 24-bytes and parameter x is for channel x-coordinate.
void run(uint8_t x) {
    // Set latch to 0
    CONTROL_SIGNAL &= ~(1 << 1);

    // Iterate through rows (y), colors (color), and bits (x)
    for (uint8_t y = 0; y < 8; y++) {          // Loop through rows
        for (uint8_t color = 0; color < 3; color++) {  // Loop through color channels (B, G, R)
            for (uint8_t bit_pos = 0; bit_pos < 8; bit_pos++) {  // Loop through 8 bits in the row
                // Extract the bit from the dots array for the current pixel
                if (dots[x][y][color] & 0x80) {
                    // Set SDA (bit4) to 1
                    CONTROL_SIGNAL |= (1 << 4);
                } else {
                    // Set SDA (bit4) to 0
                    CONTROL_SIGNAL &= ~(1 << 4);
                }

                // Toggle CLK (bit3)
                CONTROL_SIGNAL &= ~(1 << 3); // Set CLK to 0
                CONTROL_SIGNAL |= (1 << 3);  // Set CLK to 1
                
            }
        }
    }

    latch();
    CONTROL_SIGNAL &= ~(1 << 3); // Set CLK to 0
}
	//Hint: use nested loops (loops inside loops)
	//Hint2: loop iterations are 8,3,8 (pixels,color,8-bitdata)


//Latch signal. See colorsshield.pdf or DM163.pdf in project folder on how latching works
void latch(){
	CONTROL_SIGNAL |= (1 << 1);
	CONTROL_SIGNAL &= ~(1 << 1);

}


//Set one line (channel) as active, one at a time.
void open_line(uint8_t channel) {
    // Clear all channels first to ensure only the selected channel is active
    CHANNEL_SIGNAL = 0;

    // Use switch-case to activate only the specified channel
    switch (channel) {
        case 0:
            CHANNEL_SIGNAL |= (1 << 0); // Activate channel 0
            break;
        case 1:
            CHANNEL_SIGNAL |= (1 << 1); // Activate channel 1
            break;
        case 2:
            CHANNEL_SIGNAL |= (1 << 2); // Activate channel 2
            break;
        case 3:
            CHANNEL_SIGNAL |= (1 << 3); // Activate channel 3
            break;
        case 4:
            CHANNEL_SIGNAL |= (1 << 4); // Activate channel 4
            break;
        case 5:
            CHANNEL_SIGNAL |= (1 << 5); // Activate channel 5
            break;
        case 6:
            CHANNEL_SIGNAL |= (1 << 6); // Activate channel 6
            break;
        case 7:
            CHANNEL_SIGNAL |= (1 << 7); // Activate channel 7
            break;
        default:
            // Default case to clear all channels if an invalid channel number is provided
            CHANNEL_SIGNAL = 0;
            break;
    }
}
