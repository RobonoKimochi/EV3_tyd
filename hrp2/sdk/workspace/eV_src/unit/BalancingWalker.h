/******************************************************************************
 *  BalancingWalker.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class BalancingWalker
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_UNIT_BALANCINGWALKER_H_
#define EV3_UNIT_BALANCINGWALKER_H_

#include "GyroSensor.h"
#include "Motor.h"
#include "BalancerCpp.h"
#include "Filter.h"

class BalancingWalker {
public:
    static const int LOW;
    static const int NORMAL;
    static const int HIGH;

    BalancingWalker(const ev3api::GyroSensor& gyroSensor,
                    ev3api::Motor& leftWheel,
                    ev3api::Motor& rightWheel,
                    Balancer* balancer);
    virtual ~BalancingWalker();

    void init();
    void run();
    void runWithoutBalancing();
    void setCommand(int forward, int turn);
    bool detectFall();
	int SettleMode();
	void GyroChange(int16_t angle);
	int getLeftPwm();
	int getRightPwm();
	
	float mGyroLpf;

private:
    const ev3api::GyroSensor& mGyroSensor;
    ev3api::Motor& mLeftWheel;
    ev3api::Motor& mRightWheel;
    Balancer* mBalancer;
    Filter * mFilter;
    int mForward;
    int mTurn;
    int mTimerFallDown;
	int back_count;
};

#endif  // EV3_UNIT_BALANCINGWALKER_H_
