#ifndef STEPSTRATEGY_H
#define STEPSTRATEGY_H


#include "HumanSimulation.h"
#include <stdlib.h>
#include <stdio.h>
#include <map> 
#include<iostream>
#include<vector>
using namespace std;

class StepStrategy
{
public:
	HumanData humanData;            //Ψһһ�ݾͺ�

									//��������
	double inixPos, inizPos, inixRate, inizRate, markinixPos = 0, markinixRate = 0;
	double xPos, zPos, xRate, zRate;
	bool EnterTR2OneTime = 1;

	//�׹ؽ�����
	double anklexPos, anklexRate, anklezPos, anklezRate;
	double ankleNeedMove;  //�׹ؽ��ƶ�����

						   //�����������ݵ���ģ��ת�Ǻ�ƫ��
	double BodyPitch = 0;			   //����ת���Ƕ�
	double COMAngle = 0;				//����ת��
	Offset COMOffset;					//����ƫ��

										//��Ҫ��¼��
	double xCapture, markxCapture;  //��׽��
	double timeoffset = 0;         //ʱ��ƫ���� ��¼�������ý���
	double curCOP = 0;			//��ǰѹ������
	double STEP_time = 0;


	//��¼���ĺ��׹ؽ��ƶ�����
	std::map<double, Point> COM;       //�����˶�λ�ü�
	std::map<double, Point> Ankle;     //�׹ؽ����ݼ�


	std::map<double, double>BodyPitchs;  //�������ݼ�
	std::map<double, double>HipAngles;  //�������ݼ�
	std::map<double, Offset> Offsets;    //����ƫ�����ݼ�
	std::map<double, COMState> COMStates;    //����״̬���ݼ�
	std::map<double, AnkleState> AnkleStates;

	StepStrategy(HumanData h)
	{
		humanData = h;
		xPos = 0;           //�Ǽ����ĵ�λ��
		zPos = h.l;         //���ĸ߶�
		xRate = 0;          //���ĵ��ٶ�
		zRate = 0;
		inixPos = 0;
		inixRate = 0;

		anklexPos = 0;
		anklezPos = 0;
		anklexRate = 0;
		anklezRate = 0;

		xCapture = 0;
		markxCapture = 0;
		curCOP = 0;            //��ǰѹ������
		ankleNeedMove = 0;

	}
	//��Ҫ���ܺ���
	/////////////////////////////////////////////////////////////////////////////////////////
	bool IsUseful(double xpos, double xrate);       //�ж��Ƿ�ɵ��øò��Իָ�ƽ��
	void startStrategy(double xpos, double xrate);  //��ֵ ���ڿ�ʼ
	void setPosAndRate(double xpos, double xrate);  //������������
	void computexCapture();     //���Կ�ʼ����  ���㲶׽��
	void updateState(double t); //���ݹ�ʽ���㲢��������״̬
	void ModelDataFormat();     //������״̬ת��Ϊģ�͵�ת���ǶȺ�ƫ��
	bool isEnd();               //�ж�ģ���Ƿ�ƽ�⣬�����Ƿ����
	bool isTR2(double t);
	/////////////////////////////////////////////////////////////////////////////////////////

	void startStrategyAuto(double xpos, double xrate);
	void takeStep();
	void ModelData();
	void Restart(HumanData h)
	{
		BodyPitch = 0;			   //����ת���Ƕ�
		COMAngle = 0;				//����ת��
											//��Ҫ��¼��
		xCapture, markxCapture;  //��׽��
		timeoffset = 0;         //ʱ��ƫ���� ��¼�������ý���
		curCOP = 0;			//��ǰѹ������
		STEP_time = 0;
		EnterTR2OneTime = 1;
		humanData = h;
		xPos = 0;           //�Ǽ����ĵ�λ��
		zPos = h.l;         //���ĸ߶�
		xRate = 0;          //���ĵ��ٶ�
		zRate = 0;
		inixPos = 0;
		inixRate = 0;

		anklexPos = 0;
		anklezPos = 0;
		anklexRate = 0;
		anklezRate = 0;

		xCapture = 0;
		markxCapture = 0;
		curCOP = 0;            //��ǰѹ������
		ankleNeedMove = 0;

	}
	void output();                            //������
	void outToModel();                       //������


											 //get �� set ����
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
	std::map<double, Point>getCOM() { return COM; }
	std::map<double, Point>getAnkle() { return Ankle; }
	std::map<double, Offset>getOffsets() { return Offsets; }
	std::map<double, double>getHipAngles() { return HipAngles; }
	std::map<double, double>getBodyPitchs() { return BodyPitchs; }
	std::map<double, COMState>getCOMStates() { return COMStates; }
	std::map<double, AnkleState>getAnkleStates() { return AnkleStates; }
};
#endif