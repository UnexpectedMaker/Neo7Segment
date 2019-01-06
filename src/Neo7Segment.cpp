// ---------------------------------------------------------------------------
// Created by Seon Rozenblum - seon@unexpectedmaker.com
// Modified by Jean Gauthier P.Eng. - SupremeSports (2019-01-06)
// Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// See "Neo7Segment.h" for purpose, syntax, version history, links, and more.
// ---------------------------------------------------------------------------

#include "Neo7Segment.h"
// This library is 100% entwined with the Adafruit NeoPixel library - please check their licensing terms in their library.
#include <Adafruit_NeoPixel.h>

//#define DEBUG
#define USEDP

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
const byte ARRAY_SIZE = 34;


byte available_codes[ ARRAY_SIZE ][ 2 ] {
							//   pgfedcba
						  { '0', 0b00111111 },
						  { '1', 0b00000110 },
						  { '2', 0b01011011 },
						  { '3', 0b01001111 },
						  { '4', 0b01100110 },
						  { '5', 0b01101101 },
						  { '6', 0b01111101 },
						  { '7', 0b00000111 },
						  { '8', 0b01111111 },
						  { '9', 0b01101111 },
						  { 'a', 0b01110111 },
						  { 'b', 0b01111100 },
						  { 'c', 0b01011000 },
						  { 'd', 0b01011110 },
						  { 'e', 0b01111001 },
						  { 'f', 0b01110001 },
						  { 'g', 0b01100111 },
						  { 'h', 0b01110110 },
						  { 'i', 0b00010000 },
						  { 'j', 0b00011110 },
						  { 'l', 0b00110000 },
						  { 'n', 0b01010100 },
						  { 'o', 0b01011100 },
						  { 'p', 0b01110011 },
						  { 'q', 0b01100111 },
						  { 'r', 0b01010000 },
						  { 's', 0b01101101 },
						  { 't', 0b01111000 },
						  { 'u', 0b00011100 },
						  { 'x', 0b01110110 },
						  { 'y', 0b01101110 },
						  { 'z', 0b01011011 },
						  { '-', 0b01000000 },
						  { ' ', 0b00000000 }
						};

