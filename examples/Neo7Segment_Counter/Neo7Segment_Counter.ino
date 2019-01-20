//Displays all digits with the same number counting from 0 to 9. Odd numbers have decimal point ON, others OFF.
//Push button on pin 5 to control color changing
#include <Neo7Segment.h>

#define PIXELS_DIGITS       5   // Number of digits
#define PIXELS_PER_SEGMENT  4   // If you want more than 10 pixels per segment, modify the Neo7Segment_Var.cpp
#define PIXELS_PER_POINT    1   // CANNOT be higher than PIXELS_PER_SEGMENT
#define PIXELS_PIN          4   // Pin number

// Initalise the display with 5 Neo7Segment boards, 4 LEDs per segment, 1 decimal point LED, connected to GPIO 4
Neo7Segment disp(PIXELS_DIGITS, PIXELS_PER_SEGMENT, PIXELS_PER_POINT, PIXELS_PIN);

int loopIndex               = 0;
byte rainbowIndex           = 0;
unsigned long nextRainbow   = 0;
int displayFeature          = 0;
long nextCount              = millis();

//Button pin define
#define buttonPin           5

bool changeColor            = false;
bool buttonActive           = false;
int digitDisplay            = 0;

String text                 = "";

void setup()
{
  Serial.begin(9600);
  delay(1000);

  pinMode(buttonPin, INPUT);

  // Start the display with a brightness value of 20
  disp.Begin(20);

  // Set the initial display feature to show as 0
  displayFeature = 0;
}

void loop()
{
  // Wait until the display is initialised before we try to show anything
  if ( !disp.IsReady() )
    return;

  readButtonPush();

  if ( millis() > nextCount )
  {
    digitDisplay += 1;
    nextCount = millis() + 1000;
  }

  if (digitDisplay > 9)
    digitDisplay = 0;

  String dot = ( ( digitDisplay%2 ) == 0 ) ? "." : "";
  String digit = String( digitDisplay );

  text = "";
  for ( int i = 0; i < PIXELS_DIGITS; i++ )
    text = text + digit + dot;

  // Switch sequence when pressing on button
  if ( changeColor )
    displayFeature = (displayFeature + 1) % 13;

  // Display stuff on the Neo7Segment displays
  if (nextRainbow < millis() || changeColor)
    colorChangingSequences();

  changeColor = false;
}

void colorChangingSequences()
{
  switch(displayFeature)
  {
    case 0:
      disp.DisplayTextColorCycle( text, rainbowIndex );
      nextRainbow = millis() + 10;
      rainbowIndex++;
      break;

    case 1:
      disp.DisplayTextVerticalRainbow( text, disp.Color(255,0,0), disp.Color(0,0,255) );
      nextRainbow = millis() + 10;
      break;

    case 2:
        nextRainbow = millis() + 250;
        rainbowIndex+=5;
        loopIndex++;
        if (loopIndex > 1)
          loopIndex = 0;

        disp.ForceUppercase( true );
        disp.DisplayTextMarquee( text, loopIndex, disp.Wheel( rainbowIndex & 255 ) );

      break;

    case 3:
      disp.ForceUppercase( false );
      disp.DisplayTextVerticalRainbow( text, disp.Wheel( rainbowIndex & 255 ) , disp.Wheel( ( rainbowIndex + 50 ) & 255 ) );
      nextRainbow = millis() + 10;
      rainbowIndex--;
      break;

    case 4:
      nextRainbow = millis() + 50;
      rainbowIndex+=5;
      loopIndex++;
      if ( loopIndex > ( PIXELS_PER_SEGMENT-1 ) )
        loopIndex = 0;

      disp.DisplayTextChaser( text, loopIndex, disp.Wheel( rainbowIndex & 255 ) );

      break;

    case 5:
      rainbowIndex++;

      if (rainbowIndex % 5 == 0)
      {
        loopIndex++;
        if (loopIndex >= disp.GetSpinAllLength())
          loopIndex = 0;
      }

      disp.SetDigitSegments(0, disp.GetSpinAllAtIndex(loopIndex), disp.Color(0, 0, 50));
      disp.SetDigitSegments(1, disp.GetSpinAllAtIndex(loopIndex), disp.Color(0, 0, 100));
      disp.SetDigitSegments(2, disp.GetSpinAllAtIndex(loopIndex), disp.Color(0, 0, 150));
      disp.SetDigitSegments(3, disp.GetSpinAllAtIndex(loopIndex), disp.Color(0, 0, 200));
      disp.SetDigitSegments(4, disp.GetSpinAllAtIndex(loopIndex), disp.Color(0, 0, 250));

      for (int i = 5; i<PIXELS_DIGITS; i++)
        disp.SetDigit(i, "", disp.Color(0, 0, 0));

      nextRainbow = millis() + 10;
      break;

    case 6:
      if (rainbowIndex > (2*PIXELS_PER_SEGMENT*PIXELS_DIGITS))
        rainbowIndex = 0;

      disp.DisplayKnightRider(rainbowIndex, disp.Color(255,0,255));
      nextRainbow = millis() + 10;
      rainbowIndex++;
      break;

     case 7:
      disp.ForceUppercase(false);
      disp.DisplayTextHorizontalRainbow(text, disp.Wheel(rainbowIndex & 255) , disp.Wheel((rainbowIndex + 150) & 255));
      nextRainbow = millis() + 50;
      rainbowIndex--;
      break;

    case 8://
      disp.DisplayBorderAnimation(rainbowIndex, disp.Color(0, 0, 250));
      nextRainbow = millis() + 100;
      rainbowIndex--;
      break;

    case 9:
      disp.DisplayTime((digitDisplay*10+digitDisplay), (digitDisplay*10+digitDisplay), digitDisplay, disp.Color(255, 200, 0), disp.Color(0, 0, 255));
      nextRainbow = millis() + 500;
      break;

    case 10:
      disp.DisplayTextColorCycle(text, rainbowIndex);
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

      for (int i = 5; i<PIXELS_DIGITS; i++)
        digitColors[i] = disp.Color(0, 0, 0);

      disp.DisplayTextDigitColor(text, digitColors);
      nextRainbow = millis() + 50;
      rainbowIndex++;
      break;

    case 12: // Same as case #5, but allows to send string character (only first will be used) and change only one digit at a time
      disp.SetDigit(0, text, disp.Color(0, 0, 255));
      disp.SetDigit(1, text, disp.Color(0, 127, 127));
      disp.SetDigit(2, text, disp.Color(0, 255, 0));
      disp.SetDigit(3, text, disp.Color(127, 127, 0));
      disp.SetDigit(4, text, disp.Color(255, 0, 0));

      nextRainbow = millis() + 50;
      rainbowIndex++;
      break;

    default:
      displayFeature = 0;
      break;
  }
}

void readButtonPush()
{
  bool buttonPressed = !digitalRead(buttonPin);
  if (buttonPressed)
  {
    if (!buttonActive)
      buttonActive = true;
  }
  else
  {
    if (buttonActive)
      changeColor = true;

    buttonActive = false;
  }
}
