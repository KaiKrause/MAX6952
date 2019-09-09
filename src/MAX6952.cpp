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

#include <SPI.h>
#include "MAX6952.h"

//the opcodes for the MAX6952

#define NOOP   					0x00

#define REG_INTENSITY_10   		0x01
#define REG_INTENSITY_32   		0x02
#define REG_SCANLIMIT   		0x03
#define REG_CONFIGURATION 		0x04  
#define REG_USER_DEFINED_FONTS	0x05
							//  0x06 Not in use
#define REG_DISPLAYTEST 		0x07

#define REG_P0_BASE 	0x20
#define REG_D0P0 		0x20
#define REG_D1P0 		0x21
#define REG_D2P0 		0x22
#define REG_D3P0 		0x23

#define REG_P1_BASE 	0x40
#define REG_D0P1 		0x40
#define REG_D1P1 		0x41
#define REG_D2P1 		0x42
#define REG_D3P1 		0x43

#define REG_P0P1_BASE 	0x60
#define REG_D0P0P1 		0x60
#define REG_D1P0P1 		0x61
#define REG_D2P0P1 		0x62
#define REG_D3P0P1 		0x63

#define SHUTDOWN_MODE				0b00000000  //D0 - S -> Shutdown
#define ACTIVE_MODE            		0b00000001 

												//D1 Not in use
									
#define SLOW_BLINK_RATE        		0b00000100	//D2 - B -> Blink Rate

#define GLOBAL_BLINK_ENABLE     	0b00001000	//D3 - E -> Global Blink
#define GLOBAL_BLINK_DISABLE     	0b00000000

#define GLOBAL_NO_BLINK_TIMING_SYNC 0b00000000	//D4 - T -> Global Blink Timing Synchronization
#define GLOBAL_BLINK_TIMING_SYNC 	0b00010000

#define GLOBAL_NO_CLEAR_DIGIT_DATA  0b00000000	//D5 - R -> Global Clear Digit Data
#define GLOBAL_CLEAR_DIGIT_DATA  	0b00100000

												//D6 Not in use
												
#define BLINK_P1_PHASE_READ_BACK    0b00000000	//D7 - P-> Blink Phase Readback
#define BLINK_P0_PHASE_READ_BACK    0b10000000

#define	MAX_DEVICES			16
#define DEBUG				0
#define DEBUG_LVL_2			0	



MAX6952::MAX6952(int dataPin, int clkPin, int csPin, int numDevices) {
	
	
	if(DEBUG){
		Serial.println("Konstruktor");
		
	}
	SPI_MOSI	=	dataPin;
    SPI_CLK		=	clkPin;
    SPI_CS		=	csPin;
    
	if(numDevices <= 0){
		numDevices = 1;
	}
	
	if(numDevices > MAX_DEVICES ){
		numDevices = MAX_DEVICES;
	}
	
	SPI.begin();
        
   	maxDevices = numDevices;
	maxTextLength = maxDevices * 4;
	
    pinMode(SPI_MOSI,OUTPUT);
    pinMode(SPI_CLK,OUTPUT);
    pinMode(SPI_CS,OUTPUT);
   
   
    setRegister(REG_SCANLIMIT,0x01);
	clearDisplay();
	setRegister(REG_DISPLAYTEST,0x01);
	delay(1000);
	setRegister(REG_DISPLAYTEST,0x00);
	
	
    }


int MAX6952::getDeviceCount() {
    return maxDevices;
}

void MAX6952::setRegister(byte addr, byte data){
	
	digitalWrite(SPI_CS, LOW);
 
	 
	 for(int i=1;i<(maxDevices +1);i++){
		
		if(DEBUG_LVL_2){
			Serial.print("Max6952 No: ");
			Serial.println(i);
		}
		
		SPI.transfer(addr);
		SPI.transfer(data);
		 
	}
	
	digitalWrite(SPI_CS, HIGH);
	delay(1);	
}

int MAX6952::getMaxTextLength() {
	return maxTextLength;
}

