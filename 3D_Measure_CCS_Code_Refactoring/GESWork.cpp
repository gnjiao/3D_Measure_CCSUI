#include "StdAfx.h"
#include "GESWork.h"
#include <time.h>
#include "afxmt.h"
GESWork::GESWork(void)
{
}

GESWork::~GESWork(void)
{
}

void GESWork::error(short rtn)				//��������Ӻ���   ����ֵ����ĺ���ο��ĵ�
{ 
	switch(rtn) 
	{
	case -1: 
		printf("Communication Error !"); 
		break; 
	case 0: 
		break; 
	case 1: 
		printf("Command Error !");
		break; 
	case 2: 
		printf("Radius or chord is 0 !");
		break; 
	case 3: 
		printf("Length is 0 or overflow !"); 
		break; 
	case 4: 
		printf("Velocity or acceleration is less then 0 !"); 
		break; 
	case 5: 
		printf("Chord is greater than diameter !"); 
		break; 
	case 7: 
		printf("Parameter error !");
		break; 
	default: 
		printf("Error Code Unknow�� ");
		break; 
	}
}


short GESWork::GT_System_Initinal()				//ƽ̨��ʼ��
{
	short rtn; 
	rtn = GT_Open();							//���˶������� 
	error(rtn);
	rtn = GT_Reset();						//��λ�˶�������
	error(rtn);

	return rtn;
}

short GESWork::AxisInitial(short axis_num,unsigned short limit)		//�������ʼ������
{					

	short rtn; 
	rtn=GT_LmtSns(limit);	//������λ���ش�����ƽ			
	error(rtn); 
	rtn = GT_LmtsOn(1);		//1����λ��Ч
	error(rtn);
	rtn = GT_LmtsOn(2);		//2����λ��Ч
	error(rtn);

	rtn=GT_EncSns(0);		//��������������  Ĭ��
	error(rtn); 


	rtn=GT_HomeSns(0);		//Home �źŴ�����  Ĭ��
	error(rtn); 

	rtn=GT_AlarmOff(1);						//�ر���������			
	error(rtn);
	rtn=GT_AlarmOff(2);						//�ر���������			
	error(rtn);

	for(short axis_i=1;axis_i<=axis_num;++axis_i)		 //2�� axis_i = 2 
	{
		rtn=GT_CtrlMode(axis_i,1);				//���ÿ�����Ϊ�������ģʽ 
		error(rtn);
		rtn=GT_StepDir(axis_i);					   //�������������/�����ź�			*****����/����
		error(rtn); 
		rtn=GT_AxisOn(axis_i);					  //����ʹ��
		error(rtn);
		rtn=GT_ClrSts(axis_i);					  //������״̬�Ĵ�����λ
		error(rtn);

		Sleep(200);							//�����ʵ���ʱ 

	} 

	return rtn;
}

short GESWork::MotionInitial()								//���˶�������ʼ��
{ 
	short rtn;
	double StartVel, MaxVel, StopAcc;				//�����ٶ� ����ٶ� �����ٶ�
	double WorkVel, WorkAcc;						//�����ٶ�  �������ٶ�

	StartVel=0.5;									//�����ٶ�Ϊ500HZ
	MaxVel=200;										//����ٶ�Ϊ6m/min 
	StopAcc=6;										//��ͣ���ٶ�Ϊ3m/s2 
	WorkVel=20.32;									//�������ٶ�Ϊ24.5/5  mm/s    WorkVel=10.16 P/ms											
	WorkAcc=1.8;									//�������ٶ�Ϊ0.9m/s2



	rtn=GT_SetStrtVel(StartVel);					//����ϵͳ�����ٶ� 
	error(rtn); 
	rtn=GT_SetMaxVel(MaxVel);						//����ϵͳ����ٶ�
	error(rtn);
	rtn=GT_SetStpAcc(StopAcc);						//����ϵͳ��ͣ���ٶ� 
	error(rtn);

	rtn=GT_SetSynAcc(WorkAcc);						//���ý������ٶ�
	error(rtn); 
	rtn=GT_SetSynVel(WorkVel);						//���ý����ٶ�
	error(rtn);
	return rtn;
}




