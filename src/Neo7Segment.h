// ---------------------------------------------------------------------------
// Neo7Segment Library - v2.0.3 - 06/01/2019
//
// AUTHOR/LICENSE:
// Created by Seon Rozenblum - seon@unexpectedmaker.com
// Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// Modified by SupremeSports 06/01/2019
//
// LINKS:
// Project home: XXX <--
// Blog: XXX <--
//
// DISCLAIMER:
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
// Seven Segment Library for the Neo7Segment display boards, or for use with strips of NeoPixels arranged as 7 Segment displays
// This new update allows users to customize the number of pixels per segment or decimal point. It also allows many different
// 	displays to use separated classes, so displays can us different pixel numbers and digit numbers.
//
// SYNTAX:
//   Neo7Segment( digits, pixels, decimal, pin ) - Initialise the array of displays
//     Parameters:
//		* digits		- The number of digits you will be displaying on
//		* pixels		- The number of pixels per segment
//		* decimal		- The number of pixels per decimal point
//		* pin			- Pin speaker is wired to (other wire to ground, be sure to add an inline 100 ohm resistor).
//
// HISTORY:

//
// 02/01/2019 v2.0.3  - Modified to allow the class to work with many different display sizes. By SupremeSports
// 28/12/2018 v2.0.2  - Fixed few bugs with the variable arrays
// 05/12/2018 v2.0.1  - Fixed decimal point definition. Custom number of pixels for decimal point. By SupremeSports
// 05/12/2018 v2.0    - Custom number of pixel per segment. By SupremeSports
// 25/04/2018 v1.0.1  - Fixed bug due to change in Adafruit Neopixel library initialisation
// 28/01/2018 v1.0    - Initial release.
//
// ---------------------------------------------------------------------------

#ifndef Neo7Segment_h
	#define Neo7Segment_h

	#include <Adafruit_NeoPixel.h>

  	#if defined(ARDUINO) && ARDUINO >= 100
    	#include <Arduino.h>
  	#else
    	#include <WProgram.h>
		#include <pins_arduino.h>
	#endif

	#ifdef __AVR__
	  #include <avr/power.h>
	#endif
	
	#define PIXELS_PER_SEGMENT_MAX 10
	#define PIXELS_PER_DP_MAX 10

class Neo7Segment
{
	public:
		Neo7Segment( uint8_t displayCount, uint8_t dPixels, uint8_t dpPixels, uint8_t dPin );
		~Neo7Segment();
		
		void Begin( uint8_t brightness );
		void SetBrightness( uint8_t brightness );
		  
		void DisplayTextVerticalRainbow( String text, uint32_t colorA, uint32_t colorB );
		void DisplayTextHorizontalRainbow( String text, uint32_t colorA, uint32_t colorB );
		void DisplayTextColor( String text, uint32_t color );
		void DisplayTextColorCycle( String text, uint8_t index );
		void DisplayTextDigitColor( String text, uint32_t color[] );
		void DisplayTextMarquee( String text, uint8_t index, uint32_t color );
		void DisplayTextChaser( String text, uint8_t index, uint32_t color );
		void DisplayKnightRider( uint8_t index, uint32_t color );
		void DisplayBorderAnimation( uint8_t index, uint32_t color );
		void DisplayTime( uint8_t hours, uint8_t mins, uint8_t secs, uint32_t colorH, uint32_t colorM );
		
		void SetDigit( uint8_t digit, String text, uint32_t color );
		void SetDigitSegments( uint8_t digit, byte bits, uint32_t color );
		
	    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
		
		uint32_t Wheel( byte WheelPos );

		void ForceUppercase( bool force );
		
		// spinningAnims
		uint8_t GetSpinAllLength( void );
		uint8_t GetSpinTopLength( void );
		uint8_t GetSpinBottomLength( void );
		byte GetSpinAllAtIndex( uint8_t index );
		byte GetSpinTopAtIndex( uint8_t index );
		byte GetSpinBottomAtIndex( uint8_t index );

		bool IsReady( void );
		
		int FindIndexOfChar(String character);
		byte FindByteForCharater( String character );

	protected:
		void buildSegmentsPixels();
		void buildPixelsXY();

	private:
		Adafruit_NeoPixel pixels;
		uint8_t dispCount;
		uint8_t NUM_PIXELS_PER_BOARD;
		uint8_t dispPixelSegment;
		uint8_t dispPixelDp;
		uint8_t dispPin;
		bool dispUseDP;
		byte GetArraySize();
		String GetCharacterAtArrayIndex( int index );
		void SetupCharacters();
		void CheckToCacheBytes( String s );
		String PadTimeData( int8_t data );
		uint8_t Red( uint32_t col );
		uint8_t Green( uint32_t col );
		uint8_t Blue( uint32_t col );
		byte *cachedBytes;
		String cachedString;
				 bool isReady;
		 bool isForcedUpper;

		// Array of pixels per segment and pixels per decimal point
		byte segmentsPixels[8][PIXELS_PER_SEGMENT_MAX];

		// Array of pixel positions in X,Y format for mapping colours in X,Y space
		byte pixelsXY[( PIXELS_PER_SEGMENT_MAX*7 )+PIXELS_PER_DP_MAX][2];
};
#endif