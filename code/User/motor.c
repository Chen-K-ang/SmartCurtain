#include "motor.h"
/*	名称：直流电机测速
    说明：数码管显示的正反转状态与转速值应与motor encode器件上显示的值基本一致；
		  测量误差 ≤1RPM 。
*/

float cur_speed = 0;    //当前速度   	
int mcount=0;  //脉冲数

void motor_init(void)
{
	IN1 = 1; //电机不转
	IN2 = 1;
	IN3 = 1;
	IN4 = 1;
//**********设置定时器T0工作方式和定时时间**********//	  	
//	TMOD &= 0xF0;		//设置定时器模式  选择定时器0
//	TMOD |= 0x01;		//设置定时器模式  0001   M1M0 01 ==> 方式1
//	TH0  = 0x3C;  //50ms
//	TL0  = 0xB0;	   
//**************************************************//			 		
	IT0=1;		  //外部中断0（P3.2）为边沿触发
//	EX0=1;		  //允许外部中断0产生中断
	
	TMOD &= 0x0F; //设置定时器模式
	TMOD |= 0x01; //设置定时器模式
	TL0 = 0x9C;   //设置定时初值
	TH0 = 0xFF;   //设置定时初值
	TF0 = 0;      //清除TF1标志
	ET0 = 1;      //打开定时器1中断
	EA = 1;       //打开总中断
	TR0 = 1;      //定时器1开始计时
}

int motor_distance(void)
{
	unsigned char distance = 0;
	return distance;
}

//**********外部中断计脉冲数**********//
void exit0_ISR() interrupt 0
{
	if (motor_flag)
		mcount--;
	else
		mcount++;
}

bit timer0_isr_flag = 0;
bit timer0_isr_flag2 = 0;
void timer0_ISR() interrupt 1
{
	static unsigned char count=0;
	static unsigned char flag=0;
	static unsigned char flag2=0;
	ET0=0;	   	 
	count++;
//	if (count < 150)
//		ENA = 1;
//	else
//		ENA = 0;
	if(count == 200)//20ms
	{
		flag++;
		flag2++;
		count = 0; 
	}

	if (flag == 10) {//200ms
		flag = 0;
		timer0_isr_flag = ~timer0_isr_flag;
	}
	
	if (flag2 == 100) {//2s
		flag2 = 0;
		timer0_isr_flag2 = ~timer0_isr_flag2;
	}
	TL0 = 0x9C;
	TH0 = 0xFF; //重装初值
	ET0=1;
}
