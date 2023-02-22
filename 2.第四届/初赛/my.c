#include "STC15F2K60S2.H"

void Delay(unsigned char x)
{
	unsigned char y;
	while(x--)
		for(y = 120; y > 0; y--);
}

void InitHC138(unsigned char x)
{
	switch(x)
	{
		case 4:P2 = (P2 & 0x1F) | 0x80;break;
		case 5:P2 = (P2 & 0x1F) | 0xA0;break;
		case 6:P2 = (P2 & 0x1F) | 0xC0;break;
		case 7:P2 = (P2 & 0x1F) | 0xE0;break;
	}
}

void InputP0(unsigned char mod,unsigned char dat)
{
	InitHC138(mod);
	P0 = dat;
}