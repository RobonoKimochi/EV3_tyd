#include "LookUpGate.h"

// 定数
#define THRU_GATE_PRESTOPCOUNT	(300)
#define THRU_GATE_POSTSTOPCOUNT	(100)
//#define THRU_GATE_FORWARDCOUNT	(500)
//#define THRU_GATE_FORWARDCOUNT	(2500)
//#define THRU_GATE_FORWARDCOUNT	(8000 / 4)	// 前進時間

#define THRU_GATE_FORWARDCOUNT	(3000 / 4)	// 前進時間1回目
#define THRU_GATE_SECOND_COUNT	(5500/4)	// 前新次官2回目
#define THRU_SPDDOWN_COUNT	(500 / 4)	// スルー減速時間

#define BACKLOOKUPGATE_COUNT	(4500/4)	// ゲートバック後退時間

#define LINE_PID_KEISU		( 2 / 10)	// ラインpid係数
	
/**
 * コンストラクタ
 */
 LookUpGate::LookUpGate(AttitudeControl	*AttitudeControl,BalancingWalker* balancingwalker, MeasureDistance *measuredistance,PidController* pidController, LineMonitor* lineMonitor)
    :  mLookUpState(STOP),
       mAttitudeControl(AttitudeControl),
       mbalancingwalker(balancingwalker),
       mMeasureDistance(measuredistance),
       mPidController(pidController),
       mLineMonitor(lineMonitor),
       SettleCompleteFlag(false),
       LeanCompleteFlag(false),
       ThroghGateCompleteFlag(false),
       BackGateCompleteFlag(false),
       StandCompleteFlag(false),
       FindGateFlag(false),
       ThroughGateFlag(false),
       SecondFlag(false),
       LookUpCompFlag(false),
       TimerCount(0),
       ForwardCount(0),
       preFindGateFlag(false),
       StopFlag(false),
       StopCount(0),
       vib_count(0),
       ZeroCount(0),
       mThruGateSts(0),
       mBackGateSts(0)
        {
 }


bool LookUpGate::RunLookUpGate(){
	

	switch (mLookUpState) {
	case STOP:
		stop();
		break;
		
    case SETTLE:
        lean();
        break;
		
	case LEAN:
        lean();
        break;
		
	case THROGHGATE:
        ThroughLookUpGate();
        break;
		
	case BACKTOGATE:
        BackLookUpGate();
        break;
		
    case STAND:
        Stand();
        break;
		
	case BALANCE:
        StartBalance();
        break;
		
    default:
        break;
    }
	
	
	if(SettleCompleteFlag == true) {
		mLookUpState = LEAN;
		SettleCompleteFlag = false;
	}
	
	if(LeanCompleteFlag == true) {
		mLookUpState = THROGHGATE;
		LeanCompleteFlag = false;
	}
	
	if(ThroghGateCompleteFlag == true) {
#ifdef LOOKUPGATEDOUBLE
		if(SecondFlag == true) {	//★★★ダブル用フラグ初期化
				mLookUpState = STAND;
		//	mbalancingwalker->init();	//★★★前倒防止制御 うまくいくかわからない
		} else {
			mLookUpState = BACKTOGATE;//★★★ダブル用フラグ初期化
				ev3_led_set_color(LED_ORANGE);
		}
#else
	//	if(SecondFlag == true) {	//★★★ダブル用フラグ初期化
			mLookUpState = STAND;
	//	mbalancingwalker->init();	//★★★前倒防止制御 うまくいくかわからない
	//	} else {
		//	mLookUpState = BACKTOGATE;//★★★ダブル用フラグ初期化
	//	}
		
#endif
		ThroghGateCompleteFlag = false;
	}
	
	if(BackGateCompleteFlag == true) {
		mLookUpState = THROGHGATE;
		BackGateCompleteFlag = false;
		SecondFlag = true;
	}
	
	if(StandCompleteFlag == true) {
		mLookUpState = BALANCE;
		StandCompleteFlag = false;
	}
	
	return(LookUpCompFlag);
}

