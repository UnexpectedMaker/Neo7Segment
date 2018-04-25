// ---------------------------------------------------------------------------
// Created by Seon Rozenblum - seon@unexpectedmaker.com
// Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// See "Neo7Segment.h" for purpose, syntax, version history, links, and more.
// ---------------------------------------------------------------------------

#include "Neo7Segment.h"
// This library is 100% entwined with the Adafruit NeoPixel library - please check their licensing terms in their library.
#include <Adafruit_NeoPixel.h>

//#define DEBUG
#define USEDP
#ifdef USEDP
	#define NUM_PIXELS_PER_BOARD 29
#else
	#define NUM_PIXELS_PER_BOARD 28
#endif

// Array of pixels per segment, 7 segments with 4 pixels each
byte segmentsPixels[8][4] 	{ { 0, 1, 2, 3 }, 
                              { 4, 5, 6, 7 }, 
                              { 8, 9, 10, 11 }, 
                              { 12, 13, 14, 15 }, 
                              { 16, 17, 18, 19 }, 
                              {20, 21, 22, 23 }, 
                              {24, 25, 26, 27 },
							  { 28 }
						  	};

// Array of pixel positions in X,Y format for mapping colours in X,Y space
byte pixelsXY[29][2]		{ { 1,0 }, { 2,0 }, { 3,0 }, { 4,0 },
	                          { 5,1 }, { 5,2 }, { 5,3 }, { 5,4 },
	                          { 5,6 }, { 5,7 }, { 5,8 }, { 5,9 },
	                          { 4,10 }, { 3,10 }, { 2,10 }, { 1,10 },
	                          { 0,9 }, { 0,8 }, { 0,7 },  { 0,6 },
	                          { 0,4 }, { 0,3 }, { 0,2 }, { 0,1 },
	                          { 1,5 }, { 2,5 }, { 3,5 }, { 4,5 },
							  { 6,10 }
						  	};
					  
// Array of segment based rainbow colour values
uint32_t segmentRainbow[7][3] {
                              { 255,0,0 },
                              { 255,124,42 },
                              { 255,255,0 },
                              { 69,223,91 },
                              { 31,189,255 },
                              { 150,11,255 },
                              { 255,30,237 }
                            };

// Available characters a 7 Segment display can show					  
const byte ARRAY_SIZE = 32;

byte available_codes[ ARRAY_SIZE ][ 2 ] {
							{ '0', 0b00111111  }, 
							{ '1', 0b00000110 },
							{ '2', 0b01011011 },
							{ '3', 0b01001111 },
							{ '4', 0b01100110 },
							{ '5', 0b01101101 },
							{ '6', 0b01111100 },
							{ '7', 0b00000111 },
							{ '8', 0b01111111 },
							{ '9', 0b01100111 },
							{ 'a', 0b01110111 },
							{ 'b', 0b01111100 },
							{ 'c', 0b00111001 },
							{ 'd', 0b01011110 },
							{ 'e', 0b01111001 },
							{ 'f', 0b01110001 },
							{ 'g', 0b01100111 },
							{ 'h', 0b01110110 },
							{ 'i', 0b00110000 },
							{ 'j', 0b00011110 },
							{ 'l', 0b00111000 },
							{ 'n', 0b01010100 },
							{ 'o', 0b01011100 },
							{ 'p', 0b01110011 },
							{ 'q', 0b01100111 },
							{ 'r', 0b01010000 },
							{ 's', 0b01101101 },
							{ 'u', 0b00111110 },
							{ 'x', 0b01110110 },
							{ 'y', 0b01101110 },
							{ '-', 0b01000000 },
							{ ' ', 0b00000000 }
						};

