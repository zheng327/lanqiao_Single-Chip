#include "STC15F2K60S2.H"

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};
unsigned int COUNT_10MS = 0;
unsigned int FREQ = 0;

void Delay(unsigned char x)
{
	unsigned char y;
	while(x--)
	{
		for(y = 120; y > 0; y--);
	}
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

void Init_Timer(void)
{
	AUXR = 0x80;
	TMOD = 0x15;
	TH0 = 0;
	TL0 = 0;
	TF0 = 0;
	TH1 = (65536 - 10000) / 256;
	TL1 = (65536 - 10000) % 256;
	ET0 = 0;
	TR0 = 1;
	ET1 = 1;
	TR1 = 1;
	EA = 1;
}

void Display_FREQ(void)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		InputP0(6,0x01<<i);
		switch(i)
		{
			case 0:InputP0(7,duan[15]);break;
			case 1:InputP0(7,0xFF);break;
			case 2:InputP0(7,0xFF);break;
			case 3:if(FREQ >= 10000) InputP0(7,duan[FREQ/10000]);else InputP0(7,0xFF);break;
			case 4:if(FREQ >= 1000) InputP0(7,duan[FREQ%10000/1000]);else InputP0(7,0xFF);break;
			case 5:if(FREQ >= 100) InputP0(7,duan[FREQ%1000/100]);else InputP0(7,0xFF);break;
			case 6:if(FREQ >= 10) InputP0(7,duan[FREQ%100/10]);else InputP0(7,0xFF);break;
			case 7:if(FREQ >= 1) InputP0(7,duan[FREQ%10]);else InputP0(7,0xFF);break;
		}
		Delay(8);
		InputP0(7,0xFF);
	}
}

void main(void)
{
	InputP0(5,0x00);
	InputP0(4,0xFF);
	Init_Timer();
	while(1)
	{
		Display_FREQ();
	}
}

void T1_ser(void) interrupt 3
{
	TH1 = (65536 - 10000) / 256;
	TL1 = (65536 - 10000) % 256;
	if(++COUNT_10MS == 200)
	{
		FREQ = (TH0 << 8) | TL0;
		FREQ = FREQ / 2;
		TH0 = 0;
		TL0 = 0;
		COUNT_10MS = 0;
	}
}