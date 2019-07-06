*
Graphic Libs:
https://github.com/adafruit/Adafruit-GFX-Library
https://github.com/adafruit/Adafruit_SSD1306
 
Utilities header
 
Author: Kris Kasprzak (c)
 
Revision
Rev        Date                      Reason
1              12-2-2015            Initial creation -- added Format()
 
*/
 
 

 
#define SSD1306_BLACK  0
#define SSD1306_WHITE  1
#define SSD1306_INVERSE  2



// http://www.barth-dev.de/online/rgb565-color-picker/

#define LTBLUE		0xB6DF    
#define LTTEAL		0xBF5F  
#define LTGREEN       	0xBFF7
#define LTCYAN		0xC7FF
#define LTRED           0xFD34
#define LTMAGENTA       0xFD5F
#define LTYELLOW        0xFFF8
#define LTORANGE        0xFE73
#define LTPINK          0xFDDF
#define LTPURPLE	0xCCFF
#define LTGREY          0xE71C  

#define BLUE            0x001F 
#define TEAL		0x0438    
#define GREEN           0x07E0     
#define CYAN         	0x07FF      
#define RED          	0xF800      
#define MAGENTA      	0xF81F      
#define YELLOW       	0xFFE0      
#define ORANGE       	0xFD20          
#define PINK         	0xF81F
#define PURPLE		0x801F
#define GREY       	0xC618
#define WHITE        	0xFFFF 
#define BLACK        	0x0000 
    
#define DKBLUE       	0x000D
#define DKTEAL		0x020C
#define DKGREEN      	0x03E0      
#define DKCYAN       	0x03EF      
#define DKRED        	0x6000
#define DKMAGENTA      	0x8008
#define DKYELLOW        0x8400
#define DKORANGE       	0x8200         
#define DKPINK         	0x9009
#define DKPURPLE     	0x4010
#define DKGREY       	0x4A49


/*
 
 
FORMAT(data, number of digits (excluding decimal), number of decimal points)
 
Format(MyData,2,3));
 
input 3.9876656
result _2.988 (preceding space)
 
*/

 
inline String Format(double val, int dec, int dig ) {
  int addpad = 0;
  char sbuf[20];
  String condata = (dtostrf(val, dec, dig, sbuf));

  int slen = condata.length();
  for ( addpad = 1; addpad <= dec + dig - slen+1; addpad++) {
    condata = " " + condata;
  }
  return (condata);

}


/*

function to get temperature from a thermistor
10K NTC type

pass in the pin for the thermistor and get back the temp
need to improve to ge F K or C

*/

inline double GetTemp(int pin) {
  int i;
  double a1, b1, c1, d1, r2, r1, volts, tempC, tempF, tempK;
  a1 = 3.354016E-03 ;
  b1 = 2.569850E-04 ;
  c1 = 2.620131E-06 ;
  d1 = 6.383091E-08 ;

  // read the temp
  volts = 0;
  for ( i = 0; i < 10; i++) {
    volts = volts +  analogRead(pin);
  }
  volts = volts / 10;
  volts = volts / 204.6;

  // voltage divider calculation
  // vo = 5 * r2 /(r1+r2)
  // solve for r2
  // get the exact value for voltage divider r1
  r1 = 9940;
  r2 = ( volts * r1) / (5 - volts);

  //equation from data sheet
  tempK = 1.0 / (a1 + (b1 * (log(r2 / 10000.0))) + (c1 * pow(log(r2 / 10000.0), 2)) + (d1 * pow(log(r2 / 10000.0), 3)));
  tempC  = ((tempK - 273.15) );
  tempF  = (tempC * 1.8000) + 32.00;
  return tempF;

}


String FormatTime(unsigned long t)
{
// t is time in seconds = millis()/1000;
static char str[12];
long h = t / 3600;
t = t % 3600;
int m = t / 60;
int s = t % 60;
sprintf(str, "%02ld:%02d:%02d", h, m, s);
return str;
}

Utilities.h
Displaying Utilities.h.