byte available_codes_upper[ ARRAY_SIZE ][ 2 ] {
							{ '0', 0b00111111 }, 
							{ '1', 0b00000110 },
							{ '2', 0b01011011 },
							{ '3', 0b01001111 },
							{ '4', 0b01100110 },
							{ '5', 0b01101101 },
							{ '6', 0b01111100 },
							{ '7', 0b00000111 },
							{ '8', 0b01111111 },
							{ '9', 0b01100111 },
							{ 'a', 0b01110111 },
							{ 'b', 0b01111111 },
							{ 'c', 0b00111001 },
							{ 'd', 0b00111111 },
							{ 'e', 0b01111001 },
							{ 'f', 0b01110001 },
							{ 'g', 0b01100111 },
							{ 'h', 0b01110110 },
							{ 'i', 0b00110000 },
							{ 'j', 0b00011110 },
							{ 'l', 0b00111000 },
							{ 'n', 0b00110111 },
							{ 'o', 0b00111111 },
							{ 'p', 0b01110011 },
							{ 'q', 0b01100111 },
							{ 'r', 0b00110001 },
							{ 's', 0b01101101 },
							{ 'u', 0b00111110 },
							{ 'x', 0b01110110 },
							{ 'y', 0b01101110 },
							{ '-', 0b01000000 },
							{ ' ', 0b00000000 }
						};					

byte spinAnimAll[6] 	{
							0b00000001,
							0b00000010,
							0b00000100,
							0b00001000,
							0b00010000,
							0b00100000
						};
					
byte spinAnimTop[4] 	{
							0b00000001,
							0b00000010,
							0b01000000,
							0b00100000
						};

byte spinAnimBottom[4] 	{
							0b01000000,
							0b00000100,
							0b00001000,
							0b00010000
						};

byte AnimEdgeLeft[4] 	{
							0b00001000,
							0b00010000,
							0b00100000,
							0b00000001
						};
byte AnimEdgeRight[4] 	{
							0b00000001,
							0b00000010,
							0b00000100,
							0b00001000
						};
													
Neo7Segment::Neo7Segment( uint8_t displayCount, uint8_t dPin )
{
	dispCount = displayCount;
	dispPin = dPin;
	pixels = Adafruit_NeoPixel ();
	pixels.updateType( NEO_GRB + NEO_KHZ800 );
  	pixels.updateLength( dispCount * NUM_PIXELS_PER_BOARD );
  	pixels.setPin(dispPin);
	isReady = false;
}

Neo7Segment::~Neo7Segment()
{
	isReady = false;
}

bool Neo7Segment::IsReady()
{
	return isReady;
}

void Neo7Segment::Begin( uint8_t brightness )
{

	pixels.begin(); // This initializes the NeoPixel library.
	pixels.show();
	pixels.setBrightness( brightness );

	cachedString = "";
	cachedBytes = (byte *) malloc(dispCount * sizeof(byte));
	
	for ( int i = 0; i < dispCount; i++ )
		cachedBytes[i] = 0;

	#ifdef DEBUG
		Serial.print("Brightness: ");
		Serial.println(brightness);
		Serial.println("Ready!");
	#endif

	//Digits are initialised and ready
	isReady = true;
	isForcedUpper = false;
}

void Neo7Segment::SetBrightness( uint8_t brightness )
{
	if ( isReady )
	{
		pixels.setBrightness( brightness );
	}
}

void Neo7Segment::ForceUppercase( bool force )
{
	isForcedUpper = force;
}

byte Neo7Segment::GetArraySize()
{
	return ARRAY_SIZE;
}

String Neo7Segment::GetCharacterAtArrayIndex( int index )
{
	return ( (String)(char)available_codes[ index ][0] );
}

// Convert separate R,G,B into packed 32-bit RGB color.
// Packed format is always RGB, regardless of LED strand color order.
uint32_t Neo7Segment::Color(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}


