void Write_Ds1302(unsigned char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte( unsigned char address );
void Ds1302_Write_Time(unsigned char *p);
void Ds1302_Read_Time(unsigned char *p);