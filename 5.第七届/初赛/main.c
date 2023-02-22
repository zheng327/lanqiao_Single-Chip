#include "STC15F2K60S2.H"
#include "onewire.h"

sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;
sbit FAN = P3^4;

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

unsigned int Temp = 0;	//�¶�����
unsigned char Dis_Mod = 0;	//��ʾģʽ��0Ϊ����ʱ��1Ϊ������ʾ
unsigned char Dis_Wei = 0;	//λѡ
unsigned char Dis_Buffer[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Fan_Mod = 1;	//����ģʽ��1Ϊ˯�߷磬2Ϊ��Ȼ�磬3Ϊ����
unsigned char Stime_s = 0;	//����ʱ�䣬��λs
unsigned char Rtime_s = 0;	//ʣ��ʱ�䣬��λs
unsigned char Pwm_Duty = 0;	//PWM�ź�ռ�ձ�
unsigned char Count_10us = 0;
unsigned char Count_50ms = 0;
bit Fan_Flag = 0;	//���������־λ��0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ

void Delay10ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 117;
	j = 184;
	do
	{
		while (--j);
	} while (--i);
}

void Timer0Init(void)		//10΢��@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xF6;		//���ö�ʱ��ʼֵ
	TH0 = 0xFF;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0��ͣ��ʱ
	EA = 1;
	ET0 = 1;
}

void Timer1Init(void)		//50����@12.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xB0;		//���ö�ʱ��ʼֵ
	TH1 = 0x3C;		//���ö�ʱ��ʼֵ
	TF1 = 0;		//���TF1��־
	TR1 = 0;		//��ʱ��1��ͣ��ʱ
	ET1 = 1;
}

void Timer2Init(void)		//1����@12.000MHz
{
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0x20;		//���ö�ʱ��ʼֵ
	T2H = 0xD1;		//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	IE2 |= 0x04;
	EA = 1;
}

void All_Init(void)
{
	P2 &= 0x1F;
	P0 = 0x00;
	P2 |= 0xA0;
	
	P2 &= 0x1F;
	P0 = 0xFF;
	P2 |= 0x80;
	
	Timer0Init();
	Timer1Init();
	Timer2Init();
}

unsigned int DS18B20_Work(void)
{
	double dat;
	unsigned int temp;
	unsigned char lsb,msb;
	
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0x44);
	
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0xBE);
	
	lsb = Read_DS18B20();
	msb = Read_DS18B20();
	
	temp = msb;
	temp = temp << 8;
	temp = temp | lsb;
	
	dat = temp * 0.0625 + 0.5;
	
	temp = (unsigned int)dat;
	return temp;
}

void Fan_Work(void)
{
	switch(Fan_Mod)
	{
		case 1:Pwm_Duty = 20;break;
		case 2:Pwm_Duty = 30;break;
		case 3:Pwm_Duty = 70;break;
	}
	if(Rtime_s != 0)
	{
		TR0 = 1;
		TR1 = 1;
		FAN = Fan_Flag;
	}
}

void LED_Work(void)
{
	P2 &= 0x1F;
	if(Rtime_s)
	{
		switch(Fan_Mod)
		{
			case 1:P0 = 0xFE;break;
			case 2:P0 = 0xFD;break;
			case 3:P0 = 0xFB;break;
		}
	}
	else
		P0 = 0xFF;
	P2 |= 0x80;
}

void Dis_Work(void)
{
	if(Dis_Mod == 0)
	{
		Dis_Buffer[0] = 0xBF;
		Dis_Buffer[1] = duan[Fan_Mod];
		Dis_Buffer[2] = 0xBF;
		Dis_Buffer[3] = 0xFF;
		Dis_Buffer[4] = duan[Rtime_s / 1000];
		Dis_Buffer[5] = duan[Rtime_s % 1000 / 100];
		Dis_Buffer[6] = duan[Rtime_s % 100 / 10];
		Dis_Buffer[7] = duan[Rtime_s % 10];
	}
	if(Dis_Mod == 1)
	{
		Dis_Buffer[0] = 0xBF;
		Dis_Buffer[1] = duan[4];
		Dis_Buffer[2] = 0xBF;
		Dis_Buffer[3] = 0xFF;
		Dis_Buffer[4] = 0xFF;
		Dis_Buffer[5] = duan[Temp / 10];
		Dis_Buffer[6] = duan[Temp % 10];
		Dis_Buffer[7] = duan[0x0C];
	}
}

void All_Work(void)
{
	Temp = DS18B20_Work();
	Fan_Work();
	LED_Work();
	Dis_Work();
	
}
void Key_Scanf(void)
{
	if(S4 == 0)
	{
		Delay10ms();
		if(S4 == 0)
		{
			Fan_Mod = Fan_Mod + 1;
			if(Fan_Mod == 4)
				Fan_Mod = 1;
			while(S4 == 0)
				All_Work();
		}
	}
	if(S5 == 0)
	{
		Delay10ms();
		if(S5 == 0)
		{
			Stime_s = Stime_s + 60;
			if(Stime_s == 180)
				Stime_s = 0;
			Rtime_s = Stime_s;
			while(S5 == 0)
				All_Work();
		}
	}
	if(S6 == 0)
	{
		Delay10ms();
		if(S6 == 0)
		{
			Rtime_s = 0;
			Stime_s = 0;
			while(S6 == 0)
				All_Work();
		}
	}
	if(S7 == 0)
	{
		Delay10ms();
		if(S7 == 0)
		{
			Dis_Mod = (Dis_Mod + 1) % 2;
			while(S7 == 0)
				All_Work();
		}
	}
}


void main(void)
{
	All_Init();
	Temp = DS18B20_Work();
	while(1)
	{
		All_Work();
		Key_Scanf();
	}
}


void T0_ser(void) interrupt 1
{
	Count_10us = Count_10us + 1;
	if(Count_10us >= 0 && Count_10us < Pwm_Duty) //PWM�ߵ�ƽ
	{
		Fan_Flag = 1;
	}
	else if(Count_10us >= Pwm_Duty && Count_10us < 100) //PWM�͵�ƽ
	{
		Fan_Flag = 0;
	}
	else if(Count_10us == 100) //һ������Ϊ100��10us����1ms
	{
		Count_10us = 0;
	}
}

void T1_ser(void) interrupt 3
{
	Count_50ms = Count_50ms + 1;
	if(Count_50ms == 20)
	{
		if(Rtime_s > 0)	//����ʱs
			Rtime_s = Rtime_s - 1;
		else if(Rtime_s == 0) //��ʱ������ֹͣ��ʱ��T0��T1
		{
			Stime_s = 0;
			TR0 = 0;
			TH0 = (65536 - 10) / 256;
			TL0 = (65536 - 10) % 256;
			Count_10us = 0;
			TR1 = 0;
			TH1 = (65536 - 50000) / 256;
			TL1 = (65536 - 50000) % 256;
			Count_50ms = 0;
		}
		Count_50ms = 0;
	}
}

void T2_ser(void) interrupt 12
{
	P2 &= 0x1F;
	P0 = 0xFF;
	P2 |= 0xE0;
	
	P2 &= 0x1F;
	P0 = 0x01 << Dis_Wei;
	P2 |= 0xC0;
	
	P2 &= 0x1F;
	P0 = Dis_Buffer[Dis_Wei];
	P2 |= 0xE0;
	
	if(++Dis_Wei >= 8)
		Dis_Wei = 0;
}