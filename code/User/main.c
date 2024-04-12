#include <stdio.h>
#include "24C02C.h"
#include "DS1302.h"
#include "LCD1602.h"
#include "motor.h"
#include "key.h"

#define CURTAIN_CLOES 0     //ȫ��
#define CURTAIN_HALF  120   //�뿪
#define CURTAIN_OPEN  240   //ȫ��
#define CURTAIN_STEP  30    //ÿ�ΰ�������           
sbit LIGHT = P1^1;          
sbit SAFE  = P3^3;

int aimed_pos = 0;              //Ԥ��λ��
unsigned char pos_flag  = 0;    //ģ������
unsigned char key_flag = 0;
unsigned char set_flag = 0;
unsigned char mode_flag = 0;
bit choose_flag = 0;
void set_time_display(void);
void time_dispaly(void);
void key_function(void);
void mode_deal(void);

void main()
{
	IT1=1;   //��λ���أ��ⲿ�ж�1�½��ش���
        EX1=1;

	motor_init();
	LCD1602_init();
	DS1302_init();
	DS1302_read_time();
	delay_ms(20);

	while (1) {
		DS1302_read_time();
		key_function();
		time_dispaly();
		mode_deal();
	}
}

void time_dispaly(void)
{
	unsigned char str[16];
	unsigned char len = 0, len2 = 0;
	//����ʱ��
	str[0] = 'T';str[1] = 'I';str[2] = 'M';str[3] = 'E';
	str[4] = ':';
	str[5] = (time[2] >> 4) + '0';     //Сʱ
	str[6] = (time[2] & 0x0F) + '0';
	str[7] = ':';
	str[8] = (time[1] >> 4) + '0';    //����
	str[9] = (time[1] & 0x0F) + '0';

	str[10] = ':';
	str[11] = (time[0] >> 4) + '0';   //��
	str[12] = (time[0] & 0x0F) + '0';
	str[13] = ' ';
	LCD1602_show_str(0, 0, str);

	//��ʱʱ��
	if (set_flag) {
		set_time_display();	
	} else {
		//��ʾ��ǰģʽ
		if (mode_flag == 2 && timer0_isr_flag2) {
			choose_flag = 1;
			str[0] = ' ';str[1] = 'S';str[2] = 'E';str[3] = 'T';
			str[4] = ':';
			str[5] = (set_time[2] >> 4) + '0';     //Сʱ
			str[6] = (set_time[2] & 0x0F) + '0';
			str[7] = ':';
			str[8] = (set_time[1] >> 4) + '0';    //����
			str[9] = (set_time[1] & 0x0F) + '0';

			str[10] = ':';
			str[11] = (set_time[0] >> 4) + '0';   //��
			str[12] = (set_time[0] & 0x0F) + '0';
			str[13] = ' ';
			LCD1602_show_str(0, 1, str);
		} else {
			if (choose_flag) {
				LCD1602_write_cmd(LCD_SCREEN_CLR);
				delay_ms(5);
				choose_flag = 0;
			}
			switch (mode_flag) {
				case 0:
					LCD1602_show_str(12, 1, " RC ");
					break;
				case 1:
					LCD1602_show_str(12, 1, "Auto");
					break;
				case 2:
					LCD1602_show_str(12, 1, "Time");
					break;
				default:
					break;
			}

			//��ʾԤ��λ�ú�ʵ��λ��
			LCD1602_show_str(0, 1, "s:");
			len = LCD1602_integer_to_str(aimed_pos, str);
			while (len < 3)
				str[len++] = ' ';
			str[len] = '\0';
			LCD1602_show_str(2, 1, str);
			
			LCD1602_show_str(6, 1, "g:");
			len = LCD1602_integer_to_str(mcount, str);
			while (len < 3)
				str[len++] = ' ';
			str[len] = '\0';
			LCD1602_show_str(8, 1, str);
		}

	}

}

void mode_deal(void)
{
	if (aimed_pos > mcount) {
		//�����ת
		EX0 = 1;
		IN1 = 0; IN2 = 1;
		IN3 = 1; IN4 = 0;
		
	} else if (aimed_pos < mcount) {
		//�����ת
		EX0 = 1;
		IN1 = 1; IN2 = 0;
		IN3 = 0; IN4 = 1;
	} else {
		IN1 = 1; IN2 = 1;
		IN3 = 1; IN4 = 1;
		EX0 = 0;  //�رձ������ɼ�
	}
	
	//�Զ�ģʽ
	if (mode_flag == 1) {
		if (LIGHT)  //����
			aimed_pos = CURTAIN_OPEN;
		else
			aimed_pos = CURTAIN_CLOES;
	//��ʱģʽ
	} else if (mode_flag == 2) {
		if ((set_time[2] == time[2]) && (set_time[1] == time[1]) && (set_time[0] == time[0])) {
			mode_flag = 0;
			//�ж϶�ʱ����ʱ���������ǹأ����෴��������
			if (aimed_pos > CURTAIN_HALF)
				aimed_pos = CURTAIN_CLOES;
			else
				aimed_pos = CURTAIN_OPEN;
		}
	}
	
}

