#include "STC15F2K60S2.H"
#include "iic.h"
#include "my.h"

sbit S4 = P3^3;
sbit S5 = P3^2;
sbit S6 = P3^1;
sbit S7 = P3^0;

sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;
sbit L5 = P0^4;
sbit L6 = P0^5;
sbit L7 = P0^6;
sbit L8 = P0^7;

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};
unsigned int Count_10ms = 0;
bit Count_Flag = 0;
unsigned int Count_long = 0;
long Freq = 0;
long Freq_Cache = 0;
long T_1us = 0;
unsigned int Vol1 = 0,Vol3 = 0;
unsigned int Vol3_Cache = 0;
unsigned char Channel_Set = 1;
unsigned char Dis_Mod = 0;
bit LED_Work_Flag = 1;

void Init_T0(void)
{
	TMOD = 0x15;
	TH0 = 0x00;
	TL0 = 0x00;
	TH1 = (65536 - 10000) / 256;	//10ms
	TL1 = (65536 - 10000) % 256;
	ET0 = 0;
	ET1 = 1;
	EA = 1;
	TR0 = 1;
	TR1 = 1;
}

void Display(void)
{
	unsigned char i;
	if(Dis_Mod == 0)
	{
		for(i = 0; i < 8; i++)
		{
			InputP0(6,0x01<<i);
			switch(i)
			{
				case 0:InputP0(7,duan[0x0F]);break;
				case 1:if(Freq >= 1000000) InputP0(7,duan[Freq/1000000]); break;
				case 2:if(Freq >= 100000) InputP0(7,duan[Freq%1000000/100000]); break;
				case 3:if(Freq >= 10000) InputP0(7,duan[Freq%100000/10000]); break;
				case 4:if(Freq >= 1000) InputP0(7,duan[Freq%10000/1000]); break;
				case 5:if(Freq >= 100) InputP0(7,duan[Freq%1000/100]); break;
				case 6:if(Freq >= 10) InputP0(7,duan[Freq%100/10]); break;
				case 7:InputP0(7,duan[Freq%10]);break;
			}
			Delay(10);
			InputP0(7,0xFF);
		}
	}
	
	if(Dis_Mod == 1)
	{
		for(i = 0; i < 8; i++)
		{
			InputP0(6,0x01<<i);
			switch(i)
			{
				case 0:InputP0(7,0xC8);break;
				case 1:if(T_1us >= 1000000) InputP0(7,duan[T_1us/1000000]); break;
				case 2:if(T_1us >= 100000) InputP0(7,duan[T_1us%1000000/100000]); break;
				case 3:if(T_1us >= 10000) InputP0(7,duan[T_1us%100000/10000]); break;
				case 4:if(T_1us >= 1000) InputP0(7,duan[T_1us%10000/1000]); break;
				case 5:if(T_1us >= 100) InputP0(7,duan[T_1us%1000/100]); break;
				case 6:if(T_1us >= 10) InputP0(7,duan[T_1us%100/10]); break;
				case 7:InputP0(7,duan[T_1us%10]);break;
			}
			Delay(10);
			InputP0(7,0xFF);
		}
	}
	
	if(Dis_Mod == 2)
	{
		for(i = 0; i < 8; i++)
		{
			InputP0(6,0x01<<i);
			switch(i)
			{
				case 0:InputP0(7,0xC1);break;
				case 1:InputP0(7,0xBF);break;
				case 2:InputP0(7,duan[Channel_Set]);break;
				case 3:InputP0(7,0xFF);break;
				case 4:InputP0(7,0xFF);break;
				case 5:if(Channel_Set == 1) InputP0(7,duan[Vol1/100]&0x7F);else InputP0(7,duan[Vol3/100]&0x7F);break;
				case 6:if(Channel_Set == 1) InputP0(7,duan[Vol1%100/10]);else InputP0(7,duan[Vol3%100/10]);break;
				case 7:if(Channel_Set == 1) InputP0(7,duan[Vol1%10]);else InputP0(7,duan[Vol3%10]);break;
			}
			Delay(10);
			InputP0(7,0xFF);
		}
	}
}

void Key_Scanf(void)
{
	if(S4 == 0)
	{
		Delay(10);
		if(S4 == 0)
		{
			while(S4 == 0)
				Display();
			Dis_Mod = (Dis_Mod + 1) % 3;
		}
	}
	if(S5 == 0 && Dis_Mod == 2)
	{
		Delay(10);
		if(S5 == 0)
		{
			while(S5 == 0)
				Display();
			if(Channel_Set == 1)
				Channel_Set = 3;
			else if(Channel_Set == 3)
				Channel_Set = 1;
		}
	}
	if(S6 == 0)
	{
		Delay(10);
		if(S6 == 0)
		{
			while(S6 == 0)
				Display();
			Vol3_Cache = Vol3;
		}
	}
	if(S7 == 0)
	{
		Delay(10);
		if(S7 == 0)
		{
			Count_Flag = 1;
			while(S7 == 0)
				Display();
			if(Count_long >= 100)
				LED_Work_Flag = ~LED_Work_Flag;
			Count_Flag = 0;
			Freq_Cache = Freq;
		}
	}
}

void work(void)
{
	if(Channel_Set == 1)
	{
		Vol1 = (unsigned int)((float)ADC_PCF8591(1) * 5 / 255 * 100 + 0.5);
	}
	if(Channel_Set == 3)
	{
		Vol3 = (unsigned int)((float)ADC_PCF8591(3) * 5 / 255 * 100 + 0.5);
	}
	if(LED_Work_Flag == 1)
	{
		InitHC138(4);
		if(Vol3 > Vol3_Cache)
			L1 = 0;
		if(Freq > Freq_Cache)
			L2 = 0;
		if(Dis_Mod == 0)
			L3 = 0;
		else  if(Dis_Mod == 1)
			L4 = 0;
		else if(Dis_Mod == 2)
			L5 = 0;
		L6 = 1;L7 = 1;L8 = 1;
	}
	else
		InputP0(4,0xFF);
}

void 	main(void)
{
	InputP0(5,0x00);
	InputP0(4,0xFF);
	Init_T0();
	while(1)
	{
		work();
		Key_Scanf();
		Display();
	}
}

void T1_ser(void) interrupt 3
{
	TH1 = (65536 - 10000) / 256;
	TL1 = (65536 - 10000) % 256;
	if(Count_Flag == 1)
		++Count_long;
	if(Count_Flag == 0)
		Count_long = 0;
	if(++Count_10ms == 200)
	{
		Freq = (TH0 * 256 + TL0) / 2;
		T_1us = 1000000 / Freq;
		TH0 = 0x00;
		TL0 = 0x00;
		Count_10ms = 0;
	}
}