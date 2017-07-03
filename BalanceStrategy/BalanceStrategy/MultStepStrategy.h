#ifndef MULTSTEPSTRATEGY_H
#define  MULTSTEPSTRATEGY_H


#include "HumanSimulation.h"
#include "vector3.h"
#include <stdlib.h>
#include <stdio.h>
#include <map> 
#include<iostream>

using namespace std;
struct NStepData
{
	string leg[2] = { "L","R" };         //用于判断当前摆动腿是哪只腿(左腿 or 右腿)
	int  legToTake = 0;					 //此处设为0 默认先出右腿
	double xPos = 0;					 //当前跨步的位置
	double xRate = 0;					 //当前跨步的速度
	double curStep = 0;					 //当前跨步数
	double totalStep = 0;				 //本策略的总步数
	double xCapture = 0;		    	 //当前跨步的捕捉点
	double curCOP = 0;					 //当前跨步的压力中心
	Vector3 offset;					     //当前跨步质心偏移：确保支撑腿始终位于地面不动 

										 //规划踝关节轨迹
	double ankleNeedMove = 0;			 //当前跨步中踝关节需要移动的距离
	double lastAnkleNeedMove = 0;		 //上次跨步中踝关节需要移动的距离
	double swingLeginitialPos = 0;		 //用于规划踝关节轨迹，摆动腿踝关节的初始位置，即摆腿作用即将开始时刻摆动腿踝关节的初始位置

	bool isCurStep = 0;					 //判断是否是当前一步  用于两步之间如何正确衔接
	bool isStanceStraight = 1;			 //支撑腿是否处于直立状态 用于两步之间如何正确衔接
	bool isTR2 = 0;					     //是否已经到TR2时刻，即飞轮作用结束时刻
	bool isStepTime = 0;			     //是否已经到跨步结束时刻

};
class MultStepStrategy
{
public:
	HumanData humanData;    //唯一一份就好
	NStepData NStep; //当前跨步的相关数据

					 //质心数据
	double inixPos, inixRate, xPos, zPos, xRate, zRate;
	//踝关节数据
	double anklexPos, anklexRate, anklezPos, anklezRate;
	//根据质心数据调节模型转角和偏移
	double BodyPitch = 0;
	double HipAngle = 0;
	double KneeAngle = 0;
	double MSTEP_time = -humanData.sampleTime;
	// 用于两步之间如何正确衔接
	double initialCOMAngle = 0;
	double initialkneePos = 0;
	Offset COMOffset;
	//重要关键字
	double EnterTR2OneTime = 1;          //第一步过程中，只判读“当前是否为TR2时刻”一次
	double enterStanceOneTime = 0;       //每个跨步过程中，只判断“支撑腿是否为直立状态”一次
	double EnterStepTimeOneTime = 1;      //每个跨步过程中，只判断“当前是否为stepTime时刻”一次



	MultStepStrategy(HumanData h)
	{
		humanData = h;
		xPos = 0;           //骨架质心的位置
		zPos = h.l;         //质心高度
		xRate = 0;          //质心的速度
		zRate = 0;
		anklexPos = 0;
		anklezPos = 0;
		anklexRate = 0;
		anklezRate = 0;
	}
	//主要功能函数
	/////////////////////////////////////////////////////////////////////////////////////////
	int IsUseful(double xpos, double xrate);			//判断是否可调用该策略恢复平衡返回需要跨几步
	void startStrategy(double xpos, double xrate);		//赋值 调节开始
	void setPosAndRate(double xpos, double xrate);		//设置质心数据
	void computexCapture();								//策略开始调用  计算捕捉点
	void updateStateAnkle(double t);					//更新踝关节状态
	void updateState(double t);							//更新质心状态，为方便计算，分为三种类别：第一步，中间步，最后一步
	void updateFirstCOMState(double t);					//更新第一步
	void updateMidCOMState(double t);					//更新中间步
	void updateLastCOMState(double t);					//更新最后一步
	void ModelDataFormat(double t);						//将质心状态转变为模型的转动角度和偏移
	void endOfEachStep(Vector3 offset);					//在stepTime时刻切换步伐，此函数用于为新的一步赋NStep状态值
	bool isEnd();										//判断模型是否平衡，调节是否结束
	bool isTR2(double t);								//判断是否到达TR2时刻，即飞轮作用结束时刻
	bool isStepTime(double t);							//判断是否是否到达stepTime时刻，指示当前跨步是否已结束
														/////////////////////////////////////////////////////////////////////////////////////////

	void Restart(HumanData h)
	{
		BodyPitch = 0;
		HipAngle = 0;
		KneeAngle = 0;
		MSTEP_time = -humanData.sampleTime;
		// 用于两步之间如何正确衔接
		initialCOMAngle = 0;
		initialkneePos = 0;
		COMOffset;
		//重要关键字
		EnterTR2OneTime = 1;          //第一步过程中，只判读“当前是否为TR2时刻”一次
		enterStanceOneTime = 0;       //每个跨步过程中，只判断“支撑腿是否为直立状态”一次
		EnterStepTimeOneTime = 1;      //每个跨步过程中，只判断“当前是否为stepTime时刻”一次

		NStep.legToTake = 0;					 //此处设为0 默认先出右腿
		NStep.xPos = 0;					 //当前跨步的位置
		NStep.xRate = 0;					 //当前跨步的速度
		NStep.curStep = 0;					 //当前跨步数
		NStep.totalStep = 0;				 //本策略的总步数
		NStep.xCapture = 0;		    	 //当前跨步的捕捉点
		NStep.curCOP = 0;					 //当前跨步的压力中心

											 //规划踝关节轨迹
		NStep.ankleNeedMove = 0;			 //当前跨步中踝关节需要移动的距离
		NStep.lastAnkleNeedMove = 0;		 //上次跨步中踝关节需要移动的距离
		NStep.swingLeginitialPos = 0;		 //用于规划踝关节轨迹，摆动腿踝关节的初始位置，即摆腿作用即将开始时刻摆动腿踝关节的初始位置

		NStep.isCurStep = 0;					 //判断是否是当前一步  用于两步之间如何正确衔接
		NStep.isStanceStraight = 1;			 //支撑腿是否处于直立状态 用于两步之间如何正确衔接
		NStep.isTR2 = 0;					     //是否已经到TR2时刻，即飞轮作用结束时刻
		NStep.isStepTime = 0;			     //是否已经到跨步结束时刻


		humanData = h;
		xPos = 0;           //骨架质心的位置
		zPos = h.l;         //质心高度
		xRate = 0;          //质心的速度
		zRate = 0;
		anklexPos = 0;
		anklezPos = 0;
		anklexRate = 0;
		anklezRate = 0;
	}
	void takeMultSteps();
	void ModelData();				//尚无用

									//get 和 set函数
	double getxPos() { return xPos; }
	double getxRate() { return xRate; }
	double getzPos() { return zPos; }
	double getzRate() { return zRate; }
	double getanklexPos() { return  anklexPos; }
	double getanklexRate() { return anklexRate; }
	double getanklezPos() { return  anklezPos; }
	double getanklezRate() { return anklezRate; }
	double getHipAngle() { return HipAngle; }
	double getBodyPitch() { return BodyPitch; }

};
#endif