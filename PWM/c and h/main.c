#include <stc15.h>


typedef unsigned char u8;
typedef unsigned int u16;



u16 xdata count;




void main()
{
	
	
	while(1)
	{
		count++;
		if(count<10)
		{
			P0=0xff;P2=0x80;P0=0xfe;P2=0;
		}
		else
		{
			P2=0x80;P0=0xff;P2=0;
		}
		if(count>=1000)count=0;	
	}
}










