void Neo7Segment::CheckToCacheBytes( String str )
{
	if ( str != cachedString )
	{
		cachedString = str;
		
		int index = 0;
		for ( int s = 0; s < str.length(); s++ )
		{
			#ifdef DEBUG
			Serial.print( (String)str.charAt(s) );
			Serial.print( " .. " );
			#endif

			if ( (String)str.charAt(s) != "." )
			{ 
				cachedBytes[index] = FindByteForCharater( (String)str.charAt(s) );
				#ifdef DEBUG
				Serial.println( "1" );
				#endif
				index++;
			}
			else if ( s > 0 && bitRead( cachedBytes[index-1], 7 ) != 1 )
			{
				cachedBytes[index-1] = cachedBytes[index-1] | 0b10000000;
				#ifdef DEBUG
				Serial.println( "2" );
				#endif
			}
			else
			{
				cachedBytes[index] = 0b10000000;
				#ifdef DEBUG
				Serial.println( "3" );
				#endif
				index++;
			}
		}
	}
}

void Neo7Segment::DisplayTextVerticalRainbow( String text, uint32_t colorA, uint32_t colorB )
{
	if ( !isReady )
		return;
		
	CheckToCacheBytes( text );

	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	uint32_t color;
	
	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		if(code > -1)
		{
			int pixelIndex = 0; // current pixel index
		
			for( int segment = 0; segment < 7; segment++ )
			{
				bool on = ( bitRead( code, segment) == 1 );
				for ( int p = 0; p < 4; p++ )
				{
					// we want the Y position (row) so we can use that as the colour index 
					int y = pixelsXY[ pixelIndex ][1];

					uint8_t red = ((Red(colorA) * (10 - y)) + (Red(colorB) * y)) * 0.1;
					uint8_t green = ((Green(colorA) * (10 - y)) + (Green(colorB) * y)) * 0.1;
					uint8_t blue = ((Blue(colorA) * (10 - y)) + (Blue(colorB) * y)) * 0.1;

					color = Color(red, green, blue );
				
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
					pixelIndex++;
				}
			}

			#ifdef USEDP

				bool on = ( bitRead( code, 7) == 1 );
				pixels.setPixelColor( segmentsPixels[ 7 ][ 0 ], on ? color : Color(0,0,0));

			#endif
		}
	}
		
	// we have finished setting all of the colors on each segment for this Neo7Segment, so lets turn on the pixels
	pixels.show(); 
}

void Neo7Segment::DisplayTextHorizontalRainbow( String text, uint32_t colorA, uint32_t colorB )
{
	if ( !isReady )
		return;	
	
	CheckToCacheBytes( text );

	int numPixelsPerColumn = 6;
	int numColumns = dispCount * numPixelsPerColumn;

	uint32_t color;

	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );
	
	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		if(code > -1)
		{
			int pixelIndex = 0; // current pixel index
		
			for( int segment = 0; segment < 7; segment++ )
			{
				bool on = ( bitRead( code, segment) == 1 );
				for ( int p = 0; p < 4; p++ )
				{
					// we want the Y position (row) so we can use that as the colour index 
					int x = pixelsXY[ pixelIndex ][0] + ( numPixelsPerColumn * s );

					uint8_t red = ((Red(colorA) * (numColumns - x)) + (Red(colorB) * x)) / numColumns;
					uint8_t green = ((Green(colorA) * (numColumns - x)) + (Green(colorB) * x)) / numColumns;
					uint8_t blue = ((Blue(colorA) * (numColumns - x)) + (Blue(colorB) * x)) / numColumns;

					color = Color(red, green, blue );
				
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
					pixelIndex++;
				}
			}

			#ifdef USEDP

				bool on = ( bitRead( code, 7) == 1 );
				pixels.setPixelColor( segmentsPixels[ 7 ][ 0 ], on ? color : Color(0,0,0));

			#endif
		}
	}
		
	// we have finished setting all of the colors on each segment for this Neo7Segment, so lets turn on the pixels
	pixels.show(); 
}