void MAX6952::shutdown(bool b) {
    
    if(b){
		if(DEBUG){
			Serial.println("Shutdown Display");
		}
		setRegister(REG_CONFIGURATION, SHUTDOWN_MODE);
	}
        
    else{
		if(DEBUG){
			Serial.println("Activate Display");
		}
		setRegister(REG_CONFIGURATION, ACTIVE_MODE);
	}
       
}


void MAX6952::setIntensity(int intensity) {
    
	
	if(intensity<=0){
		intensity = 1;
	}
	
	if(intensity>15){
		intensity = 15;
	}
		
	/*
	 * This function sets the same intensity to all digits
	 * The intensity is set in the both registers Intensity10 and Intensity32
 
	 * The lower  4 bit of Intensity10 are the intesity for Digit0
	 * The higher 4 bit of Intensity10 are the intesity for Digit1
	 * The lower  4 bit of Intensity32 are the intesity for Digit2
	 * The higher 4 bit of Intensity32 are the intesity for Digit3
	 */
 
	intensity = (intensity & 0x0f) + ((intensity & 0x0f)<<4);
	
	if(DEBUG){
		Serial.println("SetIntensity");
	}
	
	setRegister(REG_INTENSITY_10,intensity);
	setRegister(REG_INTENSITY_32,intensity);
 	
}

void MAX6952::clearDisplay() {
    
	if(DEBUG){
		Serial.println("Clear Display");
	}
	setRegister(REG_CONFIGURATION, GLOBAL_CLEAR_DIGIT_DATA);
	
}

void MAX6952::writeDisplay(char * deviceBuffer) {
	
	if(maxDevices == 1){
		
		if(DEBUG_LVL_2){
			Serial.println("Nur ein Max6952");
		}
		
		for(int i = 1;i < 5;i++){
			
		   
		    digitalWrite(SPI_CS, LOW);
		   
			if(DEBUG_LVL_2){
				Serial.println("LOW");
				Serial.print("Device:");
				Serial.print(1);
				Serial.print(" Digit:");
				Serial.print(4-i);
				Serial.print(" Addr: ");
				Serial.print(REG_P0P1_BASE + (4-i) );
				Serial.print(" ->");
				Serial.print(deviceBuffer[(1*4)-i]);
				Serial.println("");
			}
			
		    
			SPI.transfer(REG_P0P1_BASE + (4-i) );
			SPI.transfer(deviceBuffer[(1*4)-i]);

			
		    if(DEBUG_LVL_2){
				Serial.println("HIGH");
			}
			
		    digitalWrite(SPI_CS, HIGH);
		    delay(1);
        }
	} else {
		
		
		if(DEBUG_LVL_2){
			Serial.println("Mehrere Max6952");
		}
		
		for(int i = 1;i < 5;i++){
			
			digitalWrite(SPI_CS, LOW);
			
			if(DEBUG_LVL_2){
				Serial.println("LOW");
			}
			
			for(int j = maxDevices; j > 0 ;j--){
			
			if(DEBUG_LVL_2){
				Serial.print("Device:");
				Serial.print(j);
				Serial.print(" Digit:");
				Serial.print(4-i);
				Serial.print(" Addr: ");
				Serial.print(REG_P0P1_BASE + (4-i) );
				Serial.print(" ->");
				Serial.print(deviceBuffer[(j*4)-i]);
				Serial.println("");
			}
			
			SPI.transfer(REG_P0P1_BASE + (4-i) );
			SPI.transfer(deviceBuffer[(j*4)-i]);
			
			}
			
			
			if(DEBUG_LVL_2){
				Serial.println("HIGH");
			}
			
			digitalWrite(SPI_CS, HIGH);
			delay(1);
		}
	}
	
	
	
	
}

