#include "STC15F2K60S2.H"
#include "PCF8591.H"
#include "AT24C02.H"
#include "my.h"

sbit S4 = P3^3;
sbit S5 = P3^2;
sbit S6 = P3^1;
sbit S7 = P3^0;

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};
unsigned char Rb2_Vol = 0;
unsigned char Pwm_Duty = 25;
unsigned char LED_Level = 1;
unsigned char Count_100us = 0;
unsigned char Count_10ms = 0;
unsigned char Count_100ms = 0;
unsigned char Dcount_100ms = 0;
unsigned char Dtime_100ms = 4;
unsigned char Dis_Mod = 0;
unsigned char Dis_Flag = 0;
unsigned char LED_Mod = 1;
unsigned char LED_Num = 0;
unsigned char LED_Flag = 0;
unsigned char LED_Temp = 0;
bit FFlag = 1;

void Init_Timer(void)
{
	TMOD = 0x11;
	AUXR = 0x00;
	TH0 = (65536 - 100) / 256;
	TL0 = (65536 - 100) % 256;
	TH1 = (65536 - 10000) / 256;
	TL1 = (65536 - 10000) % 256;
	TR0 = 0;
	TR1 = 0;
	EA = 1;
	ET0 = 1;
	ET1 = 1;
}

void Light_Contral(void)
{
	Rb2_Vol = AD_PCF8591(0x03);
	if(Rb2_Vol >= 0 && Rb2_Vol < 64)
	{
		LED_Level = 1;
		Pwm_Duty = 25;
	}
	else if(Rb2_Vol >= 64 && Rb2_Vol < 128)
	{
		LED_Level = 2;
		Pwm_Duty = 50;
	}
	else if(Rb2_Vol >= 128 && Rb2_Vol < 191)
	{
		LED_Level = 3;
		Pwm_Duty = 75;
	}
	else if(Rb2_Vol >= 191 && Rb2_Vol <= 255)
	{
		LED_Level = 4;
		Pwm_Duty = 100;
	}
}

void LED_Work(void)
{
	unsigned char num_temp = LED_Num;
	InputP0(4,LED_Temp);
	if(LED_Mod == 1 && LED_Flag)
	{
		InputP0(4,~(0x01<<LED_Num));
	}
	else if(LED_Mod == 2 && LED_Flag)
	{
		InputP0(4,~(0x80>>LED_Num));
	}
	else if(LED_Mod == 3 && LED_Flag)
	{
		num_temp = num_temp % 4;
		InputP0(4,(~(0x01<<num_temp)) & (~(0x80>>num_temp)));
	}
	else if(LED_Mod == 4 && LED_Flag)
	{
		num_temp = num_temp % 4;
		InputP0(4,(~(0x10<<num_temp)) & (~(0x08>>num_temp)));
	}
	else if(LED_Flag == 0)
	{
		InputP0(4,0xFF);
	}
	LED_Temp = P0;
}

void Display(void)
{
	unsigned char i;
	if(Dis_Mod == 1)
	{
		for(i = 0; i < 8; i++)
		{
			InputP0(6,0x01<<i);
			switch(i)
			{
				case 0:InputP0(7,0xBF);break;
				case 1:if(Dis_Flag) InputP0(7,duan[LED_Mod]);break;
				case 2:InputP0(7,0xBF);break;
				case 3:InputP0(7,0xFF);break;
				case 4:if(Dtime_100ms >= 10) InputP0(7,duan[1]);break;
				case 5:InputP0(7,duan[Dtime_100ms%10]);break;
				case 6:InputP0(7,duan[0]);break;
				case 7:InputP0(7,duan[0]);break;
			}
			Delay(8);
			InputP0(7,0xFF);
			LED_Work();
		}
	}
	else if(Dis_Mod == 2)
	{
		for(i = 0; i < 8; i++)
		{
			InputP0(6,0x01<<i);
			switch(i)
			{
				case 0:InputP0(7,0xBF);break;
				case 1:InputP0(7,duan[LED_Mod]);break;
				case 2:InputP0(7,0xBF);break;
				case 3:InputP0(7,0xFF);break;
				case 4:if(Dtime_100ms >= 10 && Dis_Flag) InputP0(7,duan[1]);break;
				case 5:if(Dis_Flag) InputP0(7,duan[Dtime_100ms%10]);break;
				case 6:if(Dis_Flag) InputP0(7,duan[0]);break;
				case 7:if(Dis_Flag) InputP0(7,duan[0]);break;
			}
			Delay(8);
			InputP0(7,0xFF);
			LED_Work();
		}
	}
}

