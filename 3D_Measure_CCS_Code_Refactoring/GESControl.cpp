#include "StdAfx.h"
#include "GESControl.h"


GESControl::GESControl(void)
{
}


GESControl::~GESControl(void)
{
}

bool GESControl::ges_initial()
{
	short rtn =0 ; //���������ķ���ֵ��0��ʾ�޴���
	//**********************************************************************************
	//���ƽ̨��ʼ�� 
	//**************//�����ڵ�����Ϊ����ƽ̨���еĳ����ж��ٶΣ�ע�͵����к�����ſɵ������У�����
	//**************//�����ܹ�Ϊ(9)��
	//**********************************************************************************
	/*(1)*/
	rtn = GT_System_Initinal();									//ƽ̨��ʼ��
	if(rtn!=0)
	{
		printf("�����ʼ��ʧ�ܣ�\n");
		return false;

	}

	rtn = AxisInitial(2,15);									//�������ʼ������						
	if(rtn!=0)
	{
		printf("����������ʼ��ʧ�ܣ�\n");
		return false;
	}															   
	rtn = MotionInitial();										//���˶�������ʼ��
	if(rtn!=0)
	{
		printf("����������˶�������ʼ��ʧ�ܣ�\n");
		return false;
	}
	return true;
}

bool GESControl::ges_messure_zero(int x,int y)
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

bool GESControl::ges_stop()
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