void MAX6952::setText(String inputText, int position){
	
	if(DEBUG){
		Serial.println("Set Text");
	}
	
	clearDisplay();
	setRegister(REG_CONFIGURATION,SLOW_BLINK_RATE + ACTIVE_MODE + GLOBAL_BLINK_DISABLE+GLOBAL_BLINK_TIMING_SYNC);
	
	String outputText ="";

	
	char deviceBuffer[maxTextLength];
 
	memset(deviceBuffer,0x00,sizeof(deviceBuffer));
 
	if(maxTextLength > inputText.length()) {
		
		if(DEBUG){
			Serial.println("Input < Display");
		}
		
		switch(position){
			case LEFT:
			{
				if(DEBUG){
					Serial.println("Left");
				}
				
				outputText = inputText;
   				for(int i = 0; i < (maxTextLength - inputText.length());i++){
					outputText.concat(" ");
				}
				break;
			}
			
			case RIGHT:
			{
				if(DEBUG){
					Serial.println("Right");
				}
				
				for(int i = 0; i < (maxTextLength - inputText.length());i++){
				outputText.concat(" ");
				}
				outputText.concat(inputText);
				break;
			}
			
			case CENTER:
			{
				if(DEBUG){
					Serial.println("Center");
				}
			
				int frontSpaces = (maxTextLength - inputText.length()) / 2;
								
				for(int i = 0; i < (frontSpaces);i++){
					outputText.concat(" ");
				}
				outputText.concat(inputText);
				
				
				for(int i = 0; i < (maxTextLength - inputText.length() - frontSpaces );i++){
					outputText.concat(" ");
				}
				break;
			}
			
			default:
			{
				if(DEBUG){
					Serial.println("Default: Left");
				}
				
				outputText = inputText;
   				for(int i = 0; i < (maxTextLength - inputText.length());i++){
					outputText.concat(" ");
				}
			
				break;
			}	
		}
		
		outputText.getBytes((byte*)deviceBuffer,sizeof(deviceBuffer)+1);
		
		
		if(DEBUG){
			Serial.print("MaxTextLength:");
			Serial.print(maxTextLength);
			Serial.print(" InputTextLength:");
			Serial.println(inputText.length());
			Serial.print("Input Text >");
			Serial.print(inputText);
			Serial.print("< Output Text >");
			Serial.print(deviceBuffer);
			Serial.println("<");
		}

		
   
	} else {
		
		if(DEBUG){
			Serial.println("Input > Display");
		}
		
		inputText.getBytes((byte*)deviceBuffer,sizeof(deviceBuffer)+1);
	   
		if(DEBUG){
			Serial.print("MaxTextLength:");
			Serial.print(maxTextLength);
			Serial.print(" InputTextLength:");
			Serial.println(inputText.length());
			Serial.print("Input Text >");
			Serial.print(inputText);
			Serial.print("< Output Text >");
			Serial.print(deviceBuffer);
			Serial.println("<");
		}

		
	}
 
  
 
  

	if(maxDevices == 1){
		
		if(DEBUG){
			Serial.println("Nur ein Max6952");
		}
		
		for(int i = 1;i < 5;i++){
			
		   
		    digitalWrite(SPI_CS, LOW);
		   
			if(DEBUG){
				Serial.println("LOW");
				Serial.print("Device:");
				Serial.print(1);
				Serial.print(" Digit:");
				Serial.print(4-i);
				Serial.print(" Addr: ");
				Serial.print(REG_P0P1_BASE + (4-i) );
				Serial.print(" ->");
				Serial.print(deviceBuffer[(1*4)-i]);
				Serial.println("");
			}
			
		    
			SPI.transfer(REG_P0P1_BASE + (4-i) );
			SPI.transfer(deviceBuffer[(1*4)-i]);

		  
		    if(DEBUG){
				Serial.println("HIGH");
			}
			
		    digitalWrite(SPI_CS, HIGH);
		    delay(1);
        }
	} else {
		
		if(DEBUG){
			Serial.println("Mehrere Max6952");
		}
		
		for(int i = 1;i < 5;i++){
			
			digitalWrite(SPI_CS, LOW);
			
			if(DEBUG){
				Serial.println("LOW");
			}
    
			for(int j = maxDevices; j > 0 ;j--){

			if(DEBUG){
				Serial.print("Device:");
				Serial.print(j);
				Serial.print(" Digit:");
				Serial.print(4-i);
				Serial.print(" Addr: ");
				Serial.print(REG_P0P1_BASE + (4-i) );
				Serial.print(" ->");
				Serial.print(deviceBuffer[(j*4)-i]);
				Serial.println("");
			}
			
			SPI.transfer(REG_P0P1_BASE + (4-i) );
			SPI.transfer(deviceBuffer[(j*4)-i]);
    
			}
			if(DEBUG){
				Serial.println("HIGH");
			}
			
			digitalWrite(SPI_CS, HIGH);
			delay(1);
		}
	}
  
 
	
	
}


