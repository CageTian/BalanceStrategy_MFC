
#ifndef BALANCESTRATEGYDLG_H
#define BALANCESTRATEGYDLG_H

// BalanceStrategyDlg.h : ͷ�ļ�

#pragma once
#include "BalanceControl.h"
#include "afxwin.h"
#include "tchart1.h"
#include "CSeries.h"
using namespace std;

struct ControllValue {
	bool isStatic = 0;           //�Ƿ�Ϊ��ֹ�˶�״̬
	double initialRate=0;           //���ų��ٶȴ�С
	Strategy stategyToTake = Strategy::AUTO; //��Ҫ��ȡ�Ĳ���
	bool showCOMTrategy=0;       //�Ƿ���ʾ���Ĺ켣
	bool showSkin=0;             //�Ƿ���ʾ��ƤЧ��
	bool isRun = 0;              //�Ƿ��ѵ�����а�ťor��ͣ��ť  ע�⣺�����ͣ��ťʱ�ñ���Ϊ0
	bool isRestet = 0;           //�Ƿ��ѵ�����ð�ť   ��ƽ��������޹�
	bool isEmpty = 0;            //�Ƿ��ѵ����հ�ť   ��ƽ��������޹�
	double xRate = 0;
	double xPos = 0;
	double time = 0;
	
};
// CBalanceStrategyDlg �Ի���
class CBalanceStrategyDlg : public CDialogEx
{ 
// ����
public:

	BalanceControl controller;
	ControllValue controlValue;
	CBalanceStrategyDlg(CWnd* pParent = NULL);	// ��׼���캯��

	BOOL SetWindowPixelFormat(HDC hDC);  //�趨���ظ�ʽ
	BOOL CreateViewGLContext(HDC hDC);   //view GL Context
	void RenderScene();  //���Ƴ���
	////////////////////////////////////////////////////
	void SpecialKeys(int key, int x, int y);
	void drawSrcs();
	void drawXYZ();
	////////////////////////////////////////////////////

	HDC hrenderDC;  //�豸������
	HGLRC hrenderRC;  //��Ⱦ������
	float m_yRotate;  //ת��
	int PixelFormat;    //���ظ�ʽ
	CSeries lineSeries;
	CSeries lineSeries2;



// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BALANCESTRATEGY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
//	BOOL m_zy;
	CButton m_run;
	double m_speed;
	afx_msg void OnKillfocusEdit1();
	double m_height;
	double m_legLen;
	double m_lf;
	double m_rf;
	double m_stepTime;
	int m_maxStep;
	double m_Qmax;
	CTchart1 m_chart1;
	CTchart1 m_chart2;
	afx_msg void OnBnClickedCancel();
	double m_sampleTime;
};



#endif // !BALANCESTRATEGYDLG_H
