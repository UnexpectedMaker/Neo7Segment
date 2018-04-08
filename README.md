# Neo7Segment Display Library
An API that lets you display numbers and letters on Neo7Segment RGB LED displays that are available from [tindie](https://www.tindie.com/products/seonr/neo7segment-display/)

## Installing and Use
The downloaded code should be added to your arduino IDE libraries folder, and the library requires the [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) for it to function.

To include the Neo7Segment library in your project:
    #include <Neo7Segment.h>

You can then initialise the display with the following line that includes the number of Neo7Segment digits and the GPIO to control them:
    Neo7Segment disp( 5, 4 );

You then start the display with the bebin method, passing the brightness:
    disp.Begin(20);

The you simply pass the display the String you would like displayed and a color:
    disp.DisplayText( "1234", disp.Color( 255,0,0) );
