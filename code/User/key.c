#include "key.h"

//���������� key_mode:0,��֧��������;1,֧��������; ����ֵ����ֵ
unsigned char chiclet_keyboard_scan(unsigned char key_mode)
{	 
	static unsigned char key_up=1;//�����ɿ���־

	if(key_mode)//mode=1,֧������
		key_up=1;
	
	if(key_up && (KEY1==0 || KEY2==0 || KEY3==0 || KEY4==0 || KEY5==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;

		if(KEY1==0)//����1���£�����1
			return KEY_OPEN_FLAG;
		else if(KEY2==0)//����2���£�����2
			return KEY_SET_FLAG;
		else if(KEY3==0)//����3���£�����3
			return KEY_ADD_FLAG;
		else if(KEY4==0)
			return KEY_REDUCE_FLAG;
		else if(KEY5==0)
			return KEY_SIMU_FLAG;
	} else //û�а������£��ɿ���־λ��1
		key_up=1;
	
	return 0;// �ް�������.����0
}