void MAX6952::setTextBlink(String inputText,int speed, int position){
	
	if(DEBUG){
		Serial.println("Set Text Blink");
	}
	
	String outputText ="";

	//clearDisplay();
	setRegister(REG_CONFIGURATION,ACTIVE_MODE + GLOBAL_BLINK_ENABLE);
	
	
	
	char deviceBuffer[maxTextLength];
 
	memset(deviceBuffer,0x00,sizeof(deviceBuffer));
 
	if(maxTextLength > inputText.length()) {
		
		if(DEBUG){
			Serial.println("Input < Display");
		}
		
		switch(position){
			case LEFT:
			{
				if(DEBUG){
					Serial.println("Left");
				}
				
				outputText = inputText;
   				for(int i = 0; i < (maxTextLength - inputText.length());i++){
					outputText.concat(" ");
				}
				break;
			}
			
			case RIGHT:
			{
				if(DEBUG){
					Serial.println("Right");
				}
				
				for(int i = 0; i < (maxTextLength - inputText.length());i++){
				outputText.concat(" ");
				}
				outputText.concat(inputText);
				break;
			}
			
			case CENTER:
			{
				if(DEBUG){
					Serial.println("Center");
				}
			
				int frontSpaces = (maxTextLength - inputText.length()) / 2;
								
				for(int i = 0; i < (frontSpaces);i++){
					outputText.concat(" ");
				}
				outputText.concat(inputText);
				
				
				for(int i = 0; i < (maxTextLength - inputText.length() - frontSpaces );i++){
					outputText.concat(" ");
				}
				break;
			}
			
			default:
			{
				if(DEBUG){
					Serial.println("Default: Left");
				}
				
				outputText = inputText;
   				for(int i = 0; i < (maxTextLength - inputText.length());i++){
					outputText.concat(" ");
				}
			
				break;
			}	
		}
		
		outputText.getBytes((byte*)deviceBuffer,sizeof(deviceBuffer)+1);
		
		
		if(DEBUG){
			Serial.print("MaxTextLength:");
			Serial.print(maxTextLength);
			Serial.print(" InputTextLength:");
			Serial.println(inputText.length());
			Serial.print("Input Text >");
			Serial.print(inputText);
			Serial.print("< Output Text >");
			Serial.print(deviceBuffer);
			Serial.println("<");
		}

		
   
	} else {
		
		if(DEBUG){
			Serial.println("Input > Display");
		}
		
		inputText.getBytes((byte*)deviceBuffer,sizeof(deviceBuffer)+1);
	   
		if(DEBUG){
			Serial.print("MaxTextLength:");
			Serial.print(maxTextLength);
			Serial.print(" InputTextLength:");
			Serial.println(inputText.length());
			Serial.print("Input Text >");
			Serial.print(inputText);
			Serial.print("< Output Text >");
			Serial.print(deviceBuffer);
			Serial.println("<");
		}

		
	}
 
  
 
  

	if(maxDevices == 1){
		
		if(DEBUG){
			Serial.println("Nur ein Max6952");
		}
		
		for(int i = 1;i < 5;i++){
			
		   
		    digitalWrite(SPI_CS, LOW);
		   
			if(DEBUG){
				Serial.println("LOW");
				Serial.print("Device:");
				Serial.print(1);
				Serial.print(" Digit:");
				Serial.print(4-i);
				Serial.print(" Addr: ");
				Serial.print(REG_P0_BASE + (4-i) );
				Serial.print(" ->");
				Serial.print(deviceBuffer[(1*4)-i]);
				Serial.println("");
			}
			
		    
			SPI.transfer(REG_P0_BASE + (4-i) );
			SPI.transfer(deviceBuffer[(1*4)-i]);

		  
		    if(DEBUG){
				Serial.println("HIGH");
			}
			
		    digitalWrite(SPI_CS, HIGH);
		    delay(1);
        }
	} else {
		
		if(DEBUG){
			Serial.println("Mehrere Max6952");
		}
		
		for(int i = 1;i < 5;i++){
			
			digitalWrite(SPI_CS, LOW);
			
			if(DEBUG){
				Serial.println("LOW");
			}
    
			for(int j = maxDevices; j > 0 ;j--){

			if(DEBUG){
				Serial.print("Device:");
				Serial.print(j);
				Serial.print(" Digit:");
				Serial.print(4-i);
				Serial.print(" Addr: ");
				Serial.print(REG_P0_BASE + (4-i) );
				Serial.print(" ->");
				Serial.print(deviceBuffer[(j*4)-i]);
				Serial.println("");
			}
			
			SPI.transfer(REG_P0_BASE + (4-i) );
			SPI.transfer(deviceBuffer[(j*4)-i]);
    
			}
			if(DEBUG){
				Serial.println("HIGH");
			}
			
			digitalWrite(SPI_CS, HIGH);
			delay(1);
		}
	}
  
 
	
	
}


