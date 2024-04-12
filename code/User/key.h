#ifndef HARDWARE_KEY_H
#define HARDWARE_KEY_H

#include <reg52.h>
#include "delay.h"

#define KEY_OPEN_FLAG   0x01
#define KEY_SET_FLAG    0x02
#define KEY_ADD_FLAG    0x03
#define KEY_REDUCE_FLAG 0x04
#define KEY_SIMU_FLAG   0x05

sbit KEY1 = P3^4; //设置模式
sbit KEY2 = P3^5; //设置定时
sbit KEY3 = P3^6; //加
sbit KEY4 = P3^7; //减
sbit KEY5 = P1^0; //模拟声音

unsigned char chiclet_keyboard_scan(unsigned char key_mode);

#endif
