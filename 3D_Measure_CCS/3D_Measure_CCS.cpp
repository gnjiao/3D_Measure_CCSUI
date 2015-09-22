// 3D_Measure_CCS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"
#include "cAcqEasy.h"
#include "ccs_initial.h"
#include "ccs_clean.h"
#include "ccs_work.h"
#include <time.h>

int _tmain(int argc, _TCHAR* argv[])
{
	//Acquisition parameters
	sAcqEasyParam acqEasyParam;
	//Sensor ID������ID
	MCHR_ID SensorID = NULL;
	float vel = 0;                   //Xɨ���ٶ���ʼֵ����λmm/s
	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	int width = 4000;                    //Xɨ�貽��
	int height =400;                           //Yɨ�貽��
	int IntensityStart =80;                    //LED��ǿ��ʼֵ
	int IntensityStep = 0;                      //LED��ǿ����
	int IntensityCount = 1;//(100-IntensityStart)/IntensityStep+1;                     //LED���ǿ����
	int FrequencyStart =600;                   //Ƶ����ʼֵ
	int FrequencyStep = 0;                      //Ƶ������
	int FrequencyCount =1;//(2000-FrequencyStart)/FrequencyStep+1;                     //��Ƶ�ʴ���
	int DpiStart = 4000;                      //�ֱ���
	int DpiStep = 0;								//�ֱ�������
	int DpiCount = 1;							//�ֱ������Ӵ���
	char filename[25] = "006_4";	        //���ϱ��+���� 
	int average = 1;                            //averaging����
	int holdlast = 0;                            //holdlast����
	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	time_t rawtime;								//ԭʼʱ����Ϣ
	struct tm* timeinfo;						//�¼���Ϣ
	char Time[50];                              //ϵͳ����
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(Time,50,"%Y.%m.%d",timeinfo);	
	clock_t StartTime=0,FinishTime=0;
	double duration = 0;
	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	StartTime = clock();
	int x = 0;                                  //X�����������
	int y = 0;                                  //Y�����������
	int step = 0;                               //Y�Ĳ���
	char filePath_z[100];
	//dataĿ¼���ļ���+�����յ�dat�����ļ�
	sprintf_s(filePath_z,"%s%s%s%s%s",".\\data\\",filename,"_",Time,".dat");
	//��ʼ���ļ�
	FILE *File_z;
	if (fopen_s(&File_z,filePath_z,"wb"))
	{
		printf("���ļ�����");
		return false;
	}
	//��ʼ����Ĵ���
	if(!ges_initial())
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
	if(!ges_messure_zero(100,50))    //����ԭ�㣨100,50��mm
		printf("\n���ò���ԭ��ʧ�ܣ�\n");
	else
		printf("\n���ò���ԭ��ɹ���\n");
	//�ֱ��Բ�ͬ�ֱ��ʡ���ͬƵ�ʡ���ͬLED��ǿ���в���
	for (int k=0; k < DpiCount; k++)
	{
		for (int i=0; i<FrequencyCount; i++)
		{
			for (int j=0; j<IntensityCount; j++)
			{
				//�����൱��Ļ��㱣֤�ļ�������
				vel = 25.4/DpiValue*FrequencyValue;
				float test_t = (int)(vel*2000);
				vel = test_t/2000;//����Ϊ����
				fwrite(&vel,sizeof(float),1,File_z);
				x =average*(int)(vel*(float(width)/FrequencyValue)*2000+1000);//12.5/3.2*2000;
				step = average*(int)(vel*2/FrequencyValue*1000);                          //Yɨ�貽��

				if(!ccs_initial(&SensorID,&acqEasyParam,IntensityValue,FrequencyValue,width,average,holdlast))
					printf("��������ʼ��ʧ�ܣ�\n");
				else
					printf("��������ʼ���ɹ���\n");
				ccs_work(SensorID,&acqEasyParam,File_z,x,y,vel*2,step,height,width,countSum,&nowCount);
				if(!ccs_clean(&SensorID))
					printf("ֹͣ������ʧ�ܣ�\n");
				else
					printf("ֹͣ�������ɹ���\n");
				IntensityValue += IntensityStep;

				short rtn =0 ; //���������ķ���ֵ��0��ʾ�޴���
				printf("�ز���ԭ��...");
				if(GT_ADD_Buffer_continue(0,0,25))
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
	if(!ges_stop())
		printf("ֹͣ���ʧ�ܣ�\n");
	else
		printf("ֹͣ����ɹ���\n");
	fclose(File_z);
	printf("ɨ�������");
	FinishTime = clock();
	duration = (double)(FinishTime-StartTime)/1000;
	printf("��ʱ%f s",duration);
	getchar();
	return 0;
}

