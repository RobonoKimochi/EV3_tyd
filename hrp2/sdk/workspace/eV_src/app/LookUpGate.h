#ifndef EV3_APP_LOOKUPGATE_H_
#define EV3_APP_LOOKUPGATE_H_
#include "ev3api.h"
#include "AttitudeControl.h"
#include "MotorDriver.h"
#include "MeasureDistance.h"
#include "PidController.h"
#include "LineMonitor.h"

class LookUpGate
{
public:
	LookUpGate(AttitudeControl	*AttitudeControl, BalancingWalker* balancingwalker,MeasureDistance *measuredistance,
	 PidController* pidController, LineMonitor* lineMonitor);
	bool RunLookUpGate();

	
private:
	enum LookUpState {
		STOP,
		SETTLE,
        LEAN,
        THROGHGATE,
		BACKTOGATE,
    	STAND,
		BALANCE
    };
    
    enum ThruState
    {
		THRU_PRESTOP,
		THRU_THRU,
		THRU_FORWARD,
		THRU_FINISH
	};
	
	LookUpState mLookUpState;
	AttitudeControl	*mAttitudeControl;
	BalancingWalker *mbalancingwalker;
	MeasureDistance *mMeasureDistance;
	PidController* mPidController;
	LineMonitor* mLineMonitor;
	void stop();
	void lean();
	void ThroughLookUpGate();
	void BackLookUpGate();
	void Stand();
	void StartBalance();
	bool SettleCompleteFlag;
	bool LeanCompleteFlag;
	bool ThroghGateCompleteFlag;
	bool BackGateCompleteFlag;
	bool StandCompleteFlag;
	bool FindGateFlag;
	
	bool ThroughGateFlag;	// 2018_0907
	
	bool SecondFlag;
	bool LookUpCompFlag;
	int TimerCount;
	int ForwardCount;
	const int SlowForward = 10;
	ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
	ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel();
	bool preFindGateFlag;
	bool StopFlag;
	int StopCount;
	int vib_count;
	int ZeroCount;
	
	char mThruGateSts;
	char mBackGateSts;
#define LOOKUPGATEDOUBLE
//#undef LOOKUPGATEDOUBLE


};
#endif  // LOOKUPGATE
