#include "ges_measure_zero.h"	
#include "stdafx.h"
#include "ges_work.h"
#include <windows.h>
bool ges_messure_zero(int x,int y)
{
	int isMyCyc = false;
	int xSetPoint;
	int ySetPoint;
	bool MyPointFlag = false;
	//�ж�����x���ܴ���400��y���ܴ���350
	if( x>400 || y>350 )
	{
		printf("������������Χ��");
		return false;
	}

	xSetPoint = x*1000*2+10000;
	ySetPoint = y*1000*2+10000;

	short rtn =0; //���������ķ���ֵ��0��ʾ�޴���
	if(false == MyPointFlag)
	{
		rtn =GT_GetLmtZero_X();										//X���˶�������λ�ò�����Ϊ����ԭ��
		if(rtn!=0)
		{
			printf("X������ԭ��ʧ�ܣ�");
			return false;
		}
		//*******************************************
		//�����״̬
		/*(3)*/
		rtn = GT_Check_status();
		if(rtn!=0)
		{
			printf("�����״̬ʧ�ܣ�");
			return false;
		}

		Sleep(3000);
		//Sleep(xSetPoint*0.1);

		rtn =GT_GetLmtZero_Y();										//Y���˶�������λ�ò�����Ϊ����ԭ��
		if(rtn!=0)
		{
			printf("Y������ԭ��ʧ�ܣ�");
			return false;
		}

		//Sleep(ySetPoint*0.1);		

		//*******************************************
		//�����״̬
		/*(3)*/
		rtn = GT_Check_status();
		if(rtn!=0)
		{
			printf("�����״̬ʧ�ܣ�");
			return false;
		}

		Sleep(3000);

		MyPointFlag = TRUE;
	}

	if(FALSE == isMyCyc)
	{

		//************************************************************************
		//���ָ�����������Ҫ�˶��Ĺ켣��ʼ����X��Y��δ�˶���
		/*(2)*/
		rtn = GT_Buffer();
		if(rtn!=0)
		{
			printf("����������˶�ָ������ʧ�ܣ�");
			return false;
		}

		isMyCyc = TRUE;
	}

	//*******************************************
	//�����״̬
	/*(3)*/
	rtn = GT_Check_status();
	if(rtn!=0)
	{
		printf("�����״̬ʧ�ܣ�");
		return false;
	}

	Sleep(3000);//ԭֵ6000


	//����Ŀ�ĵز�������ʹ�����λ���˶�����һ�㣬�����˶�0(�����ǲ���Ϊ0)������
	//**************************************************************************************
	/*(4)*/
	rtn = GT_ADD_Buffer_continue(xSetPoint,ySetPoint,20.32);//Ĭ���ٶ�
	if(rtn!=0)
	{
		printf("���������ָ��ִ��ʧ�ܣ�");
		return false;
	}
	else
	{
		//***************************************************************************************
		/*(5)*/
		rtn = GT_Check_status();
		if(rtn!=0)
		{
			printf("�����״̬ʧ�ܣ�");
			return false;
		}	

		Sleep(3000);

		printf("�������ԭ�㣡");
	
	}
	return true;
}