/******************************************************************************
 *  LineTracerWithStarter.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class LineTracerWithStarter
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_LINETRACERWITHSTARTER_H_
#define EV3_APP_LINETRACERWITHSTARTER_H_

#include "Starter.h"
#include "LineTracer.h"
#include "Calibration.h"
#include "Remote.h"
#include "MeasureDistance.h"
#include "LookUpGate.h"
#include "Garage.h"
#include "BalancingWalker.h"
#include "Run_Stairs.h"
#include "Sound.h"

class LineTracerWithStarter {
public:
    LineTracerWithStarter(LineTracer* lineTracer,
               const Starter* starter,
               Calibration* calibration,
               Remote*		   Remote,
               LookUpGate*     LookUpGate,
               MeasureDistance *measureDistance,
               Garage*         garage,
               BalancingWalker* balancingWalker,
               Run_Stairs*      run_Stairs
               );
    virtual ~LineTracerWithStarter();

    void run();

    int debug;  // !!
	bool TailInit;
	bool LookUpCompFlag;

private:
    enum State {
        UNDEFINED,
    	CALIBRATION_TAIL,
        CALIBRATION_GYRO,
        CALIBRATION_BLACK,
        CALIBRATION_WHITE,
        WAITING_FOR_START,
        WALKING,
    	REMOTE,
    	LOOKUPGATE,
        STAIRS,
    	GARAGE,
        GRAY_DETECT
    };

    LineTracer* mLineTracer;
    const Starter* mStarter;
    Calibration* mCalibration;
	Remote* mRemote;
	LookUpGate* mLookUpGate;// = new LookUpGate();
	MeasureDistance* mMeasureDistance;// = new MeasureDistance();
	Garage* mGarage;
    BalancingWalker* mBalancingWalker;
    Run_Stairs* mRun_Stairs;
    Sound *mSound = Sound::getInstance();
public:
    State mState;
private:
	int TimeCount;

    void execUndefined();
	void execCalibrationTail();
    void execCalibrationGyro();
    void execCalibrationBlack();
    void execCalibrationWhite();
    void execWaitingForStart();
    void execWalking();
	void execLookUpGate();
	void execGarage();
	void remote();
    void execStairs() ;
    void execGrayDetect();

    TailMotor *mTailMotor = TailMotor::getInstance();
public:
    bool mStartSignal;
    char sts ;
};

#endif  // EV3_APP_LINETRACERWITHSTARTER_H_