short GESWork::GT_Buffer() //�������˶�ָ���ź���
{

	short rtn;
	rtn=GT_StrtList(); 
	error(rtn);

	rtn=GT_MvXY(0,0, 10.16,3.6);
	error(rtn);						//��λ�˶��������//�����ٶ�Ϊ25.4/5 mm/s

	rtn=GT_LnXY(0,1);  
	error(rtn);

	rtn=GT_EndList(); //�رջ����� 
	error(rtn); 
	rtn=GT_StrtMtn(); //����������

	error(rtn);
	return rtn;
}

short GESWork::GT_Stop()							  //�رտ������ŷ�ʹ��
{
	short rtn;
	rtn=GT_AxisOff(1);					  //�����ر�		
	error(rtn);
	rtn=GT_AxisOff(2);					  //�����ر�
	error(rtn);
	return rtn;
}




short GESWork::GT_ADD_Buffer_continue(double AxisX_1, double AxisY_1,double Vel)//��ӻ�����ָ��
{
	short rtn;

	rtn=GT_AddList();//�����˶�����
	error(rtn); 

	rtn=GT_SetSynVel(Vel);//���������ٶ�
	error(rtn);

	rtn=GT_LnXY(AxisX_1,AxisY_1); //�˶�������
	error(rtn);

	rtn=GT_EndList(); //�رջ����� 
	error(rtn);

	rtn=GT_StrtMtn(); //����������
	error(rtn);

	return rtn;

}

short GESWork::GT_ADD_Buffer_trigger(double AxisX_1, double AxisY_1,double Vel)//��ӻ�����ָ��
{
	short rtn;

	rtn=GT_AddList();//�����˶�����
	error(rtn); 

	rtn=GT_SetSynVel(Vel);//���������ٶ�
	error(rtn);

	rtn = GT_ExOpt(0x0040);       //�ⲿ����Exo6
	error(rtn);

	rtn=GT_LnXY(AxisX_1,AxisY_1); //�˶�������
	error(rtn);

	rtn=GT_EndList(); //�رջ����� 
	error(rtn);

	rtn=GT_StrtMtn(); //����������
	error(rtn);

	return rtn;

}
short GESWork::GT_Check_status()	//����˶�״̬�Ƿ�ֹͣ
{	
	short rtn;
	unsigned short status;
	rtn=GT_GetCrdSts(&status);
	error(rtn);													//��status�ĵ�1λΪ0ʱ�����˶�)
	while(1)													//��status�ĵ�1λΪ1ʱ�����˶�
	{			
		if((status&0x01)==0x01)
			break;
		rtn=GT_GetCrdSts(&status);
		error(rtn);

	}

	return rtn;

}

