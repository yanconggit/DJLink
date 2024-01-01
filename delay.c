//#include "stc8g.h"
#include "intrins.h"

void Delayxms(int x_ms)		//@33.1776MHz
{
	while(x_ms--)
	{
		unsigned char i, j;

		_nop_();
		_nop_();
		i = 44;
		j = 19;
		do
		{
			while (--j);
		} while (--i);
	}	
}


void Delayx10us(int x_10us)		//@33.1776MHz
{
	while(x_10us--)
	{
		unsigned char i;

		_nop_();
		_nop_();
		i = 108;
		while (--i);
	}	
}

void Delayxus(int x_us)		//@33.1776MHz
{
	while(x_us--)
	{
		unsigned char i;

		i = 9;
		while (--i);
	}
}

