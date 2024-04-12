#ifndef HARDWARE_DS1302_H
#define HARDWARE_DS1302_H

#include <reg52.h>

sbit DS1302_CE   = P1^2;
sbit DS1302_SCLK = P1^3;
sbit DS1302_IO   = P1^4;

extern unsigned char time[7];
extern unsigned char set_time[7];
void DS1302_init(void);
void DS1302_read_time(void);

#endif

//unsigned char str[16];
//str[0] = 'D';str[1] = 'A';str[2] = 'T';str[3] = 'E';
//str[4] = ':';
//str[5] = '2';str[6] = '0';  //20xx年
//str[7] = (time[6] >> 4) + '0';     //年
//str[8] = (time[6] & 0x0F) + '0';
//str[9] = '-';
//str[10] = (time[4] >> 4) + '0';    //月
//str[11] = (time[4] & 0x0F) + '0';
//str[12] = '-';
//str[13] = (time[3] >> 4) + '0';   //日
//str[14] = (time[3] & 0x0F) + '0'; 
//str[15] = '\0';
//LCD1602_show_str(0, 0, str);

//str[0] = 'T';str[1] = 'I';str[2] = 'M';str[3] = 'E';
//str[4] = ':';
//str[5] = (time[2] >> 4) + '0';     //小时
//str[6] = (time[2] & 0x0F) + '0';
//str[7] = ':';
//str[8] = (time[1] >> 4) + '0';    //分钟
//str[9] = (time[1] & 0x0F) + '0';
//str[10] = ':';
//str[11] = (time[0] >> 4) + '0';   //秒
//str[12] = (time[0] & 0x0F) + '0';
//str[13] = ' ';
//str[14] = ((time[5] - 1) & 0x0F) + '0';  //星期
//str[15] = '\0';
//LCD1602_show_str(0, 1, str);
