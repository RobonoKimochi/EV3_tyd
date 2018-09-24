/******************************************************************************
 *  LineTracerWithStarter.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class LineTracerWithStarter
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "LineTracerWithStarter.h"
#include "ev3api.h"
#include "MotorDriver.h"
#include "app.h"
#include "TailMotor.h"
#include "Seesaw.h"
/**
 * コンストラクタ
 * @param lineTracer ライントレーサ
 * @param starter    スタータ
 */
 LineTracerWithStarter::LineTracerWithStarter(LineTracer* lineTracer,
                                              const Starter* starter,
                                              Calibration* calibration,
 											 Remote*     Remote,
 											 LookUpGate*     LookUpGate,
 											 MeasureDistance *measureDistance,
 											 Garage*         Garage,
                                              BalancingWalker* balancingWalker,
											   Seesaw*			seesaw			)
    : TailInit(false),
      LookUpCompFlag(false),
      mLineTracer(lineTracer),
      mStarter(starter),
      mCalibration(calibration),
      mRemote(Remote),
      mLookUpGate(LookUpGate),
      mMeasureDistance(measureDistance),
      mGarage(Garage),
      mBalancingWalker(balancingWalker),
	  mSeesaw(seesaw),
      mState(UNDEFINED),
      TimeCount(0),
      mStartSignal(false)
      {
}

/**
 * デストラクタ
 */
LineTracerWithStarter::~LineTracerWithStarter() {
}

/**
 * ライントレースする
 */
void LineTracerWithStarter::run() {
    debug = mState; // !!
    switch (mState) {
    case UNDEFINED:
        execUndefined();
        break;
    case CALIBRATION_GYRO:
        execCalibrationGyro();
        break;
   case CALIBRATION_TAIL:
        execCalibrationTail();
        break;
    case CALIBRATION_BLACK:
        execCalibrationBlack();
        break;
    case CALIBRATION_WHITE:
        execCalibrationWhite();
        break;
    case WAITING_FOR_START:
        execWaitingForStart();
        break;
    case WALKING:
        execWalking();
        break;
    case REMOTE:
    	remote();
    	break;
    case LOOKUPGATE:
    	execLookUpGate();
    	break;
    case GRAY_DETECT:
        execGrayDetect();
        break;
	case SEESAW:
		execSeesaw();
		break;
    case GARAGE:
    	execGarage();
    	break;
    default:
        break;
    }
}

/**
 * 未定義状態の処理
 */
void LineTracerWithStarter::execUndefined() {

	ev3_lcd_set_font(EV3_FONT_MEDIUM);
#if RUN_COURSE == RUN_LEFT_COURSE
	ev3_lcd_draw_string("LeftCourse", 10, 10);
	ev3_lcd_draw_string("LeftEdge", 10, 30);
#elif RUN_COURSE == RUN_RIGHT_COURSE
	ev3_lcd_draw_string("RightCourse", 10, 10);
	ev3_lcd_draw_string("RightEdge", 10, 30);
#endif
	mTailMotor->init(0);
    mCalibration->init();
	mState = CALIBRATION_GYRO;
}

/**
 * テールモータのキャリブレーション
 */
void LineTracerWithStarter::execCalibrationTail() {

	ev3_lcd_draw_string("TAILCALIBRATION", 10, 50);

	if(TailInit == false) {
		mTailMotor->setPWM(-2);
	}

	if(mStarter->isPushed() == true) {
		mTailMotor->setPWM(0);
		mTailMotor->init(0);
		TailInit = true;
	}

	if(TailInit == true) {

		TimeCount++;
	}

	if(TimeCount > 100) {
		mState = CALIBRATION_BLACK;
        mSound->ok();
		mTailMotor->setAngle(94);	// 開始待ち時尻尾91°
        //mState = WAITING_FOR_START; // キャリブレーション飛ばし
	}


}

/**
 * ジャイロセンサのキャリブレーション
 */