short GESWork::GT_GetLmtZero_X()//����λ��Ϊ����ԭ��
{
	short axis_num = 1;
	short rtn;
	unsigned short status;
	double prf_pos[2]={0,0};


	rtn=GT_SetSynVel(100);						//���ý����ٶ�
	error(rtn);

	prf_pos[(axis_num-1)] = -1000000;//�˶�һ�ξ���ʹ�䵽�Ｋ��λ��

	rtn=GT_LnXY(prf_pos[0],prf_pos[1]);
	error(rtn);

	Sleep(100);

	rtn=GT_GetSts(axis_num,&status); //��ȡ������״̬
	error(rtn);

	while((status&0x40)!=0x40) // �ж��Ƿ񵽴Ｋ��λ��
	{
		GT_GetSts(axis_num,&status); //��ȡ������״̬
		if((status&0x400)==0)
		{
			printf("�˶������δ������λ��\n");
			return  1;
		}
	}

	rtn=GT_StpMtn();					//ƽ��ֹͣ
	error(rtn);
	Sleep(3000);

	rtn=GT_ZeroPos(axis_num);			//����̨λ�����㽨����������ϵ
	error(rtn);
	Sleep(100);
	//**********************************************************************************
	rtn=GT_SetSynVel(50);				//���ý����ٶ�
	error(rtn);

	//�ƶ�10mm��3����
	prf_pos[axis_num-1] =30000;			//������һС�ξ����Ա������λ��־λ
	//ԭֵ 300000								//ע��Ҫ��Sleep����ʱʱ����ƥ��

	rtn=GT_LnXY(prf_pos[0],prf_pos[1]); 
	error(rtn);	
	Sleep(2000);//ԭֵ30000
	//**********************************************************************************
	//**********************************************************************************
	//����һ�Σ������ϵ��һ�ε�x�����
	/*
	rtn=GT_ClrSts(axis_num);			//�����λ״̬��־λ
	error(rtn);
	Sleep(1000);

	rtn=GT_ZeroPos(axis_num);			//����̨λ�����㽨����������ϵ
	error(rtn);
	Sleep(100);

	rtn=GT_SetSynVel(10.16);				//���ý����ٶ�
	error(rtn);
	Sleep(100);

	prf_pos[(axis_num-1)] = -1000000;//�˶�һ�ξ���ʹ�䵽�Ｋ��λ��

	rtn=GT_LnXY(prf_pos[0],prf_pos[1]); 
	error(rtn);

	Sleep(100);

	rtn=GT_GetSts(axis_num,&status); //��ȡ������״̬
	error(rtn);

	while((status&0x40)!=0x40) // �ж��Ƿ񵽴Ｋ��λ��
	{
	GT_GetSts(axis_num,&status); //��ȡ������״̬
	if((status&0x400)==0)
	{
	AfxMessageBox(_T("�˶������δ������λ��\n"));
	return  1;
	}
	}

	rtn=GT_StpMtn();					//ƽ��ֹͣ
	error(rtn);
	Sleep(3000);

	rtn=GT_ZeroPos(axis_num);			//����̨λ�����㽨����������ϵ
	error(rtn);
	Sleep(100);
	//**********************************************************************************��
	//�ڸ�λ��Ҫ������ԭ��ƥ��

	prf_pos[axis_num-1] = 200000;			//������һС�ξ����Ա������λ��־λ
	//ע��Ҫ��Sleep����ʱʱ����ƥ��

	rtn=GT_LnXY(prf_pos[0],prf_pos[1]); 
	error(rtn);
	Sleep(20000);
	*/
	//**********************************************************************************
	//**********************************************************************************
	//**********************************************************************************

	rtn=GT_ZeroPos(axis_num);			//����̨λ�����㽨����������ϵ
	error(rtn);
	Sleep(100);

	rtn=GT_ClrSts(axis_num);			//�����λ״̬��־λ
	error(rtn);
	Sleep(1000);

	return rtn;
}


short GESWork::GT_GetLmtZero_Y()//����λ��Ϊ����ԭ��
{
	short rtn;
	short axis_num = 2;
	unsigned short status;
	double prf_pos[2]={0,0};

	rtn=GT_SetSynVel(100);						//���ý����ٶ�
	error(rtn);

	prf_pos[(axis_num-1)] = -1000000;//�˶�һ�ξ���ʹ�䵽�Ｋ��λ��

	rtn=GT_LnXY(prf_pos[0],prf_pos[1]);
	error(rtn);

	Sleep(100);

	rtn=GT_GetSts(axis_num,&status); //��ȡ������״̬
	error(rtn);

	while((status&0x40)!=0x40) // �ж��Ƿ񵽴Ｋ��λ��
	{
		GT_GetSts(axis_num,&status); //��ȡ������״̬
		if((status&0x400)==0)
		{
			printf("�˶������δ������λ��\n");
			return  1;
		}
	}

	rtn=GT_StpMtn();					//ƽ��ֹͣ
	error(rtn);
	Sleep(3000);

	rtn=GT_ZeroPos(axis_num);			//����̨λ�����㽨����������ϵ
	error(rtn);
	Sleep(100);
	//**********************************************************************************	
	rtn=GT_SetSynVel(50);				//���ý����ٶ�
	error(rtn);
	//�ƶ�50mm��5����
	prf_pos[axis_num-1] = 150000;			//������һС�ξ����Ա������λ��־λ
	//ԭֵ500000										//ע��Ҫ��Sleep����ʱʱ����ƥ��
	//**********************************************************************************
	rtn=GT_LnXY(prf_pos[0],prf_pos[1]); 
	error(rtn);

	Sleep(4000);//ԭֵ50000


	//**********************************************************************************
	rtn=GT_ZeroPos(axis_num);			//����̨λ�����㽨����������ϵ
	error(rtn);
	Sleep(100);

	rtn=GT_ClrSts(axis_num);			//�����λ״̬��־λ
	error(rtn);
	Sleep(1000);

	return rtn;
}





