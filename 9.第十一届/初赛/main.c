#include "STC15F2K60S2.H"
#include "PCF8591.H"
#include "AT24C02.H"
#include "my.h"

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

unsigned char Display_Mod = 0;	//显示模式，0：数据界面，1：参数界面，2：计数界面
unsigned long Count = 0;				//计数值
unsigned char Vp = 0;						//电压参数值*10
unsigned char D_AIN3 = 0;				//模数转化值
unsigned int V_AIN3 = 0;				//电压测量值*100
unsigned char Invalid = 0;			//无效操作次数
unsigned char Flag = 0;					//下降标志，0代表上升，1代表下降
unsigned char Key_Value = 0;
unsigned char Count_Time = 0;
unsigned char LED = 0xFF;

void Display(unsigned char mod)
{
	switch(mod)
	{
		//数据界面显示
		case 0:InputP0(6,0x01);InputP0(7,0xC1);Delay(8);InputP0(7,0xFF); //U
		InputP0(6,0x02);InputP0(7,0xFF);Delay(8);InputP0(7,0xFF);	//熄灭
		InputP0(6,0x04);InputP0(7,0xFF);Delay(8);InputP0(7,0xFF);	//熄灭
		InputP0(6,0x08);InputP0(7,0xFF);Delay(8);InputP0(7,0xFF);	//熄灭
		InputP0(6,0x10);InputP0(7,0xFF);Delay(8);InputP0(7,0xFF);	//熄灭
		InputP0(6,0x20);InputP0(7,duan[V_AIN3/100]&0x7F);Delay(8);InputP0(7,0xFF);	//3.
		InputP0(6,0x40);InputP0(7,duan[V_AIN3%100/10]);Delay(8);InputP0(7,0xFF);		//2
		InputP0(6,0x80);InputP0(7,duan[V_AIN3%10]);Delay(8);InputP0(7,0xFF);				//4
		break;
		//参数显示界面
		case 1:InputP0(6,0x01);InputP0(7,0x8C);Delay(8);InputP0(7,0xFF); //P
		InputP0(6,0x02);InputP0(7,0xFF);Delay(8);InputP0(7,0xFF);	//熄灭
		InputP0(6,0x04);InputP0(7,0xFF);Delay(8);InputP0(7,0xFF);	//熄灭
		InputP0(6,0x08);InputP0(7,0xFF);Delay(8);InputP0(7,0xFF);	//熄灭
		InputP0(6,0x10);InputP0(7,0xFF);Delay(8);InputP0(7,0xFF);	//熄灭
		InputP0(6,0x20);InputP0(7,duan[Vp/10]&0x7F);Delay(8);InputP0(7,0xFF);	//3.
		InputP0(6,0x40);InputP0(7,duan[Vp%10]);Delay(8);InputP0(7,0xFF);				//0
		InputP0(6,0x80);InputP0(7,duan[0]);Delay(8);InputP0(7,0xFF);						//0
		break;
		//计数界面
		case 2:InputP0(6,0x01);InputP0(7,0xC8);Delay(8);InputP0(7,0xFF); //N
		if(Count >= 1000000){InputP0(6,0x02);InputP0(7,duan[Count/1000000]);}Delay(8);InputP0(7,0xFF);
		if(Count >= 100000){InputP0(6,0x04);InputP0(7,duan[Count%1000000/100000]);}Delay(8);InputP0(7,0xFF);
		if(Count >= 10000){InputP0(6,0x08);InputP0(7,duan[Count%100000/10000]);}Delay(8);InputP0(7,0xFF);
		if(Count >= 1000){InputP0(6,0x10);InputP0(7,duan[Count%10000/1000]);}Delay(8);InputP0(7,0xFF);
		if(Count >= 100){InputP0(6,0x20);InputP0(7,duan[Count%1000/100]);}Delay(8);InputP0(7,0xFF);
		if(Count >= 10){InputP0(6,0x40);InputP0(7,duan[Count%100/10]);}Delay(8);InputP0(7,0xFF);
		InputP0(6,0x80);InputP0(7,duan[Count%10]);Delay(8);InputP0(7,0xFF);
		break;
	}
}

