/*
 *    MAX6952.h - A library for controling multiple (max 16) MAX6952
 *    Copyright (c) 2019 Kai Krause
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */
 
 
 
 
 /* The you can control 4 Digits per MAX6952. No option to shorten (scanlimit).
  * If the text is shorter than the MaxTextLength, it is padded with blanks.
  * If the text is longer, only the first MaxTextLength- characters are used in static mode.
  *
  * Intensity can only set for all MAX6952  
  * Shutdown only for all MAX6952
  *
  *
  */
  
  
  
#ifndef MAX6952_h
#define MAX6952_h

//#include <avr/pgmspace.h>

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define LEFT				0
#define CENTER				2
#define RIGHT				1

#define	LEFT_TO_RIGHT		0
#define RIGHT_TO_LEFT		1

#define CLASSIC				0
#define BOUNCE				1



class MAX6952 {
    private :
        /* The array for shifting the data to the devices */
        //byte spidata[16];
        /* Send out a single command to the device */
        //void spiTransfer(int addr, byte opcode, byte data);

        /* We keep track of the led-status for all 8 devices in this array */
        byte status[64];
        /* Data is shifted out of this pin*/
        int SPI_MOSI;
        /* The clock is signaled on this pin */
        int SPI_CLK;
        /* This one is driven LOW for chip selectzion */
        int SPI_CS;
        /* The maximum number of devices we use */
        int maxDevices;
		/* The maximum characters we can display */
		int maxTextLength;

    public:
        /* 
         * Create a new controler 
         * Params :
         * dataPin		pin on the Arduino where data gets shifted out
         * clockPin		pin for the clock
         * csPin		pin for selecting the device 
         * numDevices	maximum number of devices that can be controled
         */
        MAX6952(int dataPin, int clkPin, int csPin, int numDevices=1);

        /*
         * Gets the number of devices attached to this MAX6952.
         * Returns :
         * int	the number of devices on this MAX6952
         */
        int getDeviceCount();
		
		/*
         * Gets the maximum length of the displayed text
         * Returns :
         * int	the number of digits
         */
        int getMaxTextLength();

        /* 
         * Set the shutdown (power saving) mode for the device
         * Params :
         * 
         * status	If true the device goes into power-down mode. Set to false
         *		for normal operation.
         */
        void shutdown(bool status);

		/* Set a specific register at MAX6952 
		 *
		 *
		 * addr		Address of the register
		 * data		data for register
		 */
		 void setRegister(byte addr, byte data );
       
        /* 
         * Set the brightness of the display.
         * Params:
         * 
         * intensity	the brightness of the display. (0..15)
         */
        void setIntensity( int intensity);

        /* 
         * Switch all Leds on the display off. 
         * Params:
         * 
         */
        void clearDisplay();


        /* 
         * Set a Text to the Display both planes set
         * Params :
         * 
         * text			the text to be displayed
         * position		left, right aligned or centered
         *  
         *		
         */
        void setText(String text, int position);
		
		
		/* 
         * Set a Text to the Display in plane0.
		 * Plane1 is blank to get blink effekt.
		 *
         * Params :
         * 
         * text			the text to be displayed
         * position		left, right aligned or centered
         * speed		blinking speed
         *		
         */
        void setTextBlink(String text,int speed, int position);
		
		
		/* 
         * Set a Text to the Display
         * Params :
         * 
         * text			the text to be displayed
         * direction	0 = rigth to left, 1 = left to right
         * speed		blinking speed
         *		
         */
        void setTextMarquee(String text,int speed, int mode, int direction);
		
		/* 
         * Set a Text to the Display
         * Params :
         * 
         * text			the text to be displayed
         *		
         */
        void writeDisplay(char *  text);
		
		
		/* 
         * Set a Text to the Display in both planes. Display shows alternating both texts
         * Params :
         * 
         * text1		the text to be displayed for plane1
		 * text2		the text to be displayed for plane2
         * position		left, right aligned or centered
         * speed		blinking speed
         *		
         */
        //void setTextBlink(String text1, String text2, int speed, int position);

        
};

#endif	//MAX6952.h



