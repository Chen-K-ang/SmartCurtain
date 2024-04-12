#include "24C02C.h"

unsigned char E2_24C02C_read_byte(unsigned char IC_addr, unsigned char byte_addr)
{
	unsigned char dat = 0;
	unsigned char i = 0;
	do {
		I2C_start();
		if(!(I2C_write_byte(IC_addr << 1))) //����Ѱַ���ж��Ƿ���Խ��ж�д
			break;
		if (i++ > 50)  //����ѭ��
			break;
		I2C_stop();
	} while (1);
	I2C_write_byte(byte_addr);     //�ڲ��ֽڵ�ַ
	I2C_start();
	I2C_write_byte((IC_addr << 1) | 0x01); //������
	dat = I2C_read_byte();
	send_ack(1);    //������ֽڣ�����NOACK
	I2C_stop();
	
	return (dat);
}

void E2_24C02C_write_byte(unsigned char IC_addr, unsigned char byte_addr, unsigned char dat)
{
	unsigned char i = 0;
	do {
		I2C_start();
		if(!(I2C_write_byte(IC_addr << 1))) //����Ѱַ���ж��Ƿ���Խ��ж�д
			break;
		if (i++ > 50)  //����ѭ��
			break;
		I2C_stop();
	} while (1);
	I2C_write_byte(byte_addr);    //ȷ���ڲ��ֽڵ�ַ
	I2C_write_byte(dat);          //д����
	I2C_stop();
}

//len < 0xFF - byte_addr
void E2_24C02C_read(unsigned char IC_addr, unsigned char byte_addr, unsigned char *buf, unsigned char len)
{
	unsigned char i = 0;
	do {
		I2C_start();
		if(!(I2C_write_byte(IC_addr << 1))) //����Ѱַ���ж��Ƿ���Խ��ж�д
			break;
		if (i++ > 50)  //����ѭ��
			break;
		I2C_stop();
	} while (1);
	I2C_write_byte(byte_addr);
	I2C_start();
	I2C_write_byte((IC_addr << 1) | 0x01);
	while (len > 1) {
		*buf++ = I2C_read_byte();  //�����϶�һ���ֽ�����
		send_ack(0);               //��ACK��׼������һ���ֽ�
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
			if(!(I2C_write_byte(IC_addr << 1))) //�ж��Ƿ���Խ��ж�д
				break;
			if (i++ > 50)  //����ѭ��
				break;
			I2C_stop();
		} while (1);
		I2C_write_byte(byte_addr++);
		I2C_write_byte(*buf++);
		I2C_stop();
		//��ʱ���ݽ���д�룬 ���벻�ɶ�д״̬		
		len--;	
	}
}

//��ҳд��8�ֽ�һҳ
//����ֵ��0 д��len�ݹ��洢�ռ��ֽ���
unsigned char E2_24C02C_write_page(unsigned char IC_addr, unsigned char byte_addr, unsigned char *buf, unsigned char len)
{
	unsigned char i = 0;
	
	if ((EEPROM_MAX_SIZE - byte_addr) > len)
		return 0;
	while (len > 0) {
		do {
			I2C_start();
			if(!(I2C_write_byte(IC_addr << 1))) //�ж��Ƿ���Խ��ж�д
				break;
			if (i++ > 50)  //����ѭ��
				break;
			I2C_stop();
		} while (1);
		I2C_write_byte(byte_addr); //ҳ��ʼ��ַ
		while (len > 0) {
			I2C_write_byte(*buf++);
			byte_addr++; 
			len--;
			if (byte_addr & EEPROM_PAGE_SIZE == 0)
				break;
		}
		I2C_stop();
		//��ʱ���ݽ���д�룬 ���벻�ɶ�д״̬
	}
	return 1;
}
