#ifndef EV3_UNIT_ATTITUDECONTROL_H_
#define EV3_UNIT_ATTITUDECONTROL_H_
#include "ev3api.h"
#include "BalancingWalker.h"
#include "TailMotor.h"
// #include "SensorDriver.h"
#include "GyroSensor.h"

class AttitudeControl
{
public:
	AttitudeControl(BalancingWalker *balancingwalker, const ev3api::GyroSensor& gyroSensor);
	bool Settle();
	bool Lean();
	bool Stand();
	void KeepAttitude();
	void ReStartBalance();

private:
	enum SettleState {
        TAILDOWN,
        CALIBRATION_GYRO,
        CALIBRATION_BLACK,
        CALIBRATION_WHITE,
    };
	BalancingWalker* mbalancingwalker;
	TailMotor* mTailMotor = TailMotor::getInstance();
	SettleState mSettleState;

	const float TailPGainSettle = 0.8F;
//	const float TailPGainLean = 2.0F;
	const float TailPGainLean = 3.0F;
//	const float TailIGainLean = 1.0F;
	const float TailIGainLean = 3.0F;	// 0924 増やす（目標までいけない対策）
	
	const float TailPGainStand = 6.0F;
	const float TailIGainStand = 6.0F;
	const float TailPGainBalance = 1.0F;
	const int TailAngleSettle = 80;
//	const int TailAngleLean = 57;		// 2018_0922 61から変更松原
//	const int TailAngleLean = 54;		// 2018_0924 57から変更松原
	const int TailAngleLean = 60;		// 2018_0929 54から変更豊田
//	const int TailAngleStand = 83;
	const int TailAngleStand = 71;
	const int TailAngleBalance = 1;
	const int CutGyroThresh = -5;
	int BackCount;
	int TailCount;
	int TimerCount;
	bool CompleteFlag;
	bool InitFlag;
	const ev3api::GyroSensor& mGyroSensor;
	bool TailSetFlag;
	bool TimerStart;

};
#endif  // ATTITUDECONTROL
