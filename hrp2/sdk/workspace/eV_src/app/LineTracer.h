/******************************************************************************
 *  LineTracer.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include "LineMonitor.h"
#include "BalancingWalker.h"
#include "PidController.h"

#include "Odmetry.h"
#include "Rotator.h"
#include "RunManager.h"
#include "app_config.h"

#define TURN_GUARD 30

class LineTracer {
public:
    LineTracer(LineMonitor* lineMonitor,
               BalancingWalker* balancingWalker,
               PidController* pidController);
    virtual ~LineTracer();

    void run();
    void runWithoutBalancing();
    bool isFinished();
	int calcDirection();

    float debug;
private:
    LineMonitor* mLineMonitor;
    BalancingWalker* mBalancingWalker;
    PidController* mPidController;
    Rotator* mRotator = new Rotator();
    bool mIsInitialized;
    bool mIsFinished;
	float percent;

	ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
    ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel();
    Odmetry *mOdmetry = Odmetry::getInstance();
    RunManager *mRunManager = RunManager::getInstance();
    RunManager::Section mSection;

    void setPID(float kp, float ki, float kd);
public:
    bool detectGray();
};

#endif  // EV3_APP_LINETRACER_H_
