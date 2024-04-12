#include "DS1302.h"

//读写地址命令  秒-分-时-日-月-周-年
unsigned char code read_RTC_cmd[7]  = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8B, 0x8D};
unsigned char code write_RTC_cmd[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C};

//2022-9-14-8:00:00
unsigned char time[7] = {0x00, 0x00, 0x08, 0x14, 0x09, 0x02, 0x20};
unsigned char set_time[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static void DS1302_wirte_byte(unsigned char dat)
{
	unsigned char i = 0;
	//数据低位在前，高位在后
	for (; i < 8; i++) {
		DS1302_IO = dat & 0x01;
		dat >>= 1;
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
}

static unsigned char DS1302_read_byte(void)
{
	unsigned char temp = 0x01;
	unsigned char dat = 0;
	
	for (; temp != 0; temp <<= 1) {
		if (DS1302_IO != 0)
			dat |= temp;
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;	
	}

	return (dat);
}

//addr 寄存器地址或命令字节    dat BCD码
void DS1302_write(unsigned char addr, unsigned char dat)
{
	DS1302_CE = 0;
	DS1302_SCLK = 0;
	DS1302_CE = 1;
	DS1302_wirte_byte(addr);
	DS1302_wirte_byte(dat);
	DS1302_CE = 0;
}

unsigned char DS1302_read(unsigned char addr)
{
	unsigned char dat = 0;
	DS1302_CE = 0;
	DS1302_SCLK = 0;
	DS1302_CE = 1;
	DS1302_wirte_byte(addr);
	dat = DS1302_read_byte();
	DS1302_CE = 0;
	
	return (dat);
}

void DS1302_init(void)
{
	unsigned char i = 0;
	DS1302_read_time();
	for (; i < 7; i++)
		set_time[i] = time[i];
//	DS1302_write(0x8E, 0x00); //关闭写保护
	//写入默认时间
//	for (; i < 7; i++)
//		DS1302_write(write_RTC_cmd[i], time[i]);
	
//	DS1302_write(0x8E, 0x80); //打开写保护，只读
}

void DS1302_read_time(void)
{
	unsigned char i = 0;
	
	for (; i < 7; i++)
		time[i] = DS1302_read(read_RTC_cmd[i]);
}
