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
	HumanData humanData;    //Ψһһ�ݾͺ�

							//��������
	double inixPos, inizPos, inixRate, inizRate;
	double xPos, zPos, xRate, zRate;

	//�׹ؽ�����
	double anklexPos, anklexRate, anklezPos, anklezRate;

	//�����������ݵ���ģ��ת�Ǻ�ƫ��
	double BodyPitch = 0;			   //����ת���Ƕ�
	double COMAngle = 0;				//����ת��
	Offset COMOffset;					//����ƫ��
	double ANKLE_time = -humanData.sampleTime;

	double curCOP;						 //ѹ������λ��
	double isHip = 0;                   //�Ƿ����Źؽڽ������õģ������������bodyPitch��ֵ
										//��¼���ĺ��׹ؽ��ƶ�����
	std::map<double, Point> COM;       //�����˶�λ�ü�
	std::map<double, Point> Ankle;     //�׹ؽ����ݼ�

	std::map<double, double>HipAngles;  //�������ݼ�
	std::map<double, Offset> Offsets;    //����ƫ�����ݼ�
	std::map<double, COMState> COMStates;    //����״̬���ݼ�


	AnkleStrategy(HumanData h)
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
		curCOP = 0;            //��ǰѹ������
	}

	//��Ҫ���ܺ���
	/////////////////////////////////////////////////////////////////////////////////////////
	bool IsUseful(double xpos, double xrate);//�ж��Ƿ�ɵ��øò��Իָ�ƽ��
	void startStrategy(double xpos, double xrate);    //��ֵ ���ڿ�ʼ
	void startStrategyAuto(double xpos, double xrate, bool iship);
	void setPosAndRate(double xpos, double xrate);//������������
	void updateState(double t);//���ݹ�ʽ���㲢��������״̬
	void ModelDataFormat(double angle);//������״̬ת��Ϊģ�͵�ת���ǶȺ�ƫ��
	bool isEnd();									 //�ж�ģ���Ƿ�ƽ�⣬�����Ƿ����
													 /////////////////////////////////////////////////////////////////////////////////////////
	void takeAnkle();
	void output();					//������
	void ModelData();				//������

	void ReStart(HumanData h)
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
		curCOP = 0;            //��ǰѹ������

		isHip = 0;
		BodyPitch = 0;
		COMAngle = 0;
	}

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

	std::map<double, Offset>getOffset() { return Offsets; }
	std::map<double, double>getHipAngles() { return HipAngles; }
	std::map<double, COMState>getStates() { return COMStates; }

	std::map<double, Point>getCOM() { return COM; }
	std::map<double, Point>getAnkle() { return Ankle; }


};
#endif