byte available_codes_upper[ ARRAY_SIZE ][ 2 ] {
							//   pgfedcba
						  { '0', 0b00111111 },
						  { '1', 0b00000110 },
						  { '2', 0b01011011 },
						  { '3', 0b01001111 },
						  { '4', 0b01100110 },
						  { '5', 0b01101101 },
						  { '6', 0b01111101 },
						  { '7', 0b00000111 },
						  { '8', 0b01111111 },
						  { '9', 0b01101111 },
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
						  { 't', 0b01111000 },
						  { 'u', 0b00111110 },
						  { 'x', 0b01110110 },
						  { 'y', 0b01101110 },
						  { 'z', 0b01011011 },
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

Neo7Segment::Neo7Segment( uint8_t displayCount, uint8_t dPixels, uint8_t dpPixels, uint8_t dPin )
{
	//Count number of pixels per decimal point (Between 0 and number of segment pixels)
	#ifdef USEDP
		dispPixelDp = constrain(dpPixels, 0, constrain(dPixels, 1, PIXELS_PER_SEGMENT_MAX));
	#else
		dispPixelDp = 0;
	#endif

	//Count number of pixels per digit board (Between 1 and maximum pixels)
	NUM_PIXELS_PER_BOARD = ( constrain( dPixels, 1, PIXELS_PER_SEGMENT_MAX )*7) + dispPixelDp;

	//Detect that decimal point is used or not
	dispUseDP = dispPixelDp>0 ? true : false;

	dispPixelSegment = dPixels;
	dispCount = displayCount;
	dispPin = dPin;
	pixels = Adafruit_NeoPixel ();
	pixels.updateType( NEO_GRB + NEO_KHZ800 );
  	pixels.updateLength( dispCount * NUM_PIXELS_PER_BOARD );
  	pixels.setPin(dispPin);
	isReady = false;
}

Neo7Segment::Neo7Segment( uint8_t displayCount, uint8_t dPin )
{
	//Count number of pixels per decimal point (Between 0 and number of segment pixels)
	#ifdef USEDP
		dispPixelDp = 1;
	#else
		dispPixelDp = 0;
	#endif

	//Count number of pixels per digit board (Between 1 and maximum pixels)
	NUM_PIXELS_PER_BOARD = 28 + dispPixelDp;

	//Detect that decimal point is used or not
	dispUseDP = dispPixelDp>0 ? true : false;

	dispPixelSegment = 4;
	dispCount = displayCount;
	dispPin = dPin;
	pixels = Adafruit_NeoPixel ();
	pixels.updateType( NEO_GRB + NEO_KHZ800 );
	pixels.updateLength( dispCount * NUM_PIXELS_PER_BOARD );
	pixels.setPin(dispPin);
	isReady = false;
}

void Neo7Segment::buildSegmentsPixels()
{
	uint8_t segPixels = dispPixelSegment;
	uint8_t dpPixels = dispPixelDp;

	for ( int i = 0; i < 7; i++ )
	{
		for ( int j = 0; j < segPixels; j++ )
			segmentsPixels[ i ][ j ] = ( i * segPixels ) + j;
	}

	for ( int j = 0; j < dpPixels; j++ )
		segmentsPixels[ 7 ][ j ] = ( segPixels * 7) + j;
}

void Neo7Segment::buildPixelsXY()
{
	uint8_t segPixels = dispPixelSegment;
	uint8_t dpPixels = dispPixelDp;
	int i = 0;

	for ( i = 0; i < segPixels; i++ )
	{
		//A Segment
		pixelsXY[ i + ( 0*segPixels ) ][ 0 ] = i + 1;
		pixelsXY[ i + ( 0*segPixels ) ][ 1 ] = 0;

		//B Segment
		pixelsXY[ i + ( 1*segPixels ) ][ 0 ] = segPixels + 1;
		pixelsXY[ i + ( 1*segPixels ) ][ 1 ] = i + 1;

		//C Segment
		pixelsXY[ i + ( 2*segPixels ) ][ 0 ] = segPixels + 1;
		pixelsXY[ i + ( 2*segPixels ) ][ 1 ] = segPixels + i + 2;

		//D Segment
		pixelsXY[ i + ( 3*segPixels ) ][ 0 ] = segPixels - i;
		pixelsXY[ i + ( 3*segPixels ) ][ 1 ] = ( segPixels * 2 ) + 2;

		//E Segment
		pixelsXY[ i + ( 4*segPixels ) ][ 0 ] = 0;
		pixelsXY[ i + ( 4*segPixels ) ][ 1 ] = ( ( segPixels * 2 ) + 1 ) - i;

		//F Segment
		pixelsXY[ i + ( 5*segPixels ) ][ 0 ] = 0;
		pixelsXY[ i + ( 5*segPixels ) ][ 1 ] = segPixels - i;

		//G Segment
		pixelsXY[ i + ( 6*segPixels ) ][ 0 ] = i + 1;
		pixelsXY[ i + ( 6*segPixels ) ][ 1 ] = segPixels + 1;
	}

	for (i = 0; i < dpPixels; i++)
	{
		//DP Segment
		pixelsXY[ i + ( 7*segPixels ) ][ 0 ] = segPixels + 2;
		pixelsXY[ i + ( 7*segPixels ) ][ 1 ] = ( segPixels * 2 ) + 2;
	}
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

	#ifdef DEBUG
		Serial.print("Brightness: ");
		Serial.println(brightness);
		Serial.println("Ready!");
	#endif

	cachedString = "";
	cachedBytes = (byte *) malloc(dispCount * sizeof(byte));
	
	for ( int i = 0; i < dispCount; i++ )
		cachedBytes[i] = 0;

	#ifdef DEBUG
		Serial.print("Initialize: ");
		Serial.print(dispPixelSegment);
		Serial.println(" pixels per segment...");
	#endif

	buildSegmentsPixels();
	buildPixelsXY();

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
	
	int digitHeight = 2 * dispPixelSegment + 3;
	float digitHeightFloat = 1.0 / float( digitHeight );

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
				for ( int p = 0; p < dispPixelSegment; p++ )
				{
					// we want the Y position (row) so we can use that as the colour index 
					int y = pixelsXY[ pixelIndex ][1];

					uint8_t red = ((Red(colorA) * (digitHeight - y)) + (Red(colorB) * y)) * digitHeightFloat;
					uint8_t green = ((Green(colorA) * (digitHeight - y)) + (Green(colorB) * y)) * digitHeightFloat;
					uint8_t blue = ((Blue(colorA) * (digitHeight - y)) + (Blue(colorB) * y)) * digitHeightFloat;

					color = Color(red, green, blue );
				
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
					pixelIndex++;
				}
			}

			if ( dispUseDP )
			{
				bool on = ( bitRead( code, 7) == 1 );
				for ( int p = 0; p < dispPixelDp; p++)
					pixels.setPixelColor( segmentsPixels[ 7 ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
			}
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

	int numPixelsPerColumn = dispPixelSegment + 2;
	int numColumns = dispCount * numPixelsPerColumn;

	uint32_t color;

	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );
	
	// Grab the byte (bits) for the segments for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		if(code > -1)
		{
			int pixelIndex = 0; // current pixel index
		
			for( int segment = 0; segment < 7; segment++ )
			{
				bool on = ( bitRead( code, segment) == 1 );
				for ( int p = 0; p < dispPixelSegment; p++ )
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

			if ( dispUseDP )
			{
				bool on = ( bitRead( code, 7) == 1 );
				for ( int p = 0; p < dispPixelDp; p++ )
					pixels.setPixelColor( segmentsPixels[ 7 ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
			}
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

	// Grab the byte (bits) for the segments for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		
		if(code > -1)
		{
			for( int segment = 0; segment < 7; segment++ )
			{
				for ( int p = 0; p < dispPixelSegment; p++ )
				{
					bool on = ( bitRead( code, segment) == 1 );
					bool evenPixel = ( (p % 2) == 0 ) ? true : false;

					if ( index == 0 && !evenPixel )
						on = false;
					else if ( index == 1 && evenPixel )
						on = false;

					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
				}
			}

			if ( dispUseDP )
			{
				bool on = ( bitRead( code, 7) == 1 );
				for ( int p = 0; p < dispPixelDp; p++ )
				{
					bool evenPixel = ( (p % 2) == 0 ) ? true : false;

					if ( index == 0 && !evenPixel && dispPixelDp > 1 )
						on = false;
					else if ( index == 1 && evenPixel && dispPixelDp > 1 )
						on = false;

					pixels.setPixelColor( segmentsPixels[ 7 ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
				}
			}
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

	// Grab the byte (bits) for the segments for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		
		if(code > -1)
		{
			for( int segment = 0; segment < 7; segment++ )
			{
				for ( int p = 0; p < dispPixelSegment; p++ )
				{
					bool on = ( bitRead( code, segment) == 1 );
									
					if ( p != index )
						on = false;

					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
				}
			}

			if ( dispUseDP )
			{
				bool on = ( bitRead( code, 7) == 1 );
				for ( int p = 0; p < dispPixelDp; p++ )
					pixels.setPixelColor(segmentsPixels[ 7 ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
			}
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
	
	// Grab the byte (bits) for the segments for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		
		if(code > -1)
		{
			int colorStart = index;
				
			for( int segment = 0; segment < 7; segment++ )
			{
				bool on = ( bitRead( code, segment) == 1 );
				for ( int p = 0; p < dispPixelSegment; p++ )
				{
					color = Wheel( colorStart & 255 );
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
					colorStart+=(255/dispPixelSegment*7);
				}
			}

			if ( dispUseDP )
			{
				bool on = ( bitRead( code, 7) == 1 );
				for ( int p = 0; p < dispPixelDp; p++ )
					pixels.setPixelColor( segmentsPixels[ 7 ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
			}
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

	// Grab the byte (bits) for the segments for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		
		if(code > -1)
		{
			for( int segment = 0; segment < 7; segment++ )
			{
				bool on = ( bitRead( code, segment) == 1 );
				for ( int p = 0; p < dispPixelSegment; p++ )
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
			}

			if ( dispUseDP )
			{
				bool on = ( bitRead( code, 7) == 1 );
				for ( int p = 0; p < dispPixelDp; p++ )
					pixels.setPixelColor( segmentsPixels[ 7 ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
			}
		}
	}

	// we have finished setting all of the colors on each segment for this Neo7Segment, so lets turn on the pixels
	pixels.show();
}

void Neo7Segment::DisplayTextDigitColor( String text, uint32_t color[] )
{
	if ( !isReady )
		return;

	CheckToCacheBytes( text );

	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	// Grab the byte (bits) for the segments for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];

		if(code > -1)
		{
			for( int segment = 0; segment < 7; segment++ )
			{
				bool on = ( bitRead( code, segment) == 1 );
				for ( int p = 0; p < dispPixelSegment; p++ )
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color[s] : Color(0,0,0) );
			}

			if ( dispUseDP )
			{
				bool on = ( bitRead( code, 7) == 1 );
				for ( int p = 0; p < dispPixelDp; p++ )
					pixels.setPixelColor( segmentsPixels[ 7 ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color[s] : Color(0,0,0) );
			}
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

	// Grab the byte (bits) for the segments for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		byte code = cachedBytes[s];
		
		if(code > -1)
		{
			uint32_t cachedColor = colorH;

			// displaying mins, so work out new color
			if ( s >= text.length() - 2 )
				cachedColor = ( secs % 2 == 0 ) ? colorM2 : colorM;

			for( int segment = 0; segment < 7; segment++ )
			{
				bool on = ( bitRead( code, segment) == 1 );
				for ( int p = 0; p < dispPixelSegment; p++ )
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? cachedColor : Color(0,0,0) );
			}

			if (dispUseDP)
			{
				bool on = ( bitRead( code, 7) == 1 );
				for ( int p = 0; p < dispPixelDp; p++ )
					pixels.setPixelColor( segmentsPixels[ 7 ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? cachedColor : Color(0,0,0) );
			}
		}
	}

	// Blank extra digits
	if ( text.length() < dispCount )
	{
		for ( int s = lengthOfLoop; s < dispCount; s++ )
		{
			for( int segment = 0; segment < 7; segment++ )
			{
				for ( int p = 0; p < dispPixelSegment; p++ )
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), Color(0,0,0) );
			}

			if (dispUseDP)
			{
				for ( int p = 0; p < dispPixelDp; p++ )
					pixels.setPixelColor( segmentsPixels[ 7 ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), Color(0,0,0) );
			}
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

	int pixelIndex = ( index % (dispCount * (2*dispPixelSegment) ) );
	if ( pixelIndex > (dispCount * dispPixelSegment) - 1 )
	{
		pixelIndex = ( dispCount * ( 2*dispPixelSegment ) ) - (pixelIndex + 1 );
		isForward = false;
	}
	
	// Grab the byte (bits) for the segments for the character passed in
	for ( int s = 0; s < dispCount; s++ )
	{
		for( int segment = 0; segment < 7; segment++ )
		{
			for ( int p = 0; p < dispPixelSegment; p++ )
			{
				if ( segment != 6 )
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), Color(0,0,0) );
				else
				{
					uint8_t currentIndex = ( s * dispPixelSegment + p );
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

		if ( dispUseDP )
		{
			for ( int p = 0; p < dispPixelDp; p++ )
				pixels.setPixelColor( segmentsPixels[ 7 ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), Color(0,0,0) );
		}
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
				for ( int p = 0; p < dispPixelSegment; p++ )
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
			}
			else
			{
				for ( int p = 0; p < dispPixelSegment; p++ )
					pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), Color(0,0,0) );
			}
		}

		if ( dispUseDP )
		{
			for ( int p = 0; p < dispPixelDp; p++ )
				pixels.setPixelColor( segmentsPixels[ 7 ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), Color(0,0,0) );
		}
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

	if ( digit < 0 || digit > dispCount )
		return;

	CheckToCacheBytes( text );

	byte code = cachedBytes[0]; //No matter the length of string received, only first character with it's point will be displayed on requested digit

	if(code > -1)
	{
		for( int segment = 0; segment < 7; segment++ )
		{
			bool on = ( bitRead( code, segment) == 1 );
			for ( int p = 0; p < dispPixelSegment; p++ )
				pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( digit * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
		}

		if ( dispUseDP )
		{
			bool on = ( bitRead( code, 7) == 1 );
			for ( int p = 0; p < dispPixelDp; p++ )
				pixels.setPixelColor( segmentsPixels[ 7 ][ p ] + ( digit * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
		}
	}

	// we have finished setting all of the colors on each segment for this Neo7Segment, so lets turn on the pixels
	pixels.show();
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
		for ( int p = 0; p < dispPixelSegment; p++ )
			pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( digit * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );		
	}

	if ( dispUseDP )
	{
		bool on = ( bitRead( bits, 7) == 1 );
		for ( int p = 0; p < dispPixelDp; p++ )
			pixels.setPixelColor( segmentsPixels[ 7 ][ p ] + ( digit * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
	}

	// we have finished setting all of the colors on each segment for this Neo7Segment, so lets turn on the pixels
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