void Neo7Segment::DisplayTextMarquee( String text, uint8_t index, uint32_t color )
{
	if ( !isReady )
		return;
		
	CheckToCacheBytes( text );
	
	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		
		if(code > -1)
		{
			for( int segment = 0; segment < 7; segment++ )
			{
				for ( int p = 0; p < 4; p++ )
				{
					bool on = ( bitRead( code, segment) == 1 );
									
					if ( index == 0 && ( p == 1 || p == 3 ) )
						on = false;
					else if ( index == 1 && ( p == 0 || p == 2 ) )
						on = false;

					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
				}
			}

			#ifdef USEDP

				bool on = ( bitRead( code, 7) == 1 );
				pixels.setPixelColor( segmentsPixels[ 7 ][ 0 ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0));

			#endif
		}
	}
	// we have finished setting all of the colors on each segment for this Neo7Segment, so lets turn on the pixels
	pixels.show();
}

void Neo7Segment::DisplayTextChaser( String text, uint8_t index, uint32_t color )
{
	if ( !isReady )
		return;
	
	CheckToCacheBytes( text );
	
	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		
		if(code > -1)
		{
			for( int segment = 0; segment < 7; segment++ )
			{
				for ( int p = 0; p < 4; p++ )
				{
					bool on = ( bitRead( code, segment) == 1 );
									
					if ( p != index )
						on = false;

					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
				}
			}

			#ifdef USEDP

				bool on = ( bitRead( code, 7) == 1 );
				pixels.setPixelColor( segmentsPixels[ 7 ][ 0 ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0));

			#endif
		}
	}
	// we have finished setting all of the colors on each segment for this Neo7Segment, so lets turn on the pixels
	pixels.show();
}

void Neo7Segment::DisplayTextColorCycle( String text, uint8_t index )
{
	if ( !isReady )
		return;
		
	CheckToCacheBytes( text );	

	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	uint32_t color;
	
	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		
		if(code > -1)
		{
			int colorStart = index;
				
			for( int segment = 0; segment < 7; segment++ )
			{
				bool on = ( bitRead( code, segment) == 1 );
				for ( int p = 0; p < 4; p++ )
				{
					color = Wheel( colorStart & 255 );
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
					colorStart+=(255/28);	
				}
			}

			#ifdef USEDP

				bool on = ( bitRead( code, 7) == 1 );
				pixels.setPixelColor( segmentsPixels[ 7 ][ 0 ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0));

			#endif
		}
	}

	// we have finished setting all of the colors on each segment for this Neo7Segment, so lets turn on the pixels
	pixels.show();
}


void Neo7Segment::DisplayTextColor( String text, uint32_t color )
{
	if ( !isReady )
		return;
		
	CheckToCacheBytes( text );
	
	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		
		if(code > -1)
		{
			for( int segment = 0; segment < 7; segment++ )
			{
				bool on = ( bitRead( code, segment) == 1 );
				for ( int p = 0; p < 4; p++ )
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
			}

			#ifdef USEDP

				bool on = ( bitRead( code, 7) == 1 );
				pixels.setPixelColor( segmentsPixels[ 7 ][ 0 ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0));

			#endif
		}
	}
	// we have finished setting all of the colors on each segment for this Neo7Segment, so lets turn on the pixels
	pixels.show();
}

