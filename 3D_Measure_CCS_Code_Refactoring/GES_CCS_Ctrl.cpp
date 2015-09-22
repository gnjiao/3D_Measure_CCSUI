#include "StdAfx.h"
#include "GES_CCS_Ctrl.h"

#include "DataSave.h"

GES_CCS_Ctrl::GES_CCS_Ctrl(void)
{
	ges=GESControl();
	ccs=CCSControl();
	short rtn =0 ; //���������ķ���ֵ��0��ʾ�޴���

	//Acquisition user buffer
	float *pAltitude = NULL;
	float *pIntensity = NULL;
	int DataCount = 0;
	short resultProcess = 0;

	//Acquisition parameters
	//sAcqEasyParam acqEasyParam;
	//Sensor ID������ID
	MCHR_ID SensorID = NULL;
	float vel = 0;                   //Xɨ���ٶ���ʼֵ����λmm/s
	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	width = 4000;                    //Xɨ�貽��
	height =400;                           //Yɨ�貽��
	IntensityStart =80;                    //LED��ǿ��ʼֵ
	IntensityStep = 0;                      //LED��ǿ����
	IntensityCount = 1;//(100-IntensityStart)/IntensityStep+1;                     //LED���ǿ����
	FrequencyStart =600;                   //Ƶ����ʼֵ
	FrequencyStep = 0;                      //Ƶ������
	FrequencyCount =1;//(2000-FrequencyStart)/FrequencyStep+1;                     //��Ƶ�ʴ���
	DpiStart = 4000;                              //�ֱ���
	DpiStep = 0;							//�ֱ�������
	DpiCount = 1;							//�ֱ������Ӵ���
	strcpy(filename, "006_4");	            //���ϱ��+���� 
	average = 1;                               //averaging����
	holdlast = 0;                            //holdlast����
}


GES_CCS_Ctrl::~GES_CCS_Ctrl(void)
{
}


