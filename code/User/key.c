#include "key.h"

//按键处理函数 key_mode:0,不支持连续按;1,支持连续按; 返回值：键值
unsigned char chiclet_keyboard_scan(unsigned char key_mode)
{	 
	static unsigned char key_up=1;//按键松开标志

	if(key_mode)//mode=1,支持连按
		key_up=1;
	
	if(key_up && (KEY1==0 || KEY2==0 || KEY3==0 || KEY4==0 || KEY5==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;

		if(KEY1==0)//按键1按下，返回1
			return KEY_OPEN_FLAG;
		else if(KEY2==0)//按键2按下，返回2
			return KEY_SET_FLAG;
		else if(KEY3==0)//按键3按下，返回3
			return KEY_ADD_FLAG;
		else if(KEY4==0)
			return KEY_REDUCE_FLAG;
		else if(KEY5==0)
			return KEY_SIMU_FLAG;
	} else //没有按键按下，松开标志位置1
		key_up=1;
	
	return 0;// 无按键按下.返回0
}