//********************************************************************************************************
//********************************************************************************************************
//*****************
//���º���δ��
//
//*****************
short GESWork::GT_Read_AxisNum()					  //��ȡ��������ǰָ��ִ�е������ı��
{	
	short rtn;
	unsigned short number;
	rtn=GT_GetMtnNm(&number);				//��ȡ��ǰ�켣�α��
	return number;

}


short GESWork::GT_GetPosition()						//��ȡ�ᵱǰλ��
{
	short rtn ;
	long atlPosX,atlPosY;

	rtn=GT_AxisOff(1);					    //�����ر�		�ȹر���ſ��Թرձ�����
	error(rtn);
	rtn=GT_AxisOff(2);					    //�����ر�
	error(rtn);

	rtn=GT_EncOn(1);						//��1�������
	error(rtn); 
	rtn=GT_EncOn(2);						//��2�������
	error(rtn); 

	GT_GetAtlPos(1, &atlPosX);				//��ȡ�������ʵ��λ��   �����ȿ�������
	GT_GetAtlPos(2, &atlPosY);

	printf("\nActual Position = %ld \n ", atlPosX);
	printf("\nActual Position = %ld \n ", atlPosY);

	rtn=GT_EncOff(1);	                    //�ر�1�������
	error(rtn); 
	rtn=GT_EncOff(2);						//�ر�2�������
	error(rtn); 

	rtn=GT_AxisOn(1);					    //����ʹ��
	error(rtn);
	rtn=GT_AxisOn(2);					    //����ʹ��
	error(rtn);

	return rtn;
}


short GESWork::GT_Stop_AxisNum(short Set_AxisNum)//ֹͣ������ָ���˶�����Set_AxisNum-1��
{
	short rtn,Stop_AxisNum; 
	unsigned short status;
	rtn=GT_GetCrdSts(&status);
	error(rtn);
	while((status&0x01)==0x00)//��status�ĵ�1λΪ1ʱ�����˶�
	{//��status�ĵ�1λΪ0ʱ�����˶�
		rtn=GT_GetCrdSts(&status);
		error(rtn);
		Stop_AxisNum=GT_Read_AxisNum();

		if(Stop_AxisNum==Set_AxisNum)
		{
			rtn=GT_StpMtn();								//ƽ��ֹͣ�켣�˶�
			return rtn;
		}

	}
	return rtn;
}