void Neo7Segment::DisplayTime( uint8_t hours, uint8_t mins, uint8_t secs, uint32_t colorH, uint32_t colorM )
{
	if ( !isReady )
		return;
		
	String text = PadTimeData( hours ) + PadTimeData( mins );

	if ( text.length() < dispCount )
	{
		for ( int i = 0; i < (dispCount - text.length()); i++ )
			text = " " + text;
	}

	CheckToCacheBytes( text );

	uint8_t r = Red( colorM ) * 0.5;
	uint8_t g = Green( colorM ) * 0.5;
	uint8_t b = Blue( colorM ) * 0.5;
	uint32_t colorM2 = Color( r, g, b );
	
	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		
		if(code > -1)
		{
			uint32_t cachedColor = colorH;

			// displaying mins, so work out new color
			if ( s >= dispCount - 2 )
			{
				cachedColor = ( secs % 2 == 0 ) ? colorM2 : colorM;
			}

			for( int segment = 0; segment < 7; segment++ )
			{
				bool on = ( bitRead( code, segment) == 1 );
				for ( int p = 0; p < 4; p++ )
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? cachedColor : Color(0,0,0) );
			}

			#ifdef USEDP

				bool on = ( bitRead( code, 7) == 1 );
				pixels.setPixelColor( segmentsPixels[ 7 ][ 0 ] + ( s * NUM_PIXELS_PER_BOARD ), on ? cachedColor : Color(0,0,0));

			#endif
		}
	}
	// we have finished setting all of the colors on each segment for this Neo7Segment, so lets turn on the pixels
	pixels.show();
}

void Neo7Segment::DisplayKnightRider( uint8_t index, uint32_t color )
{
	if ( !isReady )
		return;

	bool isForward = true;

	uint8_t r = Red( color ) * 0.5;
	uint8_t g = Green( color ) * 0.5;
	uint8_t b = Blue( color ) * 0.5;

	uint32_t colorFade = Color( r, g, b );

	int pixelIndex = ( index % (dispCount * 8 ) );
	if ( pixelIndex > (dispCount * 4) - 1 )
	{
		pixelIndex = ( dispCount * 8 ) - (pixelIndex + 1 );
		isForward = false;
	}
	
	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < dispCount; s++ )
	{
		for( int segment = 0; segment < 7; segment++ )
		{
			for ( int p = 0; p < 4; p++ )
			{
				if ( segment != 6 )
				{
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), Color(0,0,0) );
				}
				else
				{
					uint8_t currentIndex = ( s * 4 + p );
					if ( currentIndex == pixelIndex )
						pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), color );
					else if ( isForward && currentIndex == pixelIndex - 1 )
						pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), colorFade );
					else if ( !isForward && currentIndex == pixelIndex + 1 )
						pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), colorFade );
					else
						pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), Color(0,0,0) );
				}
			}		
		}

		#ifdef USEDP

			pixels.setPixelColor( segmentsPixels[ 7 ][ 0 ] + ( s * NUM_PIXELS_PER_BOARD ), Color(0,0,0) );

		#endif
	}
	// we have finished setting all of the colors on each segment for this Neo7Segment, so lets turn on the pixels
	pixels.show();
}

void Neo7Segment::DisplayBorderAnimation( uint8_t index, uint32_t color )
{
	if ( !isReady )
		return;
		
	int numSegmentSteps = 6; // default for 1 display only
	int leftOverDisplayCount = 0; // default left over displays from the middle
	if ( dispCount > 1 )
	{
		if ( dispCount < 3 )
			numSegmentSteps = 8;
		else
		{
			leftOverDisplayCount = dispCount - 2;
			numSegmentSteps = 8 + ( 2 * leftOverDisplayCount );
		}
	}

	int step = (index % numSegmentSteps);
	int curentDisplay = 0;


	byte code = -1;
	if ( step < 4 )
	{
		code = AnimEdgeLeft[ step ];
		curentDisplay = 0;
	}
	else if ( step < ( numSegmentSteps / 2 ) )
	{
		curentDisplay = step - 3;
		code = 0b00000001;
	}
	else if ( step > ( numSegmentSteps - leftOverDisplayCount - 1 ) )
	{
		curentDisplay = abs( step - numSegmentSteps );
		code = 0b00001000;
	}
	else
	{
		code = AnimEdgeRight[ step - ( numSegmentSteps / 2 ) ];
		curentDisplay = dispCount - 1;
	}

	for ( int s = 0; s < dispCount; s++ )
	{
		for( int segment = 0; segment < 7; segment++ )
		{
			if ( s == curentDisplay )
			{
				bool on = ( bitRead( code, segment) == 1 );
				for ( int p = 0; p < 4; p++ )
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
			}
			else
			{
				for ( int p = 0; p < 4; p++ )
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), Color(0,0,0) );
			}
		}

		#ifdef USEDP

			pixels.setPixelColor( segmentsPixels[ 7 ][ 0 ] + ( s * NUM_PIXELS_PER_BOARD ), Color(0,0,0) );

		#endif
	}

	// we have finished setting all of the colors on each segment for this Neo7Segment, so lets turn on the pixels
	pixels.show();
}

