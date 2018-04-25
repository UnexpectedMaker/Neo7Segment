#include <Neo7Segment.h>

// Initalise the display with 5 Neo7Segment boards connected to GPIO 4
Neo7Segment disp( 5, 4 );

int loopIndex = 0;
byte rainbowIndex = 0;
unsigned long nextRainbow = 0;
int displayFeature = 0;
int nextSwitch = 10000;

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
    displayFeature = ( displayFeature + 1 ) % 11;
  }

  // Display stuff on the Neo7Segment displays
  if ( nextRainbow < millis() )
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
        if ( loopIndex > 3 )
          loopIndex = 0;

        disp.DisplayTextChaser("lolol", loopIndex, disp.Wheel( rainbowIndex & 255 ) );

        break;  

      case 5:
        rainbowIndex++;
      
        if ( rainbowIndex % 5 == 0 )
        {
          loopIndex++;
          if ( loopIndex == disp.GetSpinAllLength() )
            loopIndex = 0;
        }
        
        disp.SetDigitSegments( 0, disp.GetSpinAllAtIndex( loopIndex ), disp.Color(0, 0, 50) );
        disp.SetDigitSegments( 1, disp.GetSpinAllAtIndex( loopIndex ), disp.Color(0, 0, 100) );
        disp.SetDigitSegments( 2, disp.GetSpinAllAtIndex( loopIndex ), disp.Color(0, 0, 150) );
        disp.SetDigitSegments( 3, disp.GetSpinAllAtIndex( loopIndex ), disp.Color(0, 0, 200) );
        disp.SetDigitSegments( 4, disp.GetSpinAllAtIndex( loopIndex ), disp.Color(0, 0, 250) );
        break;

      case 6:
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
        disp.DisplayTextColorCycle( "0....0", rainbowIndex );
        nextRainbow = millis() + 10;
        rainbowIndex++;
        break;

    }
  }
}

