
#ifndef BALANCESTRATEGYDLG_H
#define BALANCESTRATEGYDLG_H

// BalanceStrategyDlg.h : 头文件

#pragma once
#include "BalanceControl.h"
#include "afxwin.h"
#include "tchart1.h"
#include "CSeries.h"
using namespace std;

struct ControllValue {
	bool isStatic = 0;           //是否为静止运动状态
	double initialRate=0;           //受扰初速度大小
	Strategy stategyToTake = Strategy::AUTO; //需要采取的策略
	bool showCOMTrategy=0;       //是否显示质心轨迹
	bool showSkin=0;             //是否显示蒙皮效果
	bool isRun = 0;              //是否已点击运行按钮or暂停按钮  注意：点击暂停按钮时该变量为0
	bool isRestet = 0;           //是否已点击重置按钮   与平衡控制器无关
	bool isEmpty = 0;            //是否已点击清空按钮   与平衡控制器无关
	double xRate = 0;
	double xPos = 0;
	double time = 0;
	
};
// CBalanceStrategyDlg 对话框
class CBalanceStrategyDlg : public CDialogEx
{ 
// 构造
public:

	BalanceControl controller;
	ControllValue controlValue;
	CBalanceStrategyDlg(CWnd* pParent = NULL);	// 标准构造函数

	BOOL SetWindowPixelFormat(HDC hDC);  //设定像素格式
	BOOL CreateViewGLContext(HDC hDC);   //view GL Context
	void RenderScene();  //绘制场景
	////////////////////////////////////////////////////
	void SpecialKeys(int key, int x, int y);
	void drawSrcs();
	void drawXYZ();
	////////////////////////////////////////////////////

	HDC hrenderDC;  //设备上下文
	HGLRC hrenderRC;  //渲染上下文
	float m_yRotate;  //转速
	int PixelFormat;    //像素格式
	CSeries lineSeries;
	CSeries lineSeries2;



// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BALANCESTRATEGY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
