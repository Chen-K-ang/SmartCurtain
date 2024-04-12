#ifndef HARDWARE_MOTOR_H
#define HARDWARE_MOTOR_H

#include <reg52.h>

sbit motor_flag = P3^0; //正反转标志位
sbit IN3 = P2^3;
sbit IN4 = P2^4;
sbit IN1 = P2^5;
sbit IN2 = P2^6;
sbit ENA = P2^7;

extern float cur_speed;
extern int mcount;
extern bit timer0_isr_flag;
extern bit timer0_isr_flag2;

void motor_init(void);
#endif