void LookUpGate::stop(){

	mbalancingwalker->run();
	mbalancingwalker->setCommand(5,0);
	
	ZeroCount++;
	
	if(ZeroCount > 200) {
		mLookUpState = SETTLE;
	}
	
	return;
}

void LookUpGate::lean(){

	if(mLookUpState == SETTLE) {
		SettleCompleteFlag = mAttitudeControl->Settle();
	} else if (mLookUpState == LEAN) {
		
		LeanCompleteFlag = mAttitudeControl->Lean();
	}
	
	return;
}

/**
 * ルックアップゲートをくぐる
 */
void LookUpGate::ThroughLookUpGate(){
	int32_t s_errout;	// PID出力分
	int32_t s_pwmout;	// Pwm出力分
	
	int32_t s_forward_limit;
	
	// 前新次官を1回目と2回目で分ける
	if (SecondFlag == true)
	{
		s_forward_limit = THRU_GATE_SECOND_COUNT;
	}
	else
	{
		s_forward_limit = THRU_GATE_FORWARDCOUNT;
	}

	
	switch(mThruGateSts)
	{
		case THRU_PRESTOP:
			// 待機
			FindGateFlag = mMeasureDistance->ThroughtGate();	// 障害物検知を開始しておく
			
			TimerCount++;
			if(TimerCount > THRU_GATE_PRESTOPCOUNT)
			{
				TimerCount = 0;

				StopFlag = false;
				StopCount = 0;
				ev3_led_set_color(LED_RED);
				
//				mThruGateSts = THRU_THRU;
				mThruGateSts = THRU_FORWARD;
			}
			break;
			
		case THRU_THRU:
			// ゲートを通過するまで
			FindGateFlag = mMeasureDistance->ThroughtGate();

			/* PID制御 */
			mLineMonitor->LeanModecalLineThreshold();
//			mRightWheel.setPWM(SlowForward - mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()));
//			mLeftWheel.setPWM(SlowForward + mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()));

			// 決め打ちの値（黒：0、白：10、トレース値：5）でPID制御をさせる
			// 右車輪にはmForward(決め打ちの前進値）から現在のカラーセンサ値とライン閾値の差分を引いた値。
			// 左車輪にはmForward（決め打ちの前進値）から現在のカラーセンサ値とライン閾値の差分を加えた値。
//			mRightWheel.setPWM( mForward - 0.2 * mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) );
//			mLeftWheel.setPWM( mForward + 0.2 * mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) );
			s_errout = mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) * 2 / 10;
			mRightWheel.setPWM( SlowForward - s_errout);
			mLeftWheel.setPWM( SlowForward + s_errout);
			
			if((preFindGateFlag == true) && ( FindGateFlag == false) ) {
				StopFlag = true;
				
				ev3_led_set_color(LED_ORANGE);

				mThruGateSts = THRU_FORWARD;
			}
			break;
			
		case THRU_FORWARD:
			// くぐり中
			// 一定時間前進させる
			/* PID制御 */
			mLineMonitor->LeanModecalLineThreshold();
			// 決め打ちの値（黒：0、白：10、トレース値：5）でPID制御をさせる
			// 右車輪にはmForward(決め打ちの前進値）から現在のカラーセンサ値とライン閾値の差分を引いた値。
			// 左車輪にはmForward（決め打ちの前進値）から現在のカラーセンサ値とライン閾値の差分を加えた値。
//			mRightWheel.setPWM( mForward - 0.2 * mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) );
//			mLeftWheel.setPWM( mForward + 0.2 * mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) );
			s_errout = mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) ;

			mRightWheel.setPWM( SlowForward - s_errout);
			mLeftWheel.setPWM( SlowForward + s_errout);
			
			ForwardCount++;
			
			if (ForwardCount >  s_forward_limit)