short GESWork::Home(unsigned short axis,long pos,long offset,double vel_high,double vel_low) 
{ 
	short rtn;
	double prf_pos[4];
	unsigned short status,crd_status; 
	GT_ClrSts(axis);//���ԭ����״̬ 
	GT_CaptHome(axis); //����ԭ��������Ϊ 
	//ԭ�㲶��ʽ 
	GT_SetSynVel(vel_high); //���û�ԭ����ٶ� 
	GT_SetSynAcc(0.01); //���û�ԭ��ļ��ٶ� 
	GT_GetPrfPnt(prf_pos); //��ȡ����Ĺ滮λ�� 
	prf_pos[axis-1] = pos; //���û�ԭ�����Ŀ��λ��
	GT_LnXY(prf_pos[0],prf_pos[1]);

	do 
	{ 
		GT_GetCrdSts(&crd_status); //��ȡ����ϵ״̬ 
		if(crd_status&1) //����˶��Ѿ����
		{ //��Ȼû�д���Home�ź� 
			return 1; //���ش������1 
		} 
		GT_GetSts(axis,&status); //��ȡ������״̬
	}while(!(status&0x8)); //�ȴ�Home�źŴ��� 


	GT_StpMtn(); //ƽ��ֹͣ


	do
	{ 
		GT_GetCrdSts(&crd_status); //��ȡ����ϵ״̬
	}while(!(crd_status&1)); //�ȴ��˶���� 


	GT_GetCapt(axis,&pos); //��ȡ����λ��
	GT_SetSynVel(vel_low); //�����˶���ԭ�� 
	prf_pos[axis-1]= pos;
	rtn=GT_LnXY(prf_pos[0],prf_pos[1]); 


	do
	{ 
		GT_GetCrdSts(&crd_status); //��ȡ����ϵ״̬
	}while(!(crd_status&1)); //�ȴ��˶���� 


	prf_pos[axis-1]+= offset; //�˶�һС�ξ����뿪Home����
	GT_LnXY(prf_pos[0],prf_pos[1]);


	do 
	{ 
		GT_GetCrdSts(&crd_status); //��ȡ����ϵ״̬
	}while(!(crd_status&1)); //�ȴ��˶����


	Sleep(1000); //�����ʵ���ʱ���ȴ�����̨ͣ�� 
	GT_ZeroPos(axis); //����̨λ�����㽨����������ϵ
	return 0; 
}

short GESWork::GT_GetStartZero()//�ص��ϵ�ʱ������(һ��Ҫ���ڵ���GT_GetLmtZero��������ϵͳ��������ԭ���һص���ԭ��󷽿ɵ��øú����ص��ϵ�ʱ��ԭ��)
{
	short rtn;

	rtn=GT_AddList();//�����˶�����
	error(rtn);

	rtn=GT_LnXY(0,0); //�˶�������
	error(rtn);

	rtn=GT_LnXY(0,437120);//�˶�������
	error(rtn);

	rtn=GT_EndList(); //�رջ����� 
	error(rtn);

	rtn=GT_StrtMtn(); //����������
	error(rtn);

	return rtn;



}



short GESWork::GT_GetZero()//���㣬�ص�����տ�ʼ�����(ϵͳ���Ϊ���ϵ�ʱ������)
{

	short rtn;

	rtn=GT_AddList();//�����˶�����
	error(rtn); 

	rtn=GT_SetSynVel(40.64);//���������ٶ�
	error(rtn);
	rtn=GT_LnXY(0,0); //�˶�������
	error(rtn);

	rtn=GT_EndList(); //�رջ����� 
	error(rtn);

	rtn=GT_StrtMtn(); //����������
	error(rtn);

	return rtn;

}
/*
short GESWork::GT_Check_Limit()//��λ��־���
{
short rtn;
unsigned short statusX,statusY;
rtn=GT_GetSts(1,&statusX);				//��ȡָ����������λ��������ԭ�㲶���ź�״̬
error(rtn);
rtn=GT_GetSts(2,&statusY);
error(rtn);
if((statusX&0x0800) == 0 )
{
printf("X����λʧЧ\n");
return 100;
}


if((statusX&0x20)!=0)
printf("�ﵽX������λ\n");
if((statusX&0x40)!=0)
printf("�ﵽX�Ḻ��λ\n");

if((statusY&0x0800)==0)
{
printf("Y����λʧЧ\n");
return 100;
}

if((statusY&0x20)!=0)
{
printf("�ﵽY������λ\n");
}
if((statusY&0x40)!=0)
printf("�ﵽY�Ḻ��λ\n");
return rtn;

}
*/
