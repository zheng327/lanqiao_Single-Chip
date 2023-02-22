#include "my.h"
#include "ds1302.h"
#include "onewire.h"
#include "display.h"

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

void Display_Time(unsigned char *TIME)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		InputP0(6,0x01<<i);
		switch(i)
		{
			case 0:InputP0(7,duan[TIME[4]/16]);break;
			case 1:InputP0(7,duan[TIME[4]%16]);break;
				
			case 2:InputP0(7,0xBF);break;
				
			case 3:InputP0(7,duan[TIME[5]/16]);break;
			case 4:InputP0(7,duan[TIME[5]%16]);break;
				
			case 5:InputP0(7,0xBF);break;
				
			case 6:InputP0(7,duan[TIME[6]/16]);break;
			case 7:InputP0(7,duan[TIME[6]%16]);break;
		}
		Delay(8);
		InputP0(7,0xFF);
	}
}

void Display_Time1(unsigned char *TIME)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		InputP0(6,0x01<<i);
		switch(i)
		{
			case 0:InputP0(7,0xFF);break;
			case 1:InputP0(7,0xFF);break;
				
			case 2:InputP0(7,0xBF);break;
				
			case 3:InputP0(7,duan[TIME[5]/16]);break;
			case 4:InputP0(7,duan[TIME[5]%16]);break;
				
			case 5:InputP0(7,0xBF);break;
				
			case 6:InputP0(7,duan[TIME[6]/16]);break;
			case 7:InputP0(7,duan[TIME[6]%16]);break;
		}
		Delay(8);
		InputP0(7,0xFF);
	}
}

void Display_Time2(unsigned char *TIME)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		InputP0(6,0x01<<i);
		switch(i)
		{
			case 0:InputP0(7,duan[TIME[4]/16]);break;
			case 1:InputP0(7,duan[TIME[4]%16]);break;
				
			case 2:InputP0(7,0xBF);break;
				
			case 3:InputP0(7,0xFF);break;
			case 4:InputP0(7,0xFF);break;
				
			case 5:InputP0(7,0xBF);break;
				
			case 6:InputP0(7,duan[TIME[6]/16]);break;
			case 7:InputP0(7,duan[TIME[6]%16]);break;
		}
		Delay(8);
		InputP0(7,0xFF);
	}
}

void Display_Time3(unsigned char *TIME)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		InputP0(6,0x01<<i);
		switch(i)
		{
			case 0:InputP0(7,duan[TIME[4]/16]);break;
			case 1:InputP0(7,duan[TIME[4]%16]);break;
				
			case 2:InputP0(7,0xBF);break;
				
			case 3:InputP0(7,duan[TIME[5]/16]);break;
			case 4:InputP0(7,duan[TIME[5]%16]);break;
				
			case 5:InputP0(7,0xBF);break;
				
			case 6:InputP0(7,0xFF);break;
			case 7:InputP0(7,0xFF);break;
		}
		Delay(8);
		InputP0(7,0xFF);
	}
}

void Display_Clock(unsigned char *CLOCK)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		InputP0(6,0x01<<i);
		switch(i)
		{
			case 0:InputP0(7,duan[CLOCK[0]/16]);break;
			case 1:InputP0(7,duan[CLOCK[0]%16]);break;
				
			case 2:InputP0(7,0xBF);break;
				
			case 3:InputP0(7,duan[CLOCK[1]/16]);break;
			case 4:InputP0(7,duan[CLOCK[1]%16]);break;
				
			case 5:InputP0(7,0xBF);break;
				
			case 6:InputP0(7,duan[CLOCK[2]/16]);break;
			case 7:InputP0(7,duan[CLOCK[2]%16]);break;
		}
		Delay(8);
		InputP0(7,0xFF);
	}
}

void Display_Clock1(unsigned char *CLOCK)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		InputP0(6,0x01<<i);
		switch(i)
		{
			case 0:InputP0(7,0xFF);break;
			case 1:InputP0(7,0xFF);break;
				
			case 2:InputP0(7,0xBF);break;
				
			case 3:InputP0(7,duan[CLOCK[1]/16]);break;
			case 4:InputP0(7,duan[CLOCK[1]%16]);break;
				
			case 5:InputP0(7,0xBF);break;
				
			case 6:InputP0(7,duan[CLOCK[2]/16]);break;
			case 7:InputP0(7,duan[CLOCK[2]%16]);break;
		}
		Delay(8);
		InputP0(7,0xFF);
	}
}

void Display_Clock2(unsigned char *CLOCK)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		InputP0(6,0x01<<i);
		switch(i)
		{
			case 0:InputP0(7,duan[CLOCK[0]/16]);break;
			case 1:InputP0(7,duan[CLOCK[0]%16]);break;
				
			case 2:InputP0(7,0xBF);break;
				
			case 3:InputP0(7,0xFF);break;
			case 4:InputP0(7,0xFF);break;
				
			case 5:InputP0(7,0xBF);break;
				
			case 6:InputP0(7,duan[CLOCK[2]/16]);break;
			case 7:InputP0(7,duan[CLOCK[2]%16]);break;
		}
		Delay(8);
		InputP0(7,0xFF);
	}
}

void Display_Clock3(unsigned char *CLOCK)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		InputP0(6,0x01<<i);
		switch(i)
		{
			case 0:InputP0(7,duan[CLOCK[0]/16]);break;
			case 1:InputP0(7,duan[CLOCK[0]%16]);break;
				
			case 2:InputP0(7,0xBF);break;
				
			case 3:InputP0(7,duan[CLOCK[1]/16]);break;
			case 4:InputP0(7,duan[CLOCK[1]%16]);break;
				
			case 5:InputP0(7,0xBF);break;
				
			case 6:InputP0(7,0xFF);break;
			case 7:InputP0(7,0xFF);break;
		}
		Delay(8);
		InputP0(7,0xFF);
	}
}

void Delay_SEG(unsigned char x,unsigned char time_flag,unsigned char clock_flag,unsigned char *time,unsigned char *clock)
{
	unsigned char temp;
	temp = x;
	if(clock_flag == 0)
	{
		if(time_flag == 0)
			Read_Ds1302_Time(time);
		while(temp--)
		Display_Time(time);
	}
	else
	{
		while(temp--)
		Display_Clock(clock);
	}//ÁÁ
	
	temp = x;
	switch(time_flag)
	{
		case 1:while(temp--) Display_Time1(time);break;
		case 2:while(temp--) Display_Time2(time);break;
		case 3:while(temp--) Display_Time3(time);break;
		default:break;
	}
	switch(clock_flag)
	{
		case 1:while(temp--) Display_Clock1(clock);break;
		case 2:while(temp--) Display_Clock2(clock);break;
		case 3:while(temp--) Display_Clock3(clock);break;
		default:break;
	}//Ãð
}

void Display_Temp(int x)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		InputP0(6,0x01<<i);
		switch(i)
		{
			case 0:InputP0(7,0xFF);break;
			case 1:InputP0(7,0xFF);break;
			case 2:InputP0(7,0xFF);break;
			case 3:InputP0(7,0xFF);break;
			case 4:InputP0(7,0xFF);break;
			case 5:InputP0(7,0xFF);break;
			
			case 6:if(x >= 10) InputP0(7,duan[x/10]);break;
			case 7:InputP0(7,duan[x%10]);break;
		}
		Delay(8);
		InputP0(7,0xFF);
	}
}