//			if (ForwardCount > THRU_GATE_FORWARDCOUNT)
			{
				ForwardCount = 0;
				ev3_led_set_color(LED_RED);
				
				mThruGateSts = THRU_SPDDOWN;
			}
			break;
			
		case THRU_SPDDOWN:
			// 減速停止
			s_errout = mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) ;
			s_pwmout = SlowForward - s_errout;
			s_pwmout = s_pwmout * (THRU_SPDDOWN_COUNT - ForwardCount) / THRU_SPDDOWN_COUNT;

			mRightWheel.setPWM( s_pwmout );

			s_pwmout = SlowForward + s_errout;
			s_pwmout = s_pwmout * (THRU_SPDDOWN_COUNT - ForwardCount) / THRU_SPDDOWN_COUNT;
			mLeftWheel.setPWM( s_pwmout);
			
			ForwardCount++;
			if (ForwardCount >= THRU_SPDDOWN_COUNT)
			{
				ForwardCount = 0;
				ev3_led_set_color(LED_GREEN);
				mThruGateSts = THRU_FINISH;
			}
			break;
				
			
		case THRU_FINISH:
			// ゲートくぐった！！
			StopCount++;
			
			if(StopCount > THRU_GATE_POSTSTOPCOUNT) {
				ThroghGateCompleteFlag = true;
			//	ThroghGateCompleteFlag = true;//★★★ダブル用フラグ初期化
				mRightWheel.setPWM(0);
				mLeftWheel.setPWM(0);
				
				mThruGateSts = THRU_PRESTOP;

				mMeasureDistance->ThroughInit();	
				
	#ifdef LOOKUPGATEDOUBLE
				TimerCount = 0; //★★★ダブル用フラグ初期化
				ForwardCount = 0;//★★★ダブル用フラグ初期化
				StopCount = 0;//★★★ダブル用フラグ初期化
				StopFlag = false;//★★★ダブル用フラグ初期化
				FindGateFlag = false;//★★★ダブル用フラグ初期化
				preFindGateFlag = false;//★★★ダブル用フラグ初期化
	#endif
			}
			break;
			
		default:
			break;
	}
	
	preFindGateFlag = FindGateFlag;
	return;
}

#if 0
void LookUpGate::BackLookUpGate(){

	FindGateFlag = mMeasureDistance->ThroughtGate();
	if(TimerCount < 500) {
		TimerCount++;
	}
	
	if(TimerCount > 300) {
		
		/* PID制御あり */
		mLineMonitor->LeanModecalLineThreshold();
		mRightWheel.setPWM(-SlowForward / 2 - mPidController->LeancalControlledVariable(mLineMonitor->getDeviation())); //前進時と符号を逆にする
		mLeftWheel.setPWM(-SlowForward / 2 + mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()));
		ForwardCount++;
	}
	
	if(ForwardCount > 2500) {
		BackGateCompleteFlag = true;
		mRightWheel.setPWM(0);
		mLeftWheel.setPWM(0);

	}
	
	if((preFindGateFlag == true) && ( FindGateFlag == false) && (TimerCount > 300)) {
		StopFlag = true;
	}
	
	if(FindGateFlag == true) {
		StopFlag = false;
		StopCount = 0;
	}

	if(StopFlag == true) {
		StopCount++;
		if(StopCount > 300) {
			ThroghGateCompleteFlag = false;
			BackGateCompleteFlag = true;
			mRightWheel.setPWM(0);
			mLeftWheel.setPWM(0);
			StopCount = 0;
			TimerCount = 0;
			ForwardCount = 0;
			StopFlag = false;
			FindGateFlag = false;
			preFindGateFlag = false;
		}
	}
	
	preFindGateFlag = FindGateFlag;
	#if 0
	if(mMeasureDistance->DetectGate() == true) {
		preFindGateFlag = true;
	} else {
		preFindGateFlag = false;
	}
	#endif

	return;
}
#else
void LookUpGate::BackLookUpGate(){
	
	int32_t s_errout;	// PID出力分
	int32_t s_pwmout;	// Pwm出力分
	
	// 一定時間バックする
	
	switch(mBackGateSts)
	{
		case BACK_BACK:
			
			/* PID制御 */
			mLineMonitor->LeanModecalLineThreshold();

			// 決め打ちの値（黒：0、白：10、トレース値：5）でPID制御をさせる
			// 右車輪にはmForward(決め打ちの前進値）から現在のカラーセンサ値とライン閾値の差分を引いた値。
			// 左車輪にはmForward（決め打ちの前進値）から現在のカラーセンサ値とライン閾値の差分を加えた値。
		//			mRightWheel.setPWM( mForward - 0.2 * mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) );
		//			mLeftWheel.setPWM( mForward + 0.2 * mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) );
			s_errout = mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) ;

			// 白成分が多ければ、向かって左へ向かう
#ifdef BACK_STRAIGHT
			s_errout = 0;
#endif

			mRightWheel.setPWM(- SlowForward + s_errout);
			mLeftWheel.setPWM(- SlowForward - s_errout);
			// 符号どっちだ？
//			mRightWheel.setPWM(- SlowForward - s_errout);
//			mLeftWheel.setPWM(- SlowForward + s_errout);
			
			StopCount++;

			if(StopCount > BACKLOOKUPGATE_COUNT) 
			{
				StopCount = 0;
				ev3_led_set_color(LED_RED);
				mBackGateSts = BACK_SPDDOWN;
			}
			break;
			
		case BACK_SPDDOWN:
			// 減速停止
			s_errout = mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) ;

