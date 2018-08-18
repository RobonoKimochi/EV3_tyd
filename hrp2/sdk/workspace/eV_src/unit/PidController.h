/******************************************************************************
 *  PidController.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/06/12
 *  Implementation of the Class BalancingWalker
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_UNIT_PIDCONTROLLER_H_
#define EV3_UNIT_PIDCONTROLLER_H_

#include "ev3api.h"


class PidController
{
public:
    PidController();
    virtual ~PidController();

    /**
     * PID制御でTurn値を算出する
     */
    int8_t calControlledVariable(int8_t deviation);
	int8_t LeancalControlledVariable(int8_t deviation);
    void setPID(float kp, float ki, float kd);

private:
    int32_t mDeviation;
    int32_t mIntegral;
    float mKp;
    float mKi;
    float mKd;
	
    const float mLKp = 3.0;
    const float mLKi = 0.00;
    const float mLKd = 2.00;
};

#endif  // EV3_UNIT_PIDCONTROLLER_H_
