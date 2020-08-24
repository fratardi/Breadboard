
#include <wiringPi.h>
#include <stdio.h>

/* DEFINING PINS
** serial_data pin
** memory clock pin
** shift register clock pin
*/

#define   SERIAL_DATA	0x01
#define   MEM_CLK_PIN	0x01
#define   SHIFT_REG_CLK	0x02

/*
**	init ti 74hc595 bit shift register procedure
*/

void bb_init(void)
{
	pinMode(SERIAL_DATA, OUTPUT);	//init serial data input 
	pinMode(MEM_CLK_PIN, OUTPUT);	//init memory clock input
	pinMode(SHIFT_REG_CLK, OUTPUT);	//init shift register clock input
	
	/*	send 0 signal to each pin for safety below*/
	digitalWrite(SERIAL_DATA, 0x00);
	digitalWrite(MEM_CLK_PIN, 0x00);
	digitalWrite(MEM_CLK_PIN, 0x00);
}

/*
**	fill ti 74hc595 bit shift register procedure
*/

void bb_fill_frame(unsigned char frame)
{
	unsigned char i = 0;

	while(i < 8)
	{
		digitalWrite(SERIAL_DATA, 0x80 & (frame << i));		//only write the left-most frame bit to the bit shift register
		digitalWrite(SHIFT_REG_CLK, 0x01);						// throw clock to shift reg one
		delay(1); 												// Delay is mandatory
		digitalWrite(SHIFT_REG_CLK, 0x00);						// end clock cycle
		i++;
	}
}

/*
**	Throw frame to matrix
*/

void bb_frame_out()
{
	digitalWrite(MEM_CLK_PIN, 0x01);	
	delay(1); // Cycle is Mandatory 
	digitalWrite(MEM_CLK_PIN, 0x00);
}

/*
** Fill two frames then flush content to matrix
*/

void	bb_print(unsigned char frame_1, unsigned char frame_2)
{
	bb_fill_frame(frame_1);
	bb_fill_frame(frame_2);	
	bb_frame_out();
}

void	bb_bubble(void)
{
	unsigned char x;
	unsigned char y;

	x = 0;
	y = 0;	
	while(1)
	{
		while( y != 0)
		{		
			while(x < 0xFF)
			{
				bb_print(x , y);
				printf("    x = [%#x] y = [%#x]\n " , x  , y );
				delay(1);
				x++;
			}			
			x = 0;
			y--;
		}
		y = 0XFF;		
	
		bb_print(0x00 , 0x05	);
	
	}
}

/*
** 0 for success 1 for failure 
*/

int main(void)
{
	if(0 < wiringPiSetup())
	{
		printf("Setup failed");
		return (1); 
	}
	bb_init();
	bb_bubble();
	return(0);
}