#ifdef BACK_STRAIGHT
			s_errout = 0;
#endif
			s_pwmout = -SlowForward + s_errout;
//			s_pwmout = -SlowForward - s_errout;
			s_pwmout = s_pwmout * (THRU_SPDDOWN_COUNT - StopCount) / THRU_SPDDOWN_COUNT;

			mRightWheel.setPWM( s_pwmout );

			s_pwmout = -SlowForward - s_errout;
//			s_pwmout = -SlowForward + s_errout;
			s_pwmout = s_pwmout * (THRU_SPDDOWN_COUNT - StopCount) / THRU_SPDDOWN_COUNT;
			mLeftWheel.setPWM( s_pwmout);
			
			StopCount++;
			
			if(StopCount > THRU_SPDDOWN_COUNT) 
			{
				ThroghGateCompleteFlag = false;
				BackGateCompleteFlag = true;
				
				mBackGateSts = BACK_BACK;

			//	ThroghGateCompleteFlag = true;//★★★ダブル用フラグ初期化
				mRightWheel.setPWM(0);
				mLeftWheel.setPWM(0);
				
				
		#ifdef LOOKUPGATEDOUBLE
				TimerCount = 0; //★★★ダブル用フラグ初期化
				ForwardCount = 0;//★★★ダブル用フラグ初期化
				StopCount = 0;//★★★ダブル用フラグ初期化
				StopFlag = false;//★★★ダブル用フラグ初期化
				FindGateFlag = false;//★★★ダブル用フラグ初期化
				preFindGateFlag = false;//★★★ダブル用フラグ初期化
		#endif
			}
			break;
			
		default:
			// 停止
			mRightWheel.setPWM(0);
			mLeftWheel.setPWM(0);
			
			break;
	}
			
	return;
}

#endif

void LookUpGate::Stand(){
	
	//mbalancingwalker->SettleMode(); //★★★前倒防止制御仮 うまくいくかわからない

	    mRightWheel.setPWM(-2);
        mLeftWheel.setPWM(-2);
	StandCompleteFlag = mAttitudeControl->Stand();
	
	#if(0)
	if((vib_count % 2) == 0) {
    	mRightWheel.setPWM(-1);
        mLeftWheel.setPWM(-1);
    	vib_count++;
    } else if ((vib_count % 2) == 1) {
    	vib_count++;
    	if(vib_count > 100) {
    		vib_count = 0;
    	}
    }
    #endif

	
	return;
}

void LookUpGate::StartBalance(){
	
//	mAttitudeControl->ReStartBalance();
	
//	    mRightWheel.setPWM(0);
//        mLeftWheel.setPWM(0);

	mbalancingwalker->setCommand( 5, 0 ) ;
	mbalancingwalker->run();	// OKか？

	LookUpCompFlag = true;
	return;
}