uint8_t Neo7Segment::GetSpinAllLength( void )
{
	return ( sizeof(spinAnimAll) / sizeof(byte) );
}
uint8_t Neo7Segment::GetSpinTopLength( void )
{
	return ( sizeof(spinAnimTop) / sizeof(byte) );
}
uint8_t Neo7Segment::GetSpinBottomLength( void )
{
	return ( sizeof(spinAnimBottom) / sizeof(byte) );
}

byte Neo7Segment::GetSpinAllAtIndex( uint8_t index )
{
	if ( index < GetSpinAllLength() )
		return ( spinAnimAll[ index ] );
}

byte Neo7Segment::GetSpinTopAtIndex( uint8_t index )
{
	if ( index < GetSpinTopLength() )
		return ( spinAnimTop[ index ] );
}

byte Neo7Segment::GetSpinBottomAtIndex( uint8_t index )
{
	if ( index < GetSpinBottomLength() )
		return ( spinAnimBottom[ index ] );
}
		

void Neo7Segment::SetDigit( uint8_t digit, String text, uint32_t color )
{
	if ( !isReady )
		return;
		
}
void Neo7Segment::SetDigitSegments( uint8_t digit, byte bits, uint32_t color )
{
	if ( !isReady )
		return;
	
	if ( digit < 0 || digit > dispCount )
		return;
	
	for( int segment = 0; segment < 7; segment++ )
	{
		bool on = ( bitRead( bits, segment ) == 1 );
		for ( int p = 0; p < 4; p++ )
			pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( digit * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );		
	}

	#ifdef USEDP

		bool on = ( bitRead( bits, 7 ) == 1 );
		pixels.setPixelColor( segmentsPixels[ 7 ][ 0 ] + ( digit * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );

	#endif

	pixels.show();
}

int Neo7Segment::FindIndexOfChar(String character)
{
	String s = character;
	s.toLowerCase();
	for(int i=0; i< ARRAY_SIZE; i++)
	{
		if( s.equals( (String)( (char)available_codes[i][0] ) ) )
			return i;
	}
	return -1;
}

byte Neo7Segment::FindByteForCharater( String character )
{
	String s = character;
	s.toLowerCase();
	for(int i=0; i< ARRAY_SIZE; i++)
	{
		if ( isForcedUpper )
		{
			if( s.equals( (String)( (char)available_codes_upper[i][0] ) ) )
				return available_codes_upper[i][1];
		}
		else
		{
			if( s.equals( (String)( (char)available_codes[i][0] ) ) )
				return available_codes[i][1];
		}
	}
	return -1;
}

String Neo7Segment::PadTimeData( int8_t data )
{
  if( data < 10 )
	return String("0") + String(data);

  return String(data);
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Neo7Segment::Wheel(byte WheelPos )
{
	WheelPos = 255 - WheelPos;
	if(WheelPos < 85)
		return Color(255 - WheelPos * 3, 0, WheelPos * 3);

	if(WheelPos < 170)
	{
		WheelPos -= 85;
		return Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	
	WheelPos -= 170;
	return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint8_t Neo7Segment::Red( uint32_t col )
{
	return col >> 16;
}

uint8_t Neo7Segment::Green( uint32_t col )
{
	return col >> 8;
}


uint8_t Neo7Segment::Blue( uint32_t col )
{
	return col;
}
