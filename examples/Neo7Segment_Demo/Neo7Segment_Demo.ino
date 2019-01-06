//Displays different designs
//Changes feature every 5 seconds
#include <Neo7Segment.h>

#define PIXELS_DIGITS       5   // Number of digits
#define PIXELS_PER_SEGMENT  4   // Pixels per segment - If you want more than 10 pixels per segment, modify the Neo7Segment_Var.cpp
#define PIXELS_PER_POINT    1   // Pixels per decimal point - CANNOT be higher than PIXELS_PER_SEGMENT
#define PIXELS_PIN          4   // Pin number

// Initalise the display with 5 Neo7Segment boards, 4 LEDs per segment, 1 decimal point LED, connected to GPIO 4
Neo7Segment disp(PIXELS_DIGITS, PIXELS_PER_SEGMENT, PIXELS_PER_POINT, PIXELS_PIN);

int loopIndex = 0;
byte rainbowIndex = 0;
unsigned long nextRainbow = 0;
int displayFeature = 0;
long nextSwitch = millis();

void setup()
{
  Serial.begin(9600);
  delay(1000);

  // Start the display with a brightness value of 20
  disp.Begin(20);

  // Set the initial display feature to show as 0
  displayFeature = 0;
}

void loop()
{
  // Wait until the display is initialised before we try to show anything
  if ( !disp.IsReady() )
  {
    nextSwitch = millis();
    return;
  }

  // Switch what we are showing every 5 seconds
  if ( millis() > nextSwitch )
  {
    nextSwitch = millis() + 5000;
    displayFeature = ( displayFeature + 1 ) % 13;
  }

  // Display stuff on the Neo7Segment displays
  if ( nextRainbow < millis() )
    colorChangingSequences();
}

