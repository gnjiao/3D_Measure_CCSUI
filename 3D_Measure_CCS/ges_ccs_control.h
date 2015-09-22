#ifndef GES_CCS_CONTROL_H
#define GES_CCS_CONTROL_H
#include "stdio.h"
#include "ges_work.h"
#include "cAcqEasyType.h"
#include "cAcqEasy.h"
#include "afxmt.h"
#include "DataSave.h"
bool ges_ccs_control(sAcqEasyParam* acqEasyParam,MCHR_ID SensorID,HANDLE evAcquisition,cAcqEasy* pAcqEasy,FILE *File_z,int x,int y,int step,float vel,int height,int width,int countSum,int* nowCount)
{
	short rtn =0 ; //���������ķ���ֵ��0��ʾ�޴���
		
	//Acquisition user buffer
	float *pAltitude = NULL;
	float *pIntensity = NULL;
    int DataCount = 0;
    short resultProcess = 0;
	rtn = GT_System_Initinal();									//ƽ̨��ʼ��
	if(rtn!=0)
	{
		printf("�����ʼ��ʧ�ܣ�");
		return false;
	}

	rtn = AxisInitial(2,15);									//�������ʼ������						
	if(rtn!=0)
	{
		printf("����������ʼ��ʧ�ܣ�");

		return false;

	}															   
	rtn = MotionInitial();										//���˶�������ʼ��
	if(rtn!=0)
	{
		printf("����������˶�������ʼ��ʧ�ܣ�");
		return false;
	}

	//���ָ�����������Ҫ�˶��Ĺ켣��ʼ����X��Y��δ�˶���
	/*(2)*/
	rtn = GT_Buffer();

	if(rtn!=0)
	{

		printf("����������˶�ָ������ʧ�ܣ�");
		return false;
	}

	for(int i=1;i<=height;i++)
	{
		(*nowCount)++;
		printf("����� %.2f %% !!!\n",((*nowCount)*1.0/countSum*100));
		rtn = GT_Check_status();
		if(rtn!=0)
		{
			printf("�����״̬ʧ�ܣ�");
			return false;
		}
		GT_ExOpt(0x0000);       //�ⲿ����Exo6��0
		Sleep(10);
		pAltitude = new float[width];
		pIntensity = new float[width];
		//read acquisition result
		GT_ADD_Buffer_trigger(x,y,vel);//�ٶ�12.5mm/s
		Sleep(10);
		GT_ExOpt(0x0000);
		resultProcess = Process(pAcqEasy, evAcquisition, *acqEasyParam, pAltitude, pIntensity,&DataCount); 
		if (resultProcess == STATUS_NO_ERROR)
		{
			if (!DataSave(File_z,pAltitude,1,width))
				printf("DataSave Error!");
			if (!DataSave(File_z,pIntensity,1,width))
				printf("DataSave Error!");
		}	
		else
		{
			if (resultProcess != STATUS_TIMEOUT_ERROR)
			{
				printf("Main : Process() : Error 0x%X\n", resultProcess);
			}
		}
		
		y = y + step;//12.5um	pluse = 0.5um
		if(rtn!=0)
		{
			printf("���������ָ��ִ��ʧ�ܣ�");
			return false;
		}
		else
		{
			rtn = GT_Check_status();
			if(rtn!=0)
			{
				printf("�����״̬ʧ�ܣ�");
				return false;
			}	
		}
		rtn = GT_ADD_Buffer_continue(0,y,100);
		if(rtn!=0)
		{
			printf("���������ָ��ִ��ʧ�ܣ�");
			return false;
		}
		else
		{
			rtn = GT_Check_status();
			if(rtn!=0)
			{
				printf("�����״̬ʧ�ܣ�");
				return false;
			}	
			Sleep(1000);//?
		}
		delete[] pAltitude;
		delete[] pIntensity;
	}
	return true;
}
#endif