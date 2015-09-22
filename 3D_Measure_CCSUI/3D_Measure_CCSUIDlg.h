// 3D_Measure_CCSUIDlg.h : ͷ�ļ�
//

#pragma once

#include "ges.h"

// CMy3D_Measure_CCSUIDlg �Ի���
class CMy3D_Measure_CCSUIDlg : public CDialog
{
// ����
public:
	CMy3D_Measure_CCSUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY3D_MEASURE_CCSUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

//˽������
private:
	int width;                    //Xɨ�貽��
	int height;                           //Yɨ�貽��
	int IntensityStart;                    //LED��ǿ��ʼֵ
	int IntensityStep;                      //LED��ǿ����
	int IntensityCount;//(100-IntensityStart)/IntensityStep+1;                     //LED���ǿ����
	int FrequencyStart;                   //Ƶ����ʼֵ
	int FrequencyStep;                      //Ƶ������
	int FrequencyCount;//(2000-FrequencyStart)/FrequencyStep+1;                     //��Ƶ�ʴ���
	int DpiStart;                              //�ֱ���
	int DpiStep;							//�ֱ�������
	int DpiCount;							//�ֱ������Ӵ���
	char filename[25];	            //���ϱ��+���� 
	int average;                               //averaging����
	int holdlast;                            //holdlast����
};