void Display_Level(void)
{
	InputP0(6,0x40);
	InputP0(7,0xBF);
	Delay(8);
	InputP0(7,0xFF);
	InputP0(6,0x80);
	InputP0(7,duan[LED_Level]);
	Delay(8);
	InputP0(7,0xFF);
}

void Key_Scanf(void)
{
	if(S4 == 0)
	{	
		Delay(10);
		if(S4 == 0)
		{
			if(Dis_Mod == 3 || Dis_Mod == 0)
			{
				while(S4 == 0)
				{
					Light_Contral();
					LED_Work();
					Display_Level();
				}
			}
			else if(Dis_Mod == 1)
			{
				if(LED_Mod >= 2)
					LED_Mod = LED_Mod - 1;
				while(S4 == 0)
					Display();
			}
			else if(Dis_Mod == 2)
			{
				if(Dtime_100ms >= 5)
					Dtime_100ms = Dtime_100ms - 1;
				while(S4 == 0)
					Display();
			}
		}
	}
	if(S5 == 0 && Dis_Mod != 0)
	{
		Delay(10);
		if(S5 == 0)
		{
			if(Dis_Mod == 1)
			{
				if(LED_Mod <= 3)
					LED_Mod = LED_Mod + 1;
				while(S5 == 0)
					Display();
			}
			if(Dis_Mod == 2)
			{
				if(Dtime_100ms <= 11)
					Dtime_100ms = Dtime_100ms + 1;
				while(S5 == 0)
					Display();
			}
		}
	}
	if(S6 == 0)
	{
		Delay(10);
		if(S6 == 0)
		{
			if(Dis_Mod == 2)
			{
				Write_Byte_24C02(0x00,LED_Mod);
				Delay(20);
				Write_Byte_24C02(0x01,Dtime_100ms);
			}
			Dis_Mod = (Dis_Mod + 1) % 3;
			while(S6 == 0)
				Display();
		}
	}
	if(S7 == 0)
	{
		Delay(10);
		if(S7 == 0)
		{
			FFlag = ~FFlag;
			while(S7 == 0)
				Display();
		}
	}
}

void main(void)
{
	InputP0(5,0x00);
	InputP0(4,0xFF);
	Init_Timer();
	LED_Mod = Read_Byte_24C02(0x00);
	Dtime_100ms = Read_Byte_24C02(0x01);
	TR0 = 1;
	TR1 = 1;
	while(1)
	{
		Light_Contral();
		LED_Work();
		Display();
		Key_Scanf();
	}
}

void T0_ser(void) interrupt 1
{
	TH0 = (65536 - 100) / 256;
	TL0 = (65536 - 100) % 256;
	Count_100us = Count_100us + 1;
	if(Count_100us >= 0 && Count_100us < Pwm_Duty)
		LED_Flag = 1;
	else if(Count_100us >= Pwm_Duty && Count_100us < 100)
		LED_Flag = 0;
	else if(Count_100us == 100)
		Count_100us = 0;
}

void T1_ser(void) interrupt 3
{
	TH1 = (65536 - 10000) / 256;
	TL1 = (65536 - 10000) % 256;
	Count_10ms = Count_10ms + 1;
	if(Count_10ms == 10)
	{
		Dcount_100ms = Dcount_100ms + 1;
		if(Dcount_100ms == Dtime_100ms)
		{
			if(FFlag)
				LED_Num = (LED_Num + 1) % 8;
			Dcount_100ms = 0;
		}
		Count_100ms = Count_100ms + 1;
		if(Count_100ms == 8)
		{
			Dis_Flag = ~Dis_Flag;
			Count_100ms = 0;
		}
		Count_10ms = 0;
	}
}