void colorChangingSequences()
{
    switch( displayFeature )
    {
      case 0:
        disp.DisplayTextColorCycle( "12345", rainbowIndex );
        nextRainbow = millis() + 10;
        rainbowIndex++;
        break;

      case 1:
        disp.DisplayTextVerticalRainbow("     ", disp.Color(255,0,0), disp.Color(0,0,255) );
        delay(250);
        disp.DisplayTextVerticalRainbow("C    ", disp.Color(255,0,0), disp.Color(0,0,255) );
        delay(250);
        disp.DisplayTextVerticalRainbow("CL   ", disp.Color(255,0,0), disp.Color(0,0,255) );
        delay(250);
        disp.DisplayTextVerticalRainbow("CLE  ", disp.Color(255,0,0), disp.Color(0,0,255) );
        delay(250);
        disp.DisplayTextVerticalRainbow("CLEA ", disp.Color(255,0,0), disp.Color(0,0,255) );
        delay(250);
        disp.DisplayTextVerticalRainbow("CLEAR", disp.Color(255,0,0), disp.Color(0,0,255) );
        delay(1250);
        disp.DisplayTextVerticalRainbow(" LEAR", disp.Color(255,0,0), disp.Color(0,0,255) );
        delay(250);
        disp.DisplayTextVerticalRainbow("  EAR", disp.Color(255,0,0), disp.Color(0,0,255) );
        delay(250);
        disp.DisplayTextVerticalRainbow("   AR", disp.Color(255,0,0), disp.Color(0,0,255) );
        delay(250);
        disp.DisplayTextVerticalRainbow("    R", disp.Color(255,0,0), disp.Color(0,0,255) );
        delay(250);
        disp.DisplayTextVerticalRainbow("     ", disp.Color(255,0,0), disp.Color(0,0,255) );
        delay(500);
        nextSwitch = 0;
        break;

      case 2:
          nextRainbow = millis() + 250;
          rainbowIndex+=5;
          loopIndex++;
          if ( loopIndex > 1 )
            loopIndex = 0;

          disp.ForceUppercase( true );
          disp.DisplayTextMarquee("SPOON", loopIndex, disp.Wheel( rainbowIndex & 255 ) );

        break;

      case 3:
        disp.ForceUppercase( false );
        disp.DisplayTextVerticalRainbow("ABCDE", disp.Wheel( rainbowIndex & 255 ) , disp.Wheel( ( rainbowIndex + 50 ) & 255 )  );
        nextRainbow = millis() + 10;
        rainbowIndex--;
        break;
        
      case 4:
        nextRainbow = millis() + 25;
        rainbowIndex+=5;
        loopIndex++;
        if ( loopIndex > ( PIXELS_PER_SEGMENT-1 ) )
          loopIndex = 0;

        disp.DisplayTextChaser("lolol", loopIndex, disp.Wheel( rainbowIndex & 255 ) );

        break;  

      case 5:
        rainbowIndex++;

        if ( rainbowIndex % 5 == 0 )
        {
          loopIndex++;
          if ( loopIndex >= disp.GetSpinAllLength() )
            loopIndex = 0;
        }
        
        disp.SetDigitSegments( 0, disp.GetSpinAllAtIndex( loopIndex ), disp.Color(0, 0, 50) );
        disp.SetDigitSegments( 1, disp.GetSpinAllAtIndex( loopIndex ), disp.Color(0, 0, 100) );
        disp.SetDigitSegments( 2, disp.GetSpinAllAtIndex( loopIndex ), disp.Color(0, 0, 150) );
        disp.SetDigitSegments( 3, disp.GetSpinAllAtIndex( loopIndex ), disp.Color(0, 0, 200) );
        disp.SetDigitSegments( 4, disp.GetSpinAllAtIndex( loopIndex ), disp.Color(0, 0, 250) );

        for (int i = 5; i<PIXELS_DIGITS; i++)
          disp.SetDigit(i, "", disp.Color(0, 0, 0));

        nextRainbow = millis() + 10;
        break;

      case 6:
        if (rainbowIndex > (2*PIXELS_PER_SEGMENT*PIXELS_DIGITS))
          rainbowIndex = 0;

        disp.DisplayKnightRider( rainbowIndex, disp.Color(255,0,255) );
        nextRainbow = millis() + 80;
        rainbowIndex++;
        break;

       case 7:
        disp.ForceUppercase( false );
        disp.DisplayTextHorizontalRainbow("67890", disp.Wheel( rainbowIndex & 255 ) , disp.Wheel( ( rainbowIndex + 150 ) & 255 )  );
        nextRainbow = millis() + 10;
        rainbowIndex--;
        break;

      case 8:
        disp.DisplayBorderAnimation(rainbowIndex, disp.Color(0, 0, 250)  );
        nextRainbow = millis() + 100;
        rainbowIndex--;
        break;

      
      case 9:
        disp.DisplayTime( 22, 16, (nextRainbow % 60), disp.Color(255, 200, 0), disp.Color(0, 0, 255) );
        nextRainbow = millis() + 500;
        break;

      case 10:
        disp.DisplayTextColorCycle( "0....0    ", rainbowIndex );
        nextRainbow = millis() + 10;
        rainbowIndex++;
        break;

      case 11: // Same as case #5, but allows to send complete string and change each digit's color
        uint32_t digitColors[PIXELS_DIGITS];
        digitColors[0] = disp.Color(255, 0, 0);
        digitColors[1] = disp.Color(127, 127, 0);
        digitColors[2] = disp.Color(0, 255, 0);
        digitColors[3] = disp.Color(0, 127, 127);
        digitColors[4] = disp.Color(0, 0, 255);

        disp.DisplayTextDigitColor("8.8.8.8.8.", digitColors);
        nextRainbow = millis() + 50;
        rainbowIndex++;
        break;

      case 12: // Same as case #5, but allows to send string character (only first will be used) and change only one digit at a time
        disp.SetDigit(0, "5", disp.Color(0, 0, 255));
        disp.SetDigit(1, "4", disp.Color(0, 127, 127));
        disp.SetDigit(2, "3", disp.Color(0, 255, 0));
        disp.SetDigit(3, "2", disp.Color(127, 127, 0));
        disp.SetDigit(4, "1", disp.Color(255, 0, 0));

        nextRainbow = millis() + 50;
        rainbowIndex++;
        break;

      default:
        displayFeature = 0;
        break;
    }
}
