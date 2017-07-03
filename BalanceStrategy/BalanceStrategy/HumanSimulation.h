#ifndef HUMANSIMULATION_H
#define HUMANSIMULATION_H


#include<math.h> 
#include<iostream>
using namespace std;
#define pi 3.1415926

//人体建模数据
class HumanData
{
public:

	//用于小人身体建模
	double m = 66.64;                   // 人体质量
	double mf = 1.01;                   // 脚部质量
	double height = 1.8805;             //人体高度
	double l = 1.2805;				    //质心到地面的距离
	double legLen = 1.1805;             //腿的长度
	double lowerleg = 0.5;			    //小腿长度
	double upperleg = legLen - lowerleg;//大腿长度
	double head = 0.1;				    //头部长度
	double neck = 0.08;			        //脖子长度
	double torse = height - legLen;     //躯干长度
	double upperarm = 0.5;				//上臂长度
	double lowerarm = 0.6;				//下臂长度
	double shoulderwidth = 0.5;			//肩宽
	double hipwidth = 0.3;				//髋宽
	double foot = 0.27;					//脚长
	double lf = 0.09;                   //踝关节到脚尖距离
	double rf = 0.18;                   //踝关节到脚跟距离
	double hf = 0.05;                   //脚高度

										//重要计算常量
	double g = 9.8;					   //重力加速度
	double J = 5;					   // 转动惯量
	double w = sqrt(g / l);			   //角速度
	double rateLimit = 0.01;          //用于判定模型是否静止

										//用于髋关节策略设计  
	double Tmax = 200;			          //最大飞轮转矩      
	double Qmax = pi / 3.0;		          //最大飞轮转角
	double TR2 = sqrt(4 * J*Qmax / Tmax); //飞轮作用结束时间
	double TR1 = TR2 / 2;                 //飞轮作用中间时刻

										  //用于跨步策略设计
	double sampleTime = 0.05;       //系统采样时间
	double maxSteps = 10;			//多步策略的最大跨步数
	double stepTime = TR2 + 0.1;	//跨步时间 即摆腿时间
	double hstep = 0.05;            //跨步高度
	double rstep = 0.6;             //跨步长度
	double QmaxforStep = pi / 3.0;						  //跨步最大飞轮转角
	double TR2forStep = sqrt(4 * J*QmaxforStep / Tmax);  //跨步飞轮作用结束时间
	double TR1forStep = TR2forStep / 2;					 //跨步飞轮作用中间时刻

	//void setsampleTime(double t) { sampleTime = t; }
	//void setsampleTime(double t) { sampleTime = t; }
	//void setQmax(double t) { Qmax = t; }
	//void stepTime(double t) { stepTime = t; }
};


struct Offset
{
	double px = 0;
	double pz = 0;

};
struct COMState
{
	double xpos = 0;
	double xrate = 0;
	double zpos = 0;

	COMState(double pos = 0, double rate = 0, double z = 0) { xpos = pos; xrate = rate; zpos = z; }

};
struct AnkleState
{
	double anklexpos;
	double anklezpos;
	AnkleState(double x, double z) { anklexpos = x; anklezpos = z; }
};
struct Point
{
	double px = 0;
	double py = 0;
	double pz = 0;
	Point(double x = 0, double y = 0, double z = 0) { px = x; py = y; pz = z; }
	void setPoint(double x, double y, double z) { px = x; py = y; pz = z; }

};

#endif // !HUMANSIMULATION_H