#include "AttitudeControl.h"
/**
 * コンストラクタ
 */
 AttitudeControl::AttitudeControl(BalancingWalker* balancingwalker,const ev3api::GyroSensor& gyroSensor)
 :mbalancingwalker(balancingwalker),
  BackCount(0),
  TailCount(0),
  TimerCount(0),
  CompleteFlag(false),
  InitFlag(false),
  mGyroSensor(gyroSensor),
  TailSetFlag(false),
  TimerStart(false)
        {
 }


bool AttitudeControl::Settle(){

	CompleteFlag = false;
	mTailMotor->setPgain( TailPGainSettle );
	mTailMotor->setAngle( TailAngleSettle );
	mTailMotor->moveTail();
	
	if((mTailMotor->Complete() == true) || (TailCount > 5000)) {
		TailSetFlag =true;
	}
	if( TimerStart == false) {
		if(TailSetFlag == false) {
			mbalancingwalker->run();
			mbalancingwalker->setCommand(-5,0);
		} else if(((TailCount++ > 50) && (TailSetFlag == true))||(TailCount++ > 5000)){
			mbalancingwalker->setCommand(-5,0);
			BackCount = mbalancingwalker->SettleMode();
			ev3_led_set_color(LED_RED);

			if((BackCount > 20) && (mGyroSensor.getAnglerVelocity() < CutGyroThresh)) {
				TimerStart = true;
			}
		} else {
			mbalancingwalker->run();
		}
	}
	
	if(TailCount < 5001) {
		TailCount++;
	}
	
	if(TimerStart == true) {
		TimerCount++;
	}
	if(TimerCount > 500) {
		CompleteFlag = true;
	}

	return (CompleteFlag);
}

bool AttitudeControl::Lean(){

	CompleteFlag = false;
	
	mTailMotor->setTargetAngle( TailAngleLean );
	mTailMotor->setPgain( TailPGainLean );
	mTailMotor->setAngleSlow();
	if(mTailMotor->CompleteDull() == true) {
		CompleteFlag = true;
	}

	return(CompleteFlag);
}

bool AttitudeControl::Stand(){

	CompleteFlag = false;

	mTailMotor->setTargetAngle(TailAngleStand);
	mTailMotor->setPgain( TailPGainStand );
	mTailMotor->setAngleSlow();
	if(mTailMotor->CompleteDull() == true) {
		CompleteFlag = true;
	}

	return(CompleteFlag);
}

void AttitudeControl::KeepAttitude(){

	mTailMotor->setTargetAngle( TailAngleLean );
	mTailMotor->setPgain( TailPGainLean );
	mTailMotor->setAngleSlow();

	return;
}

void AttitudeControl::ReStartBalance(){

	if(InitFlag == false) {
		mbalancingwalker->init();
		InitFlag =true;
	}
	
	mTailMotor->setTargetAngle(TailAngleBalance);
	mTailMotor->setPgain(TailPGainBalance);
	mTailMotor->setAngleSlow();

	mbalancingwalker->run();

	return;
}