void AD_AIN3(void)
{
	D_AIN3 = PCF8591_AD(0x03);
	V_AIN3 = (unsigned int)((double)(D_AIN3)*5/255*100+0.5);
	if(Flag == 1 && V_AIN3 <= (unsigned int)(Vp)*10)
	{
		Flag = 0;
		Count++;
		Display(Display_Mod);
	}
	if(Count % 2 == 1)
		LED = LED & 0xFD;
	else
		LED = LED | 0x02;
	if(V_AIN3 > (unsigned int)(Vp)*10)
	{
		Flag = 1;
		TR0 = 0;
		InitHC138(4);
		LED = LED | 0x01;
		Count_Time = 0;
	}
	else
	{
		TR0 = 1;
		Flag = 0;
	}
}

unsigned char Key_Scanf(void)
{
	unsigned char key_value,row,col;
	P44 = 0;P42 = 0;P35 = 0; P34 = 0; P33 = 1; P32 = 1; P31 = 1; P30 = 1;
	Delay(10);
	if(P33 == 0) row = 1;
	else if(P32 == 0) row = 2;
	else if(P31 == 0) row = 3;
	else if(P30 == 0) row = 4;
	else row = 0;
	
	Delay(10);
	
	P44 = 1;P42 = 1;P35 = 1; P34 = 1; P33 = 0; P32 = 0; P31 = 0; P30 = 0;
	Delay(10);
	if(P44 == 0) col = 1;
	else if(P42 == 0) col = 2;
	else if(P35 == 0) col = 3;
	else if(P34 == 0) col = 4;
	else col = 0;
	
	P44 = 0;P42 = 0;P35 = 0; P34 = 0; P33 = 0; P32 = 0; P31 = 0; P30 = 0;
	
	if(row && col)
		key_value = 4 * col + row - 1;
	else
		key_value = 0;
	
	return key_value;
}

void Key_Work(void)
{
	Key_Value = Key_Scanf();
	//显示界面切换
	if(Key_Value == 12)
	{
		Display_Mod = (Display_Mod + 1) % 3;
		if(Display_Mod == 2)
			AT24C02_Write_Byte(0x00,Vp);
		Invalid = 0;
		while(Key_Value == 12)
		{
			Key_Value = Key_Scanf();
			Display(Display_Mod);
		}
	}
	//计数清零
	if(Key_Value == 13)
	{
		if(Display_Mod != 2)
			Invalid++;
		else
		{
			Count = 0;
			Invalid = 0;
		}
		while(Key_Value == 13)
		{
			Key_Value = Key_Scanf();
			Display(Display_Mod);
		}
	}
	//参数加
	if(Key_Value == 16)
	{
		if(Display_Mod != 1)
			Invalid++;
		else
		{
			Vp = (Vp + 5) % 55;
			Invalid = 0;
		}
		while(Key_Value == 16)
		{
			Key_Value = Key_Scanf();
			Display(Display_Mod);
		}
	}
	//参数减
	if(Key_Value == 17)
	{
		if(Display_Mod != 1)
			Invalid++;
		else
		{
			if(Vp != 0)
				Vp = Vp - 5;
			else
				Vp = 50;
			Invalid = 0;
		}
		while(Key_Value == 17)
		{
			Key_Value = Key_Scanf();
			Display(Display_Mod);
		}
	}
	if(Invalid >= 3)
	{
		LED = LED & 0xFB;
	}
	else
	{
		LED = LED | 0x04;
	}
}

void Work_Init(void)
{
	InputP0(5,0x00);
	InputP0(4,0xFF);
	Count = 0;
	Vp = AT24C02_Read_Byte(0x00);
}

void T0_Init(void)
{
	TMOD = 0x01;
	TH0 = (65536 - 50000) / 256;
	TL0 = (65536 - 50000) % 256;
	ET0 = 1;
	EA = 1;
	TR0 = 0;
}

void main(void)
{
	T0_Init();
	Work_Init();
	while(1)
	{
		AD_AIN3();
		Display(Display_Mod);
		Key_Work();
		InputP0(4,LED);
	}
}

void T0_Ser(void) interrupt 1
{
	TH0 = (65536 - 50000) / 256;
	TL0 = (65536 - 50000) % 256;
	Count_Time++;
	if(Count_Time == 100)
	{
		InitHC138(4);
		LED = LED & 0xFE;
		TR0 = 0;
	}
}