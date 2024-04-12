#include "delay.h"

void delay_ms(unsigned int times)		//@12.000MHz
{
	unsigned char i, j;
	for (; times > 1; times --) {
		i = 2;
		j = 239;
		do {
			while (--j);
		} while (--i);
	}
}

