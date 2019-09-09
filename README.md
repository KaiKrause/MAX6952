MAX6952
==========
MAX6952 is an [Arduino](http://arduino.cc) library for MAX6952 Led display drivers.
The code also works with the ESP32

Documentation
-------------
Documentation for the library is not available. 

        /* 
         * Set a Text to the Display both planes set
         * Params :
         * 
         * text			the text to be displayed
         * position		LEFT = 0 = left aligned
         *            RIGHT = 1
         *            CENTER = 2
         *  
         *		
         */
        void setText(String text, int position);
        
        
        
        /* 
         * Set a Text to the Display
         * Params :
         * 
         * text			  the text to be displayed
         * direction	RIGHT_TO_LEFT = 1 = scrolling from rigth to left
         *            LEFT_TO_RIGHT = 0 = scrolling from left to right
         * speed		  moving speed in milliseconds
         * mode     	CLASSIC = 0 = scroll though
         *            BOUNCE = 1 = scroll back and forth
         */
        void setTextMarquee(String text,int speed, int mode, int direction);
        
        

Known Issues: Global blink is not in Sync when multiple MAX6952 are used.

Download
--------
The lastest binary version of the Library is always available from the 
[MAX6952 Release Page](https://github.com/KaiKrause/MAX6952) 


Install
-------
The library can be installed using the [standard Arduino library install procedure](http://arduino.cc/en/Guide/Libraries#.UwxndHX5PtY)  







