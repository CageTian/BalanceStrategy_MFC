#ifndef AnkleSTRATEGY_H
#define AnkleSTRATEGY_H


#include "HumanSimulation.h"
#include <stdlib.h>
#include <stdio.h>
#include <map> 
#include<iostream>
#include<vector>
using namespace std;

class AnkleStrategy
{
public:
	HumanData humanData;    //唯一一份就好

							//质心数据
	double inixPos, inizPos, inixRate, inizRate;
	double xPos, zPos, xRate, zRate;

	//踝关节数据
	double anklexPos, anklexRate, anklezPos, anklezRate;

	//根据质心数据调节模型转角和偏移
	double BodyPitch = 0;			   //上身转动角度
	double COMAngle = 0;				//质心转角
	Offset COMOffset;					//质心偏移
	double ANKLE_time = -humanData.sampleTime;

	double curCOP;						 //压力中心位置
	double isHip = 0;                   //是否是髋关节结束调用的，根据这个来给bodyPitch赋值
										//记录质心和踝关节移动曲线
	std::map<double, Point> COM;       //质心运动位置集
	std::map<double, Point> Ankle;     //踝关节数据集

	std::map<double, double>HipAngles;  //质心数据集
	std::map<double, Offset> Offsets;    //质心偏移数据集
	std::map<double, COMState> COMStates;    //质心状态数据集


	AnkleStrategy(HumanData h)
	{
		humanData = h;
		xPos = 0;           //骨架质心的位置
		zPos = h.l;         //质心高度
		xRate = 0;          //质心的速度
		zRate = 0;
		inixPos = 0;
		inixRate = 0;
		anklexPos = 0;
		anklezPos = 0;
		anklexRate = 0;
		anklezRate = 0;
		curCOP = 0;            //当前压力中心
	}

	//主要功能函数
	/////////////////////////////////////////////////////////////////////////////////////////
	bool IsUseful(double xpos, double xrate);//判断是否可调用该策略恢复平衡
	void startStrategy(double xpos, double xrate);    //赋值 调节开始
	void startStrategyAuto(double xpos, double xrate, bool iship);
	void setPosAndRate(double xpos, double xrate);//设置质心数据
	void updateState(double t);//根据公式计算并更新质心状态
	void ModelDataFormat(double angle);//将质心状态转变为模型的转动角度和偏移
	bool isEnd();									 //判断模型是否平衡，调节是否结束
													 /////////////////////////////////////////////////////////////////////////////////////////
	void takeAnkle();
	void output();					//尚无用
	void ModelData();				//尚无用

	void ReStart(HumanData h)
	{
		humanData = h;
		xPos = 0;           //骨架质心的位置
		zPos = h.l;         //质心高度
		xRate = 0;          //质心的速度
		zRate = 0;
		inixPos = 0;
		inixRate = 0;
		anklexPos = 0;
		anklezPos = 0;
		anklexRate = 0;
		anklezRate = 0;
		curCOP = 0;            //当前压力中心

		isHip = 0;
		BodyPitch = 0;
		COMAngle = 0;
	}

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

	std::map<double, Offset>getOffset() { return Offsets; }
	std::map<double, double>getHipAngles() { return HipAngles; }
	std::map<double, COMState>getStates() { return COMStates; }

	std::map<double, Point>getCOM() { return COM; }
	std::map<double, Point>getAnkle() { return Ankle; }


};
#endif
