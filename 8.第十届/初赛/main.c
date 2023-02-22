#include "STC15F2K60S2.H"
#include "PCF8591.H"
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

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

unsigned char Dis_Mod = 0;
unsigned char DAC_Mod = 0;
unsigned char LED_On = 1;
unsigned char Dis_On = 1;
unsigned int Freq = 0;
unsigned char Count_10ms = 0;
unsigned int Rb2_vol = 0;

void PCF8591_Work(void)
{
	unsigned int temp;
	Rb2_vol = PCF8591_AD(0x03);
	temp = (unsigned int)((double)Rb2_vol * 5 / 255 * 100 + 0.5);
	if(DAC_Mod == 0)
		PCF8591_DA(0x66);	//Êä³ö2V
	else
		PCF8591_DA(Rb2_vol);	//Êä³öRb2µçÑ¹
	Rb2_vol = temp;
}

void Init_Timer(void)
{
	AUXR = 0x80;
	TMOD = 0x15;
	TH0 = 0;
	TL0 = 0;
	TH1 = 0xD8;
	TL1 = 0xF0;
	ET0 = 0;
	ET1 = 1;
	TR0 = 1;
	TR1 = 1;
	EA = 1;
}

void Display(unsigned char mod)
{
	unsigned char i;
	if(Dis_On)
	{
		if(mod == 0)
		{
			for(i = 0; i < 8; i++)
			{
				InputP0(6,0x01<<i);
				switch(i)
				{
					case 0:InputP0(7,0xC1);break;
					case 1:InputP0(7,0xFF);break;
					case 2:InputP0(7,0xFF);break;
					case 3:InputP0(7,0xFF);break;
					case 4:InputP0(7,0xFF);break;
					case 5:InputP0(7,duan[Rb2_vol/100]&0x7F); break;
					case 6:InputP0(7,duan[Rb2_vol%100/10]); break;
					case 7:InputP0(7,duan[Rb2_vol%10]);break;
				}
				Delay(8);
				InputP0(7,0xFF);
			}
		}
		if(mod == 1)
		{
			for(i = 0; i < 8; i++)
			{
				InputP0(6,0x01<<i);
				switch(i)
				{
					case 0:InputP0(7,duan[0x0F]);break;
					case 1:InputP0(7,0xFF);break;
					case 2:InputP0(7,0xFF);break;
					case 3:if(Freq >= 10000) InputP0(7,duan[Freq/10000]); else InputP0(7,0xFF); break;
					case 4:if(Freq >= 1000) InputP0(7,duan[Freq%10000/1000]); else InputP0(7,0xFF); break;
					case 5:if(Freq >= 100) InputP0(7,duan[Freq%1000/100]); else InputP0(7,0xFF); break;
					case 6:if(Freq >= 10) InputP0(7,duan[Freq%100/10]); else InputP0(7,0xFF); break;
					case 7:InputP0(7,duan[Freq%10]);break;
				}
				Delay(8);
				InputP0(7,0xFF);
			}
		}
	}
}

void s4_scanf(void)
{
	if(S4 == 0)
	{
		Delay(10);
		if(S4 == 0)
		{
			while(S4 == 0)
				Display(Dis_Mod);
			Dis_Mod = (Dis_Mod + 1) % 2;
		}
	}
}

void s5_scanf(void)
{
	if(S5 == 0)
	{
		Delay(10);
		if(S5 == 0)
		{
			while(S5 == 0)
				Display(Dis_Mod);
			DAC_Mod = (DAC_Mod + 1) % 2;
		}
	}
}

void s6_scanf(void)
{
	if(S6 == 0)
	{
		Delay(10);
		if(S6 == 0)
		{
			while(S6 == 0)
				Display(Dis_Mod);
			LED_On = (LED_On + 1) % 2;
		}
	}
}

void s7_scanf(void)
{
	if(S7 == 0)
	{
		Delay(10);
		if(S7 == 0)
		{
			while(S7 == 0)
				Display(Dis_Mod);
			Dis_On = (Dis_On + 1) % 2;
		}
	}
}

void LED_work(void)
{
	InitHC138(4);
	P0 = P0 | 0xE0;
	if(LED_On)
	{
		//L1¡¢L2
		if(Dis_Mod == 0)
		{
			L1 = 0;
			L2 = 1;
		}
		else
		{
			L1 = 1;
			L2 = 0;
		}
		//L3
		if(Rb2_vol >= 350)
			L3 = 0;
		else if(Rb2_vol >= 250 && Rb2_vol < 350)
			L3 = 1;
		else if(Rb2_vol >= 150 && Rb2_vol < 250)
			L3 = 0;
		else if(Rb2_vol < 150)
			L3 = 1;
		//L4
		if(Freq >= 10000)
			L4 = 0;
		else if(Freq >= 5000 && Freq < 10000)
			L4 = 1;
		else if(Freq >= 1000 && Freq < 5000)
			L4 = 0;
		else if(Freq < 1000)
			L4 = 1;
		//L5
		if(DAC_Mod == 0)
			L5 = 1;
		else 
			L5 = 0;
	}
	else
		P0 = 0xFF;
}

void main(void)
{
	InputP0(5,0x00);
	InputP0(4,0xFF);
	Init_Timer();
	while(1)
	{
		PCF8591_Work();
		s4_scanf();
		s5_scanf();
		s6_scanf();
		s7_scanf();
		Display(Dis_Mod);
		LED_work();
	}
}

void T1_ser(void) interrupt 3
{
	TH1 = (65536 - 10000) / 256;
	TL1 = (65536 - 10000) % 256;
	if(++Count_10ms == 100)
	{
		Freq = (TH0 << 8) | TL0;
		Count_10ms = 0;
		TH0 = 0;
		TL0 = 0;
	}
}







