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
