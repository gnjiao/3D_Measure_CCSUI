#include "stdafx.h"
#include "ges_stop.h"

bool ges_stop()
{
    //**********************************************************************************
	//���ֹͣ
	/*(9)*/
	short rtn =0 ; //���������ķ���ֵ��0��ʾ�޴���
	rtn = GT_Stop();	//�رտ������ŷ�ʹ��
	if(rtn!=0)
	{
		printf("�رտ������ŷ�ʹ��ʧ�ܣ�");
		return false;
	}
	rtn = GT_Close();						//�ر��˶�������

	if(rtn!=0)
	{
		error(rtn);
		printf("�ر��˶�������ʧ�ܣ�");
	    return false;
	}
	return true;
}