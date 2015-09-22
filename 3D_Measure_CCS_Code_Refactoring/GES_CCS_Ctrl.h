#pragma once

#include "GESControl.h"
#include "CCSControl.h"

class GES_CCS_Ctrl
{
private:
	//Acquisition parameters
	sAcqEasyParam acqEasyParam;
	//Sensor ID������ID
	MCHR_ID SensorID ;
	float vel ;                   //Xɨ���ٶ���ʼֵ����λmm/s
	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	int width;                    //Xɨ�貽��
	int height;                           //Yɨ�貽��
	int IntensityStart ;                    //LED��ǿ��ʼֵ
	int IntensityStep ;                      //LED��ǿ����
	int IntensityCount ;//(100-IntensityStart)/IntensityStep+1;                     //LED���ǿ����
	int FrequencyStart ;                   //Ƶ����ʼֵ
	int FrequencyStep ;                      //Ƶ������
	int FrequencyCount ;//(2000-FrequencyStart)/FrequencyStep+1;                     //��Ƶ�ʴ���
	int DpiStart ;                              //�ֱ���
	int DpiStep ;							//�ֱ�������
	int DpiCount ;							//�ֱ������Ӵ���
	char filename[25] ;	            //���ϱ��+���� 
	int average ;                               //averaging����
	int holdlast ;                            //holdlast����
	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

	GESControl ges;
	CCSControl ccs;

	short rtn; //���������ķ���ֵ��0��ʾ�޴���

	//Acquisition user buffer
	float *pAltitude;
	float *pIntensity;
	int DataCount;
	short resultProcess;
public:
	GES_CCS_Ctrl(void);
	~GES_CCS_Ctrl(void);
	
	bool ges_ccs_control(sAcqEasyParam* acqEasyParam,MCHR_ID SensorID,
		HANDLE evAcquisition,cAcqEasy* pAcqEasy,
		FILE *File_z,int x,int y,int step,float vel,int height,int width,int countSum,int* nowCount);

	//����
	bool ccs_work(MCHR_ID SensorID,sAcqEasyParam* acqEasyParam,
		FILE*File_z,int x,int y,int vel,int step,int height,int width,int countSum,int* nowCount);

	void WorkingProcess(void);

};

