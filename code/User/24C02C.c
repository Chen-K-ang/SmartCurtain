#include "24C02C.h"

unsigned char E2_24C02C_read_byte(unsigned char IC_addr, unsigned char byte_addr)
{
	unsigned char dat = 0;
	unsigned char i = 0;
	do {
		I2C_start();
		if(!(I2C_write_byte(IC_addr << 1))) //器件寻址，判断是否可以进行读写
			break;
		if (i++ > 50)  //防死循环
			break;
		I2C_stop();
	} while (1);
	I2C_write_byte(byte_addr);     //内部字节地址
	I2C_start();
	I2C_write_byte((IC_addr << 1) | 0x01); //读操作
	dat = I2C_read_byte();
	send_ack(1);    //读完此字节，发送NOACK
	I2C_stop();
	
	return (dat);
}

void E2_24C02C_write_byte(unsigned char IC_addr, unsigned char byte_addr, unsigned char dat)
{
	unsigned char i = 0;
	do {
		I2C_start();
		if(!(I2C_write_byte(IC_addr << 1))) //器件寻址，判断是否可以进行读写
			break;
		if (i++ > 50)  //防死循环
			break;
		I2C_stop();
	} while (1);
	I2C_write_byte(byte_addr);    //确定内部字节地址
	I2C_write_byte(dat);          //写数据
	I2C_stop();
}

//len < 0xFF - byte_addr
void E2_24C02C_read(unsigned char IC_addr, unsigned char byte_addr, unsigned char *buf, unsigned char len)
{
	unsigned char i = 0;
	do {
		I2C_start();
		if(!(I2C_write_byte(IC_addr << 1))) //器件寻址，判断是否可以进行读写
			break;
		if (i++ > 50)  //防死循环
			break;
		I2C_stop();
	} while (1);
	I2C_write_byte(byte_addr);
	I2C_start();
	I2C_write_byte((IC_addr << 1) | 0x01);
	while (len > 1) {
		*buf++ = I2C_read_byte();  //总线上读一个字节数据
		send_ack(0);               //发ACK，准备读下一个字节
		len--;
	}
	*buf = I2C_read_byte();
	send_ack(1);
	I2C_stop();
}

//len < 0xFF - byte_addr
void E2_24C02C_write(unsigned char IC_addr, unsigned char byte_addr, unsigned char *buf, unsigned char len)
{
	unsigned char i = 0;
	while (len > 0) {
		do {
			I2C_start();
			if(!(I2C_write_byte(IC_addr << 1))) //判断是否可以进行读写
				break;
			if (i++ > 50)  //防死循环
				break;
			I2C_stop();
		} while (1);
		I2C_write_byte(byte_addr++);
		I2C_write_byte(*buf++);
		I2C_stop();
		//此时数据进行写入， 进入不可读写状态		
		len--;	
	}
}

//按页写，8字节一页
//返回值，0 写入len草果存储空间字节数
unsigned char E2_24C02C_write_page(unsigned char IC_addr, unsigned char byte_addr, unsigned char *buf, unsigned char len)
{
	unsigned char i = 0;
	
	if ((EEPROM_MAX_SIZE - byte_addr) > len)
		return 0;
	while (len > 0) {
		do {
			I2C_start();
			if(!(I2C_write_byte(IC_addr << 1))) //判断是否可以进行读写
				break;
			if (i++ > 50)  //防死循环
				break;
			I2C_stop();
		} while (1);
		I2C_write_byte(byte_addr); //页起始地址
		while (len > 0) {
			I2C_write_byte(*buf++);
			byte_addr++; 
			len--;
			if (byte_addr & EEPROM_PAGE_SIZE == 0)
				break;
		}
		I2C_stop();
		//此时数据进行写入， 进入不可读写状态
	}
	return 1;
}
