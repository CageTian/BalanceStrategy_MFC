

#ifndef BALANCECONTROL_H
#define BALANCECONTROL_H

#pragma once

#include "afxdialogex.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl/glut.h>
#include<vector>
#include "timer.h"
#include "basicterrain.h"
#include "skeleton.h"
#include "fkframer.h"
#include "ikframer.h"
#include "humanskeleton.h"
#include "humanwalkanimation.h"
#include "humanwalkframer.h"
#include "animation.h"
#include "humanskin.h"
#include"names.h"
#include"actionFramer.h"
#include"actionAnimation.h"
#include "StepStrategy.h"
#include "HipStrategy.h"
#include "AnkleStrategy.h"
#include "MultStepStrategy.h"
#include<iostream>
#include<GL\gl.h>
#include<GL\GLU.h>

#define DEG_TO_RAD 0.0174532925

// Distance of camera from actor
#define CAMERA_DISTANCE 5.0

// Number of stairs to walk up
#define NUM_STAIRS 4

// Initial y translation for the human skeleton, to place bottom of feet at y = 0.0
#define INITIAL_ELEVATION 1.98

// Initial z translation for the human skeleton
#define INITIAL_POSITION -2.6

// Actor velocity - rate of motion along z axis as units per second; can be adjusted with rate variable from keyboard (r/R keys)
#define ACTOR_VELOCITY 0.40

// Translation increment when translating x, y, z from keyboard input
#define TRANSLATION_INCREMENT 0.25

// Translation increment when translating x, y, z from keyboard input
#define ROTATION_INCREMENT 0.5

enum Strategy { AUTO, ANKLE, HIP, STEP, MSTEP };
class BalanceControl
{
public:
	Timer timer;
    HumanData humanData;                      //存储人体数据
	HumanSkeleton hskel;
	HumanSkin hskin;
	StepStrategy  stepStrategy;   //跨步策略
	HipStrategy hipStrategy;     //髋关节策略
	AnkleStrategy ankleStrategy; //踝关节策略
	MultStepStrategy MstepStrategy;
	BasicTerrain ground;
	HumanWalkAnimation humanAnimator;
	actionAnimation *tempanimation = NULL;
	actionAnimation * yacobianimation = NULL;
	actionFramer *jacobiframer;
	actionFramer *forTest;
	HumanWalkFramer humanFramer;
	std::vector<Point>markAnkleProfile;
	std::vector<Point>markCOMProfile;
	std::map<double, Point> COM;
	std::map<double, Point> Ankle;
	std::map<double, double> COMData;
	Vector3 bodypos;
	double tempXPos;
	double tempXRate;
	double flag = -humanData.sampleTime, flag1 = -humanData.sampleTime;
	double curStrategy = -1;
	double Auto_Strategy = -1;
	bool isFinished = false;
	double currentTime = 0;
	double currentxPos = 0;
	double currentxRate = 0;


	BalanceControl() :hskel(humanData), hskin(humanData), stepStrategy(humanData), hipStrategy(humanData), ankleStrategy(humanData), MstepStrategy(humanData),
		humanAnimator(&ground) {}
	
	void updateAnkleModel();
	void updateHipModel();
	void updateStepModel();
	void updateMultStepModel();
	void takeAnkle(std::map<std::string, double> angles_, Vector3 COM, double xpos, double xrate);
	void takeHip(double xpos, double xrate);
	void takeStep(double xpos, double xrate);
	void takeMultSteps(double xpos, double xrate);
	void takeAnkleAuto(double xpos, double xrate, bool iship);
	void takeStepAuto(double xpos, double xrate);
	void takeMultStepsAuto(double xpos, double xrate);
	void RecoverBalance(double xpos, double xrate, Strategy strategyToTake);
	void getxPosAndxRate(double &xPos,double &xRate, double &time);
	void Reset();
	void initData();
  	void initControl();


};

#endif // !BALANCECONTROL_H