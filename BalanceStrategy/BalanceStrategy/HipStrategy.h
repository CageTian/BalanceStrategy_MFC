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
	HumanData humanData;    //Ψһһ�ݾͺ�


							//��������
	double inixPos, inizPos, inixRate, inizRate, markinixPos = 0, markinixRate = 0;
	double xPos, zPos, xRate, zRate;

	//�׹ؽ�����
	double anklexPos, anklexRate, anklezPos, anklezRate;


	//�����������ݵ���ģ��ת�Ǻ�ƫ��
	double BodyPitch = 0;			   //����ת���Ƕ�
	double COMAngle = 0;				//����ת��
	Offset COMOffset;					//����ƫ��

										//��Ҫ��¼��
	double xCapture, markxCapture;  //��׽��
	double timeoffset = 0;         //ʱ��ƫ���� ��¼�������ý���
	double curCOP;			//��ǰѹ������
	double markEndFlag = 0;  //���ڱ���Źؽڵ����Ƿ��Ѿ�����


							 //��¼���ĺ��׹ؽ��ƶ�����
	std::map<double, Point> COM;       //�����˶�λ�ü�
	std::map<double, Point> Ankle;     //�׹ؽ����ݼ�
	std::map<double, double> COMData;  //�������ݼ�
	std::map<double, Offset> offset;    //����ƫ�����ݼ�


	HipStrategy(HumanData h)
	{
		humanData = h;
		xCapture = 0;
		inixRate = 0;
		inixPos = 0;
		xPos = 0;           //�Ǽ����ĵ�λ��
		zPos = h.l;         //���ĸ߶�
		xRate = 0;          //���ĵ��ٶ�
		zRate = 0;
		anklexPos = 0;
		anklezPos = 0;
		anklexRate = 0;
		anklezRate = 0;
		curCOP = 0;            //��ǰѹ������
		markxCapture = 0;
	}

	//��Ҫ���ܺ���
	/////////////////////////////////////////////////////////////////////////////////////////
	bool IsUseful(double xpos, double xrate);       //�ж��Ƿ�ɵ��øò��Իָ�ƽ��
	void startStrategy(double xpos, double xrate);  //��ֵ ���ڿ�ʼ
	void setPosAndRate(double xpos, double xrate);  //������������
	void updateState(double t);						//���ݹ�ʽ���㲢��������״̬
	void ModelDataFormat();							//������״̬ת��Ϊģ�͵�ת���ǶȺ�ƫ��
	bool isEnd(double time);						//�ж�ģ���Ƿ�ƽ�⣬�����Ƿ����
													/////////////////////////////////////////////////////////////////////////////////////////
	void Restart(HumanData h)
	{
		humanData = h;
		xCapture = 0;
		inixRate = 0;
		inixPos = 0;
		xPos = 0;           //�Ǽ����ĵ�λ��
		zPos = h.l;         //���ĸ߶�
		xRate = 0;          //���ĵ��ٶ�
		zRate = 0;
		anklexPos = 0;
		anklezPos = 0;
		anklexRate = 0;
		anklezRate = 0;
		curCOP = 0;            //��ǰѹ������
		markxCapture = 0;

		BodyPitch = 0;			   //����ת���Ƕ�
		COMAngle = 0;				//����ת��

		timeoffset = 0;         //ʱ��ƫ���� ��¼�������ý���
		markEndFlag = 0;  //���ڱ���Źؽڵ����Ƿ��Ѿ�����
	}
	void output();							//������
	void outToModel();						//������

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