bool GES_CCS_Ctrl::ges_ccs_control(sAcqEasyParam* acqEasyParam,MCHR_ID SensorID,
	HANDLE evAcquisition,cAcqEasy* pAcqEasy,
	FILE *File_z,int x,int y,int step,float vel,int height,int width,int countSum,int* nowCount)
{

	rtn = ges.GT_System_Initinal();						//ƽ̨��ʼ��
	if(rtn!=0)
	{
		printf("�����ʼ��ʧ�ܣ�");
		return false;
	}

	rtn = ges.AxisInitial(2,15);							//�������ʼ������						
	if(rtn!=0)
	{
		printf("����������ʼ��ʧ�ܣ�");

		return false;

	}															   
	rtn = ges.MotionInitial();							//���˶�������ʼ��
	if(rtn!=0)
	{
		printf("����������˶�������ʼ��ʧ�ܣ�");
		return false;
	}

	//���ָ�����������Ҫ�˶��Ĺ켣��ʼ����X��Y��δ�˶���
	/*(2)*/
	rtn = ges.GT_Buffer();

	if(rtn!=0)
	{

		printf("����������˶�ָ������ʧ�ܣ�");
		return false;
	}

	for(int i=1;i<=height;i++)
	{
		(*nowCount)++;
		printf("����� %.2f %% !!!\n",((*nowCount)*1.0/countSum*100));
		rtn = ges.GT_Check_status();
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
		ges.GT_ADD_Buffer_trigger(x,y,vel);//�ٶ�12.5mm/s
		Sleep(10);
		GT_ExOpt(0x0000);
		//����CCS
		resultProcess = ccs.Process(pAcqEasy, evAcquisition, *acqEasyParam,
			pAltitude, pIntensity,&DataCount); 
		if (resultProcess == STATUS_NO_ERROR)
		{
			//��������
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
			rtn = ges.GT_Check_status();
			if(rtn!=0)
			{
				printf("�����״̬ʧ�ܣ�");
				return false;
			}	
		}
		rtn = ges.GT_ADD_Buffer_continue(0,y,100);
		if(rtn!=0)
		{
			printf("���������ָ��ִ��ʧ�ܣ�");
			return false;
		}
		else
		{
			rtn = ges.GT_Check_status();
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

bool GES_CCS_Ctrl::ccs_work(MCHR_ID SensorID,sAcqEasyParam* acqEasyParam, FILE*File_z,int x,int y,int vel,int step,int height,int width,int countSum,int* nowCount)
{
	//User Event
	HANDLE evAcquisition = NULL;
	//cAcqEasy Class
	cAcqEasy* pAcqEasy = NULL;
	//create cAcqEasy class
	pAcqEasy = new cAcqEasy(SensorID);
	if (ccs.CreateEvents(pAcqEasy, &evAcquisition))
	{				
		//start acquisition
		printf ("Start Acquisition\n");
		if (ccs.StartAcquisition(pAcqEasy, *acqEasyParam))
		{
			ges_ccs_control(acqEasyParam,SensorID,evAcquisition,pAcqEasy,File_z,x,y,step,(float)vel,height,width,countSum,nowCount);
			//GT_ExOpt(0x0000);       //�ⲿ����Exo6��0
			//Sleep(1000);
			//printf ("Allocate user buffer memory\n");
			//pAltitude = new float[buffer_width];
			//pCounter = new float[buffer_width];
			//pIntensity = new float[buffer_width];
			////read acquisition result
			//GT_ADD_Buffer_trigger(test_x,test_y,test_v);//�ٶ�12.5mm/s
			//resultProcess = Process(pAcqEasy, evAcquisition, *acqEasyParam, pAltitude, pCounter, pIntensity,&DataCount); 
			//if (resultProcess == STATUS_NO_ERROR)
			//{
			//	if (!DataSave(File_z,pAltitude,1,buffer_width))
			//		printf("DataSave Error!");
			//	if (!DataSave(File_z,pIntensity,1,buffer_width))
			//		printf("DataSave Error!");
			//}	
			//else
			//{
			//	if (resultProcess != STATUS_TIMEOUT_ERROR)
			//	{
			//		printf("Main : Process() : Error 0x%X\n", resultProcess);
			//	}
			//}
			//delete[] pAltitude;
			//delete[] pCounter;
			//delete[] pIntensity;
		}
		printf ("Stop Acquisition\n");
		pAcqEasy->StopAcquisition();	
		ccs.ReleaseEvents(&evAcquisition);
	}
	delete pAcqEasy;
	return true;
}

void GES_CCS_Ctrl::WorkingProcess(void)
{
	time_t rawtime;
	struct tm* timeinfo;
	char Time[50];                              //ϵͳ����
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(Time,50,"%Y.%m.%d",timeinfo);	
	clock_t StartTime=0,FinishTime=0;
	double duration = 0;

	StartTime = clock();
	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	int x = 0;                                  //X�����������
	int y = 0;                                  //Y�����������
	int step = 0;                               //Y�Ĳ���
	char filePath_z[100];
	sprintf_s(filePath_z,"%s%s%s%s%s",".\\data\\",filename,"_",Time,".dat");
	FILE *File_z;
	if (fopen_s(&File_z,filePath_z,"wb"))
	{
		printf("���ļ�����");
		return;
	}
	if(!ges.ges_initial())
		printf("�����ʼ��ʧ�ܣ�\n");
	else
		printf("�����ʼ���ɹ���\n");
	int FrequencyValue = FrequencyStart;
	int IntensityValue = IntensityStart;
	int DpiValue = DpiStart;
	fwrite(&width,sizeof(int),1,File_z);
	fwrite(&DpiStart,sizeof(int),1,File_z);
	fwrite(&DpiStep,sizeof(int),1,File_z);
	fwrite(&DpiCount,sizeof(int),1,File_z);
	fwrite(&height,sizeof(int),1,File_z);
	fwrite(&IntensityStart,sizeof(int),1,File_z);
	fwrite(&IntensityStep,sizeof(int),1,File_z);
	fwrite(&IntensityCount,sizeof(int),1,File_z);
	fwrite(&FrequencyStart,sizeof(int),1,File_z);
	fwrite(&FrequencyStep,sizeof(int),1,File_z);
	fwrite(&FrequencyCount,sizeof(int),1,File_z);
	fwrite(&average,sizeof(int),1,File_z);
	//float resolution = 25.4 / (test_v / FrequencyStart * average);
	//fwrite(&resolution,sizeof(int),1,File_z);
	int countSum = DpiCount * height * FrequencyCount * IntensityCount; //�����������
	int nowCount = 0;                                                     //��ǰ�����������
	printf("���ڲ���ԭ��...");
	if(!ges.ges_messure_zero(100,50))    //����ԭ�㣨100,50��mm
		printf("\n���ò���ԭ��ʧ�ܣ�\n");
	else
		printf("\n���ò���ԭ��ɹ���\n");
	for (int k=0; k < DpiCount; k++)
	{
		for (int i=0; i<FrequencyCount; i++)
		{
			for (int j=0; j<IntensityCount; j++)
			{
				vel = (int)(25.4/DpiValue*FrequencyValue);
				float test_t = (float)(vel*2000);
				vel = test_t/2000;//����Ϊ����
				fwrite(&vel,sizeof(float),1,File_z);
				x =average*(int)(vel*(float(width)/FrequencyValue)*2000+1000);//12.5/3.2*2000;
				step = average*(int)(vel*2/FrequencyValue*1000);                          //Yɨ�貽��

				if(!ccs.ccs_initial(&SensorID,&acqEasyParam,IntensityValue,FrequencyValue,width,average,holdlast))
					printf("��������ʼ��ʧ�ܣ�\n");
				else
					printf("��������ʼ���ɹ���\n");
				ccs_work(SensorID,&acqEasyParam,File_z,x,y,(int)vel*2,step,height,width,countSum,&nowCount);
				if(!ccs.ccs_clean(&SensorID))
					printf("ֹͣ������ʧ�ܣ�\n");
				else
					printf("ֹͣ�������ɹ���\n");
				IntensityValue += IntensityStep;

				short rtn =0 ; //���������ķ���ֵ��0��ʾ�޴���
				printf("�ز���ԭ��...");
				if(ges.GT_ADD_Buffer_continue(0,0,25))
					printf("��ԭ��ʧ�ܣ�\n");
				else 
					printf("��ԭ��ɹ�!\n");

			}
			FrequencyValue += FrequencyStep;
			IntensityValue = IntensityStart;
		}
		FrequencyValue = FrequencyStart;
		IntensityValue = IntensityStart;
		DpiValue += DpiStep;
	}
	if(!ges.ges_stop())
		printf("ֹͣ���ʧ�ܣ�\n");
	else
		printf("ֹͣ����ɹ���\n");
	fclose(File_z);
	printf("ɨ�������");
	FinishTime = clock();
	duration = (double)(FinishTime-StartTime)/1000;
	printf("��ʱ%f s",duration);
	getchar();
}