void MAX6952::setTextMarquee(String inputText,int speed, int mode, int direction){
	
	if(DEBUG){
		Serial.println("Set Text Marquee");
	}
	
	clearDisplay();
	setRegister(REG_CONFIGURATION, ACTIVE_MODE );
	
	String outputText ="";

	
	char deviceBuffer[maxTextLength];
	char marqueeBuffer[(2 * maxTextLength) + inputText.length()];
 
	memset(deviceBuffer,0x00,sizeof(deviceBuffer));
	memset(marqueeBuffer,0x00,sizeof(marqueeBuffer));
	
	if(inputText.length() > maxTextLength){
		
		if(mode == BOUNCE){
			
			if(DEBUG){
				Serial.println("BOUNCE mode not possible. Input text too long. Switch to classic marquee");
				mode = CLASSIC;
				
			}
			
			
		}
	}
 
	
	switch(mode){
		case CLASSIC:
		{
			if(DEBUG){
				Serial.println("CLASSIC");
			}
			for(int i = 0; i < (maxTextLength);i++){
				outputText.concat(" ");
			}
			outputText.concat(inputText);
			
			for(int i = 0; i < (maxTextLength);i++){
				outputText.concat(" ");
			}
			
			outputText.toCharArray(marqueeBuffer,sizeof(marqueeBuffer)+1);
			
			if(DEBUG){
				Serial.print("MaxTextLength:");
				Serial.print(maxTextLength);
				Serial.print(" InputTextLength:");
				Serial.println(inputText.length());
				Serial.print("Input Text >");
				Serial.print(inputText);
				Serial.print("< Output Text >");
				Serial.print(marqueeBuffer);
				Serial.println("<");
			}
			
			if(direction){
				
				if(DEBUG){
					Serial.println("RIGHT_TO_LEFT");
				}
				
				for(int i=0;i<(sizeof(marqueeBuffer)-maxTextLength);i++){
		
				strncpy(deviceBuffer,&marqueeBuffer[i],sizeof(deviceBuffer));
				writeDisplay(deviceBuffer);
				delay(speed);
				}
			} else{
				
				if(DEBUG){
					Serial.println("LEFT_TO_RIGTH");
				}
				
				for(int i=(sizeof(marqueeBuffer)-maxTextLength);i>0;i--){
		
				strncpy(deviceBuffer,&marqueeBuffer[i],sizeof(deviceBuffer));
				writeDisplay(deviceBuffer);
				delay(speed);
				}	
			}
			
			break;
		}
		
		
		case BOUNCE:
		{
			if(DEBUG){
				Serial.println("BOUNCE");
			}
		
										
			for(int i = 0; i < (maxTextLength - inputText.length());i++){
				outputText.concat(" ");
			}
			outputText.concat(inputText);
			
			
			for(int i = 0; i < (maxTextLength - inputText.length());i++){
				outputText.concat(" ");
			}
			
			outputText.toCharArray(marqueeBuffer,sizeof(marqueeBuffer)+1);
			
			
			if(DEBUG){
				Serial.print("MaxTextLength:");
				Serial.print(maxTextLength);
				Serial.print(" InputTextLength:");
				Serial.println(inputText.length());
				Serial.print("Input Text >");
				Serial.print(inputText);
				Serial.print("< Output Text >");
				Serial.print(marqueeBuffer);
				Serial.println("<");
			}
			
			
			if(direction){
				
				if(DEBUG){
					Serial.println("RIGHT_TO_LEFT");
				}
				
				for(int i=0;i<(maxTextLength - inputText.length());i++){
		
					strncpy(deviceBuffer,&marqueeBuffer[i],sizeof(deviceBuffer));
					writeDisplay(deviceBuffer);
					delay(speed);
				}
				for(int i=(maxTextLength - inputText.length());i > 0; i--){
					
					strncpy(deviceBuffer,&marqueeBuffer[i],sizeof(deviceBuffer));
					writeDisplay(deviceBuffer);
					delay(speed);
					
				}
				
			} else{
				
				if(DEBUG){
					Serial.println("LEFT_TO_RIGTH");
				}
				
				for(int i=(maxTextLength - inputText.length());i > 0; i--){
					
					strncpy(deviceBuffer,&marqueeBuffer[i],sizeof(deviceBuffer));
					writeDisplay(deviceBuffer);
					delay(speed);
					
				}
				for(int i=0;i<(maxTextLength - inputText.length());i++){
		
					strncpy(deviceBuffer,&marqueeBuffer[i],sizeof(deviceBuffer));
					writeDisplay(deviceBuffer);
					delay(speed);
				}
				
				
			}
			
			
			
			
			
			
			
			
			
			break;
		}
		
		default:
		{
				
			if(DEBUG){
				Serial.println("Default: CLASSIC");
			}
			for(int i = 0; i < (maxTextLength);i++){
				outputText.concat(" ");
			}
			outputText.concat(inputText);
			
			for(int i = 0; i < (maxTextLength);i++){
				outputText.concat(" ");
			}
			
			outputText.toCharArray(marqueeBuffer,sizeof(marqueeBuffer)+1);
			
			if(DEBUG){
				Serial.print("MaxTextLength:");
				Serial.print(maxTextLength);
				Serial.print(" InputTextLength:");
				Serial.println(inputText.length());
				Serial.print("Input Text >");
				Serial.print(inputText);
				Serial.print("< Output Text >");
				Serial.print(marqueeBuffer);
				Serial.println("<");
			}
			
			if(RIGHT_TO_LEFT){
				
				for(int i=0;i<(sizeof(marqueeBuffer)-maxTextLength);i++){
		
				strncpy(deviceBuffer,&marqueeBuffer[i],sizeof(deviceBuffer));
				writeDisplay(deviceBuffer);
				delay(speed);
				}
			} else{
				for(int i=0;i<(sizeof(marqueeBuffer)-maxTextLength);i++){
		
				strncpy(deviceBuffer,&marqueeBuffer[i],sizeof(deviceBuffer));
				writeDisplay(deviceBuffer);
				delay(speed);
				}	
			}
		
			break;
		}	
	}
	 
}
