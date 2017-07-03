#ifndef HipSTRATEGY_H
#define HipSTRATEGY_H


#include "HumanSimulation.h"
#include <stdlib.h>
#include <stdio.h>
#include <map> 
#include<iostream>
#include<vector>
using namespace std;

class HipStrategy
{
public:
	HumanData humanData;    //唯一一份就好


							//质心数据
	double inixPos, inizPos, inixRate, inizRate, markinixPos = 0, markinixRate = 0;
	double xPos, zPos, xRate, zRate;

	//踝关节数据
	double anklexPos, anklexRate, anklezPos, anklezRate;


	//根据质心数据调节模型转角和偏移
	double BodyPitch = 0;			   //上身转动角度
	double COMAngle = 0;				//质心转角
	Offset COMOffset;					//质心偏移

										//重要记录量
	double xCapture, markxCapture;  //捕捉点
	double timeoffset = 0;         //时间偏移量 记录飞轮作用结束
	double curCOP;			//当前压力中心
	double markEndFlag = 0;  //用于标记髋关节调节是否已经结束


							 //记录质心和踝关节移动曲线
	std::map<double, Point> COM;       //质心运动位置集
	std::map<double, Point> Ankle;     //踝关节数据集
	std::map<double, double> COMData;  //质心数据集
	std::map<double, Offset> offset;    //质心偏移数据集


	HipStrategy(HumanData h)
	{
		humanData = h;
		xCapture = 0;
		inixRate = 0;
		inixPos = 0;
		xPos = 0;           //骨架质心的位置
		zPos = h.l;         //质心高度
		xRate = 0;          //质心的速度
		zRate = 0;
		anklexPos = 0;
		anklezPos = 0;
		anklexRate = 0;
		anklezRate = 0;
		curCOP = 0;            //当前压力中心
		markxCapture = 0;
	}

	//主要功能函数
	/////////////////////////////////////////////////////////////////////////////////////////
	bool IsUseful(double xpos, double xrate);       //判断是否可调用该策略恢复平衡
	void startStrategy(double xpos, double xrate);  //赋值 调节开始
	void setPosAndRate(double xpos, double xrate);  //设置质心数据
	void updateState(double t);						//根据公式计算并更新质心状态
	void ModelDataFormat();							//将质心状态转变为模型的转动角度和偏移
	bool isEnd(double time);						//判断模型是否平衡，调节是否结束
													/////////////////////////////////////////////////////////////////////////////////////////
	void Restart(HumanData h)
	{
		humanData = h;
		xCapture = 0;
		inixRate = 0;
		inixPos = 0;
		xPos = 0;           //骨架质心的位置
		zPos = h.l;         //质心高度
		xRate = 0;          //质心的速度
		zRate = 0;
		anklexPos = 0;
		anklezPos = 0;
		anklexRate = 0;
		anklezRate = 0;
		curCOP = 0;            //当前压力中心
		markxCapture = 0;

		BodyPitch = 0;			   //上身转动角度
		COMAngle = 0;				//质心转角

		timeoffset = 0;         //时间偏移量 记录飞轮作用结束
		markEndFlag = 0;  //用于标记髋关节调节是否已经结束
	}
	void output();							//尚无用
	void outToModel();						//尚无用

											//get 和 set 函数
	double getxPos() { return xPos; }
	double getxRate() { return xRate; }
	double getzPos() { return zPos; }
	double getzRate() { return zRate; }
	double getanklexPos() { return  anklexPos; }
	double getanklexRate() { return anklexRate; }
	double getanklezPos() { return  anklezPos; }
	double getanklezRate() { return anklezRate; }
	double getcurCOP() { return curCOP; }
	double getCOMAngle() { return COMAngle; }
	Offset getCOMOffset() { return COMOffset; }
	double getBodyPitch() { return BodyPitch; }
	std::map<double, Offset>getOffset() { return offset; }
	std::map<double, double>getCOMData()
	{
		std::map<double, double >::iterator iter1;
		std::map<double, double>::iterator iter2;
		for (iter1 = COMData.begin(); iter1 != COMData.end(); iter1++)
		{
			std::cout << "angle   " << iter1->second << "   " << endl;
		}
		return COMData;
	}
	std::map<double, Point>getCOM() { return COM; }
	std::map<double, Point>getAnkle() { return Ankle; }
};
#endif

