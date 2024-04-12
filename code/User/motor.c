#include "motor.h"
/*	���ƣ�ֱ���������
    ˵�����������ʾ������ת״̬��ת��ֵӦ��motor encode��������ʾ��ֵ����һ�£�
		  ������� ��1RPM ��
*/

float cur_speed = 0;    //��ǰ�ٶ�   	
int mcount=0;  //������

void motor_init(void)
{
	IN1 = 1; //�����ת
	IN2 = 1;
	IN3 = 1;
	IN4 = 1;
//**********���ö�ʱ��T0������ʽ�Ͷ�ʱʱ��**********//	  	
//	TMOD &= 0xF0;		//���ö�ʱ��ģʽ  ѡ��ʱ��0
//	TMOD |= 0x01;		//���ö�ʱ��ģʽ  0001   M1M0 01 ==> ��ʽ1
//	TH0  = 0x3C;  //50ms
//	TL0  = 0xB0;	   
//**************************************************//			 		
	IT0=1;		  //�ⲿ�ж�0��P3.2��Ϊ���ش���
//	EX0=1;		  //�����ⲿ�ж�0�����ж�
	
	TMOD &= 0x0F; //���ö�ʱ��ģʽ
	TMOD |= 0x01; //���ö�ʱ��ģʽ
	TL0 = 0x9C;   //���ö�ʱ��ֵ
	TH0 = 0xFF;   //���ö�ʱ��ֵ
	TF0 = 0;      //���TF1��־
	ET0 = 1;      //�򿪶�ʱ��1�ж�
	EA = 1;       //�����ж�
	TR0 = 1;      //��ʱ��1��ʼ��ʱ
}

int motor_distance(void)
{
	unsigned char distance = 0;
	return distance;
}

//**********�ⲿ�жϼ�������**********//
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
	TH0 = 0xFF; //��װ��ֵ
	ET0=1;
}
