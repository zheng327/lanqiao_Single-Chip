#include <reg52.h>
#include "ds1302.h"
#include "onewire.h"
#include "my.h"
#include "display.h"

sbit S4 = P3^3;	//减
sbit S5 = P3^2;	//加
sbit S6 = P3^1;	//闹钟设置按钮
sbit S7 = P3^0;	//时钟设置按钮
char TIME[7] = {0x22,0x07,0x01,0x02,0x23,0x59,0x50};
char CLOCK[3] = {0x00,0x00,0x00};
char TIME_SET[7];
char CLOCK_SET[3];
unsigned char TIME_SET_FLAG = 0;
unsigned char CLOCK_SET_FLAG = 0;
unsigned int TEMP_18B20 = 0;

void Init(void)
{
	InputP0(5,0x00);
	InputP0(4,0xFF);
	Write_Ds1302_Time(TIME);
}

void s7_scanf(void)
{
	if(S7 == 0)
	{
		if(S7 == 0 && CLOCK_SET_FLAG == 0)
		{
			while(S7 == 0);
				//Delay_SEG(10,TIME_SET_FLAG,CLOCK_SET_FLAG,TIME,CLOCK);
			TIME_SET_FLAG = TIME_SET_FLAG + 1;
			if(TIME_SET_FLAG == 4)
			{
				Write_Ds1302_Time(TIME);
				TIME_SET_FLAG = 0;
			}
		}
	}
}

void s6_scanf(void)
{
	if(S6 == 0)
	{
		if(S6 == 0 && TIME_SET_FLAG == 0)
		{
			while(S6 == 0);
				//Delay_SEG(10,TIME_SET_FLAG,CLOCK_SET_FLAG,TIME,CLOCK);
			CLOCK_SET_FLAG = CLOCK_SET_FLAG + 1;
			if(CLOCK_SET_FLAG == 4)
			{
				CLOCK_SET_FLAG = 0;
			}
		}
	}
}

void s5_scanf(void)
{
	if(S5 == 0)
	{
		if(S5 == 0 && TIME_SET_FLAG != 0)
		{
			while(S5 == 0)
				Delay_SEG(10,TIME_SET_FLAG,CLOCK_SET_FLAG,TIME,CLOCK);
			switch(TIME_SET_FLAG)
			{
				case 1:if(TIME[4]<24)TIME[4]++;break;
				case 2:if(TIME[5]<59)TIME[5]++;break;
				case 3:if(TIME[6]<59)TIME[6]++;break;
			}
		}
		else if(S5 == 0 && CLOCK_SET_FLAG != 0)
		{
			while(S5 == 0)
				Delay_SEG(10,TIME_SET_FLAG,CLOCK_SET_FLAG,TIME,CLOCK);
			switch(CLOCK_SET_FLAG)
			{
				case 1:if(CLOCK[0]<24)CLOCK[0]++;break;
				case 2:if(CLOCK[1]<59)CLOCK[1]++;break;
				case 3:if(CLOCK[2]<59)CLOCK[2]++;break;
			}
		}
	}
}

void s4_scanf(void)
{
	if(S4 == 0)
	{
		if(S4 == 0 && TIME_SET_FLAG != 0)
		{
			while(S4 == 0)
				Delay_SEG(10,TIME_SET_FLAG,CLOCK_SET_FLAG,TIME,CLOCK);
			switch(TIME_SET_FLAG)
			{
				case 1:if(TIME[4]>0)TIME[4]--;break;
				case 2:if(TIME[5]>0)TIME[5]--;break;
				case 3:if(TIME[6]>0)TIME[6]--;break;
			}
		}
		else if(S4 == 0 && CLOCK_SET_FLAG != 0)
		{
			while(S4 == 0)
				Delay_SEG(10,TIME_SET_FLAG,CLOCK_SET_FLAG,TIME,CLOCK);
			switch(CLOCK_SET_FLAG)
			{
				case 1:if(CLOCK[0]>0)CLOCK[0]--;break;
				case 2:if(CLOCK[1]>0)CLOCK[1]--;break;
				case 3:if(CLOCK[2]>0)CLOCK[2]--;break;
			}
		}
	}
	else if(S4 == 0 && TIME_SET_FLAG == 0 && CLOCK_SET_FLAG == 0)
	{
		if(S4 == 0)
		{
			while(S4 == 0)
			{
				TEMP_18B20 = Read_Temp_ds18b20();
				Display_Temp(TEMP_18B20);
			}
		}
	}
}

void Key_Scanf(void)
{
	s7_scanf();
	s6_scanf();
	s5_scanf();
	s4_scanf();
}
void Work(void)
{
	Key_Scanf();
	Delay_SEG(100,TIME_SET_FLAG,CLOCK_SET_FLAG,TIME,CLOCK);
}

void main(void)
{
	Init();
	while(1)
		Work();
	
}