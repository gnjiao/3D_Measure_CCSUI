#pragma once

void error(short rtn);//��������Ӻ���   ����ֵ����ĺ���ο��ĵ�

short AxisInitial(short axis_num,unsigned short limit);//�������ʼ������

short MotionInitial();//���˶�������ʼ��

short GT_System_Initinal();  //ƽ̨��ʼ��

short GT_Buffer();//�������˶�ָ���ź���

short GT_GetPosition();//��ȡ�ᵱǰλ��

short GT_Stop();//�رտ������ŷ�ʹ��

short GT_Check_Limit();//��λ��־���

short GT_ADD_Buffer_continue(double AxisX_1, double AxisY_1,double Vel);

short GT_ADD_Buffer_trigger(double AxisX_1, double AxisY_1,double Vel);

short GT_Check_status()	;//����˶�״̬�Ƿ�ֹͣ

short GT_GetLmtZero_X();//X�˶�������λ�ò�����Ϊϵͳ�����ԭ��

short GT_GetLmtZero_Y();//Y�˶�������λ�ò�����Ϊϵͳ�����ԭ��

//********************************************************************************************************

short GT_GetZero();//���㣬�ص�����տ�ʼ����㣨�ϵ�����Ϊϵͳԭ�㣩

short Home(unsigned short axis,long pos,long offset,double vel_high,double vel_low) ;//home�źŻ���

short GT_ADD_Buffer_continue(double AxisX_1, double AxisY_1,double AxisX_2, double AxisY_2);//��ӻ�����ָ��

short GT_GetStartZero();//�ص��ϵ�ʱ������(��������Ϊϵͳԭ��)

short GT_Stop_AxisNum(short Set_AxisNum);//ֹͣ������ָ���˶�����Set_AxisNum-1��

short GT_Read_AxisNum();//��ȡ��������ǰָ��ִ�е������ı��