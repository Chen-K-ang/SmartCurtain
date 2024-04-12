#ifndef HARDWARE_24C02C_H
#define HARDWARE_24C02C_H

#include "I2C.h"

#define E2_24C020_IC_ADDR 0x50

#define EEPROM_MAX_SIZE  0xFF
#define EEPROM_PAGE_SIZE 0x07  //µØÖ·0x00~0x07 8¸ö×Ö½Ú

union float_data
{
	float f_data;
	unsigned char byte[4];
};

unsigned char E2_24C02C_read_byte(unsigned char IC_addr, unsigned char byte_addr);
void E2_24C02C_write_byte(unsigned char IC_addr, unsigned char byte_addr, unsigned char dat);
void E2_24C02C_read(unsigned char IC_addr, unsigned char byte_addr, unsigned char *buf, unsigned char len);
void E2_24C02C_write(unsigned char IC_addr, unsigned char byte_addr, unsigned char *buf, unsigned char len);
unsigned char E2_24C02C_write_page(unsigned char IC_addr, unsigned char byte_addr, unsigned char *buf, unsigned char len);

#endif
