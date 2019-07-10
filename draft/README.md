Disk read_ahead: 

The read_ahead parameter speeds up the disk read operation by prefetching data and loading it to random access memory. 

Setting up a relatively higher value for read_ahead will beneit clients performing sequential read operations. 

You can check the current  read_ahead value using this command:
```
# cat /sys/block/vda/queue/read_ahead_kb
```
To set read_ahead to a higher value, execute the following command:

```
# echo "8192">/sys/block/vda/queue/read_ahead_kb
```

Typically, customized read_ahead settings are used on Ceph clients that use RBD. You should change read_ahead for all the RBDs mapped to this host; also make sure to use the correct device path name.

Notes:

This short demo shows how to use a 0.96 inch OLED from Balance world Inc. This Yellow and Blue unit is on the I2C Serial buss and works with Arduino UNO, or Mega boards. Shown are voltage measurements and a performance test using a small bar graph.

```
#include <avr/io.h>
void setup(){
DDRB |= (1 << PB3);			//replaces pinMode(PB3, OUTPUT);
DDRB |= (1 << PB4);  			//replaces pinMode(PB4, OUTPUT);
}
void loop()
{
 delay(random(600000, 900000));
 byte state = random(0, 2);
 switch(state)
  {
   case 0:
   PORTB |= (1 << PB3);			//replaces digitalWrite(PB3, HIGH);
   delay(20);
   PORTB &= ~(1 << PB3);		//replaces digitalWrite(PB3, LOW);
   break;
   case 1:
   PORTB |= (1 << PB4);			//replaces digitalWrite(PB4, HIGH);
   delay(20);
   PORTB &= ~(1 << PB4);		//replaces digitalWrite(PB4, LOW);
   break;
  }
}
```
* Attiny85

https://www.instructables.com/id/Arduino-Nano-as-Attiny-85-programmer-and-5-LED-POV/
https://www.youtube.com/watch?v=MirP8WvzXuA


Once the clock on the Attiny has been set to ‘external’ without the presence of an external clock, Arduino will produce the following error on the next attempt to burn a bootloader:

```
    avrdude: Yikes! Invalid device signature.
    Double check connections and try again, or use -F to override this check.
    Error while burning bootloader.
```

Fixing this should be as simple as fitting a crystal oscillator between the pins 2 and 3 of the Arduino in any orientation (a crystal oscillator does not have a polarity). The frequency of the crystal oscillator should not matter, although I did it with a 16Mhz oscillator. Note that the crystal oscillator needs to be fitted to the physical pins 2&3 (the digital pins in blue and the analog pins in red have a somewhat confusing naming convention on the Attiny).

Now, all that’s left to do is to select the correct ‘internal’ clock (e.g 8Mhz) and click ‘Burn Bootloader’ in Arduino. Once Arduino is done with this, the crystal can be removed again.

Note: Two 22pf capacitors can be fitted between both ends of the oscillator and ground, but this should not be necessary for a quick fix.



