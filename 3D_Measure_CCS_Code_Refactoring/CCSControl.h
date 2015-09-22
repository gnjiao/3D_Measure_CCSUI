#pragma once

#include "Resource.h"
#include "CCSWork.h"

class CCSControl:public CCSWork
{
public:
	CCSControl(void);
	~CCSControl(void);
	//��ʼ��
	bool ccs_initial(MCHR_ID *SensorID,sAcqEasyParam* acqEasyParam,
		int intensity,int frequency,int buffer_width,int average,int holdlast);
	
	//�ر�
	bool ccs_clean(MCHR_ID *SensorID);

};