void key_function(void)
{
	static bit KEY1_flag = 0, KEY2_flag = 0, KEY3_flag = 0,
			KEY4_flag = 0, KEY5_flag = 0;
	unsigned char temp = 0;
	
	if (KEY1 == 0) {    //����ģʽ  ���ֶ����Զ�����ʱ��
		delay_ms(10);
		if (KEY1_flag) {
			KEY1_flag = 0;
			mode_flag++;
			if(mode_flag > 2)
				mode_flag = 0;
			LCD1602_write_cmd(LCD_SCREEN_CLR);
		}		
	} else
		KEY1_flag = 1;
	
	if (KEY2 == 0) {    //���ö�ʱ
		delay_ms(10);
		if (KEY2_flag) {
			KEY2_flag = 0;
			set_flag++;
			if(set_flag > 3)
				set_flag = 0;
			LCD1602_write_cmd(LCD_SCREEN_CLR);
		}
	} else
		KEY2_flag = 1;

	if (KEY3 == 0) {    //��
		delay_ms(10);
		if (KEY3_flag) {
			KEY3_flag = 0;
			if (set_flag) { //ʱ����ļӼ��߼�
				temp = 3 - set_flag;
				if (set_time[2] == 0x23 || set_time[temp] == 0x59) {
					set_time[temp] = 0x00;
				} else if ((set_time[temp] & 0x0F) == 0x09) {
					set_time[temp] -= 0x09; set_time[temp] += 0x10;
				} else {
					set_time[temp] += 0x01;
				}
			} else {
				aimed_pos += CURTAIN_STEP;
				if (aimed_pos > CURTAIN_OPEN)
					aimed_pos = CURTAIN_OPEN;
			}
		}
	} else
		KEY3_flag = 1;
	
	if (KEY4 == 0) {    //��
		delay_ms(10);
		if (KEY4_flag) {
			KEY4_flag = 0;
			if (set_flag) { //ʱ����ļӼ��߼�
				temp = 3 - set_flag;
				if (set_time[temp] == 0x00) {
					if (temp == 2)
						set_time[temp] = 0x23;
					else 
						set_time[temp] = 0x59;
				} else if ((set_time[temp] & 0x0F) == 0x00) {
					set_time[temp] -= 0x10; set_time[temp] += 0x09;
				} else {
					set_time[temp] -= 0x01;
				}
			} else {
				aimed_pos -= CURTAIN_STEP;
				if (aimed_pos < CURTAIN_CLOES)
					aimed_pos = CURTAIN_CLOES;
			}
		}
	} else
		KEY4_flag = 1;
	
	if (KEY5 == 0) {    //ģ������
		delay_ms(10);
		if (KEY5_flag) {
			KEY5_flag = 0;
			pos_flag++;
			if(pos_flag > 2)
				pos_flag = 0;
			
			if (pos_flag == 0)
				aimed_pos = CURTAIN_CLOES;         
			else if (pos_flag == 1)
				aimed_pos = CURTAIN_HALF;
			else
				aimed_pos = CURTAIN_OPEN;
			
			EX0 = 1;  //�����������ɼ�
		}
	} else
		KEY5_flag = 1;
}

void set_time_display(void)
{
	unsigned char str[16];
	str[0] = ' ';str[1] = 'S';str[2] = 'E';str[3] = 'T';
	str[4] = ':';
	if (set_flag == 1 && timer0_isr_flag) {
		str[5] = ' ';
		str[6] = ' ';
	} else {
		str[5] = (set_time[2] >> 4) + '0';     //Сʱ
		str[6] = (set_time[2] & 0x0F) + '0';
	}
	str[7] = ':';
	if (set_flag == 2 && timer0_isr_flag) {
		str[8] = ' ';
		str[9] = ' ';
	} else {
		str[8] = (set_time[1] >> 4) + '0';    //����
		str[9] = (set_time[1] & 0x0F) + '0';
	}
	str[10] = ':';
	if (set_flag == 3 && timer0_isr_flag) {
		str[11] = ' ';
		str[12] = ' ';
	} else {
		str[11] = (set_time[0] >> 4) + '0';   //��
		str[12] = (set_time[0] & 0x0F) + '0';
	}
	str[13] = ' ';
	LCD1602_show_str(0, 1, str);
}
//��λ����
void exit1_ISR() interrupt 2
{
	aimed_pos = mcount;
	mode_flag = 0;
	IN1 = 1; IN2 = 1;
	IN3 = 1; IN4 = 1;
	EX0 = 0;  //�رձ������ɼ�
}