void LineTracerWithStarter::execCalibrationGyro() {
    if (mCalibration->calibrateGyro(mStarter->isPushed()) == true) {

        mState = CALIBRATION_TAIL;
        mSound->ok();
        //mState = WAITING_FOR_START; // きゃりぶれ飛ばし
    }
}

/**
 * 黒キャリブレーション
 */
void LineTracerWithStarter::execCalibrationBlack() {
    if (mCalibration->calibrateBlack(mStarter->isPushed()) == true) {

        mState = CALIBRATION_WHITE;
        mSound->ok();
    }

}

/**
 * 白キャリブレーション
 */
void LineTracerWithStarter::execCalibrationWhite() {
    if (mCalibration->calibrateWhite(mStarter->isPushed()) == true) {

        mState = WAITING_FOR_START;
    	ev3_lcd_fill_rect(0,0,EV3_LCD_WIDTH,EV3_LCD_HEIGHT,EV3_LCD_WHITE);
#if RUN_COURSE == RUN_LEFT_COURSE
    	ev3_lcd_draw_string("LeftCourse", 10, 30);
		ev3_lcd_draw_string("LeftEdge", 10, 50);
#elif RUN_COURSE == RUN_RIGHT_COURSE
    	ev3_lcd_draw_string("RightCourse", 10, 30);
		ev3_lcd_draw_string("RightEdge", 10, 50);
#endif
		ev3_lcd_draw_string("READY", 60, 70);
        mSound->trumpet();
    }

}

/**
 * 開始待ち状態の処理
 */
void LineTracerWithStarter::execWaitingForStart() {


    if (mStarter->isPushed()|| mStartSignal == true) {

        mCalibration->calibrateLineThreshold();

        mTailMotor->setAngle(0);	// 開始時尻尾0°

        mState = WALKING;
    }
}

/**
 * 走行中状態の処理
 */
void LineTracerWithStarter::execWalking() {
	if(mRemote->RemoteState() == true) {
		mState = REMOTE;
	} else {
        mLineTracer->run();
#if RUN_COURSE == RUN_RIGHT_COURSE
        if( mLineTracer->isFinished() == true){

            mState = SEESAW;
#elif RUN_COURSE == RUN_LEFT_COURSE
        if( mLineTracer->isFinished() == true && mMeasureDistance->DetectGate() == true){
            mState = LOOKUPGATE;
#endif
		}
	}
}

/**
 * ルックアップゲート状態の処理
 */
void LineTracerWithStarter::execLookUpGate() {
	if(mRemote->RemoteState() == true) {
		mState = REMOTE;
	} else {
		LookUpCompFlag =mLookUpGate->RunLookUpGate();
		if(LookUpCompFlag == true) {
			// mState = GARAGE;
            mState = GRAY_DETECT;
		}
	}
}
/**
 * シーソー制御の処理
 */
void LineTracerWithStarter::execSeesaw()
{
	ev3_led_set_color(LED_GREEN);				/* 通常走行は緑 			*/

	mSeesaw->Initialize();						/* 初期化					*/
	mSeesaw->State();							/* 状態判定					*/
	mSeesaw->Run_Manage();						/* 走行制御					*/
}
/**
 * 車庫入れ状態の処理
 */
void LineTracerWithStarter::execGarage() {
	ev3_led_set_color(LED_ORANGE);
	if(mRemote->RemoteState() == true) {
		mState = REMOTE;
	} else {
		mGarage->StateGarage();
	}
}
/**
 * 灰色検知してガレージに渡す処理
 */
void LineTracerWithStarter::execGrayDetect() {
	ev3_led_set_color(LED_OFF);
	if(mRemote->RemoteState() == true) {
		mState = REMOTE;
	} else {
		if( mLineTracer->detectGray() == true){
            mState = GARAGE;
        }
	}
}
/**
 * リモート中の処理
 */
void LineTracerWithStarter::remote() {
	if(mRemote->RemoteState() == false) {
		mState = WALKING;
		ev3_led_set_color(LED_GREEN); /* 通常走行は緑 */
	} else {
		mRemote->RemoteControl();
	}
}
