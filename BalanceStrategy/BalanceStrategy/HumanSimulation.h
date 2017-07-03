#ifndef HUMANSIMULATION_H
#define HUMANSIMULATION_H


#include<math.h> 
#include<iostream>
using namespace std;
#define pi 3.1415926

//���彨ģ����
class HumanData
{
public:

	//����С�����彨ģ
	double m = 66.64;                   // ��������
	double mf = 1.01;                   // �Ų�����
	double height = 1.8805;             //����߶�
	double l = 1.2805;				    //���ĵ�����ľ���
	double legLen = 1.1805;             //�ȵĳ���
	double lowerleg = 0.5;			    //С�ȳ���
	double upperleg = legLen - lowerleg;//���ȳ���
	double head = 0.1;				    //ͷ������
	double neck = 0.08;			        //���ӳ���
	double torse = height - legLen;     //���ɳ���
	double upperarm = 0.5;				//�ϱ۳���
	double lowerarm = 0.6;				//�±۳���
	double shoulderwidth = 0.5;			//���
	double hipwidth = 0.3;				//�ſ�
	double foot = 0.27;					//�ų�
	double lf = 0.09;                   //�׹ؽڵ��ż����
	double rf = 0.18;                   //�׹ؽڵ��Ÿ�����
	double hf = 0.05;                   //�Ÿ߶�

										//��Ҫ���㳣��
	double g = 9.8;					   //�������ٶ�
	double J = 5;					   // ת������
	double w = sqrt(g / l);			   //���ٶ�
	double rateLimit = 0.01;          //�����ж�ģ���Ƿ�ֹ

										//�����Źؽڲ������  
	double Tmax = 200;			          //������ת��      
	double Qmax = pi / 3.0;		          //������ת��
	double TR2 = sqrt(4 * J*Qmax / Tmax); //�������ý���ʱ��
	double TR1 = TR2 / 2;                 //���������м�ʱ��

										  //���ڿ粽�������
	double sampleTime = 0.05;       //ϵͳ����ʱ��
	double maxSteps = 10;			//�ಽ���Ե����粽��
	double stepTime = TR2 + 0.1;	//�粽ʱ�� ������ʱ��
	double hstep = 0.05;            //�粽�߶�
	double rstep = 0.6;             //�粽����
	double QmaxforStep = pi / 3.0;						  //�粽������ת��
	double TR2forStep = sqrt(4 * J*QmaxforStep / Tmax);  //�粽�������ý���ʱ��
	double TR1forStep = TR2forStep / 2;					 //�粽���������м�ʱ��

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