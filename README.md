# Neo7Segment Display Library
An API that lets you display numbers and letters on Neo7Segment RGB LED displays that are available from [tindie](https://www.tindie.com/products/seonr/neo7segment-display/)

You can find the Open Source hardware PCB design and Gerber files [here](https://github.com/UnexpectedMaker/Neo7Segment-Design-Files)

## Installing and Use
The downloaded code should be added to your arduino IDE libraries folder, and the library requires the [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) for it to function.

To include the Neo7Segment library in your project:

    #include <Neo7Segment.h>

You can then initialise the display with the following line that includes the number of Neo7Segment digits and the GPIO to control them:

    Neo7Segment disp( 5, 4 );

You can also initialise the display with custom pixels per segment and pixel per decimal point

	#define PIXELS_DIGITS       5   // Number of digits
	#define PIXELS_PER_SEGMENT  4   // Pixels per segment - If you want more than 10 pixels per segment, modify the Neo7Segment.cpp
	#define PIXELS_PER_POINT    1   // Pixels per decimal point - CANNOT be higher than PIXELS_PER_SEGMENT
	#define PIXELS_PIN          4   // Pin number

	// Initalise the display with 5 Neo7Segment boards, 4 LEDs per segment, 1 decimal point LED, connected to GPIO 4
	Neo7Segment disp(PIXELS_DIGITS, PIXELS_PER_SEGMENT, PIXELS_PER_POINT, PIXELS_PIN);

You then start the display with the bebin method, passing the brightness:

    disp.Begin(20);

The you simply pass the display the String you would like displayed and a color:

    disp.DisplayText( "1234", disp.Color( 255,0,0) );
    
# Buy me a coffee?
I love making and designing projects but sharing open source projects takes a lot of thought and time. I do it because I think it’s important to share knowledge and give back to the community like many have done before me.

If you find this project useful or want to see more open source projects like it, please consider a donation to say thanks!

[![paypal](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/YLVGbhJP0)

# Unexpected Maker
http://twitter.com/unexpectedmaker

https://www.facebook.com/unexpectedmaker/

https://www.instagram.com/unexpectedmaker/

https://www.tindie.com/stores/seonr/

