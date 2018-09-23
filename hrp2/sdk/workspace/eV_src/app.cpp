/******************************************************************************
 *  app.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Implementation of the Task main_task
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "app.h"
#include "LineTracer.h"
#include "LineTracerWithStarter.h"
// added part
#include "Odmetry.h"
#include "UI.h"
// #include "BlueTooth.h"
#include "Logger.h"

#include "MotorDriver.h"
#include "SensorDriver.h"

#include "Remote.h"
#include "AttitudeControl.h"
#include "MeasureDistance.h"
#include "RunManager.h"
#include "TailMotor.h"
#include "BalancingWalker.h"
#include "Sound.h"


#include "Clock.h"

// デストラクタ問題の回避
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
void *__dso_handle=0;

// using宣言
using ev3api::ColorSensor;
using ev3api::GyroSensor;
using ev3api::Motor;
using ev3api::Clock;

// Device objects
// オブジェクトを静的に確保する。
//ColorSensor gColorSensor(PORT_3);
//GyroSensor  gGyroSensor(PORT_4);
//Motor       gLeftWheel(PORT_C);
//Motor       gRightWheel(PORT_B);
// Motor関連
ev3api::Motor gLeftWheel = MotorDriver::getInstance().getLeftWheel();
ev3api::Motor gRightWheel = MotorDriver::getInstance().getRightWheel();
// Sensor関連
ev3api::TouchSensor gTouchSensor = SensorDriver::getInstance().getTouchSensor();
ev3api::ColorSensor gColorSensor = SensorDriver::getInstance().getColorSensor();
ev3api::SonarSensor gSonarSensor = SensorDriver::getInstance().getSonarSensor();
ev3api::GyroSensor gGyroSensor = SensorDriver::getInstance().getGyroSensor();

// オブジェクトの定義
static LineMonitor     *gLineMonitor;
static Balancer        *gBalancer;
static BalancingWalker *gBalancingWalker;
static LineTracer      *gLineTracer;
static Starter			*gStarter;
static Calibration		*gCalibration;
static PidController	*gPidController;
static LineTracerWithStarter *gLineTracerWithStarter;
static MeasureDistance	*gMeasureDistance;
static UI				*gUI;
static Logger			*gLogger;
static TailMotor		*gTailMotor;// = TailMotor::getInstance();
static RunManager		*gRunManager;
static Garage			*gGarage;
static Remote			*gRemote;
static AttitudeControl	*gAttitudeControl;
static LookUpGate		*gLookUpGate;
static Odmetry			*gOdmetry;
static Sound 			*gSound;
static Seesaw			*gSeesaw;
static Clock			*gClock;

/**
 * EV3システム生成
 */
static void user_system_create() {
	tslp_tsk(2);
    // オブジェクトの作成
    gBalancer        = new Balancer();
    gBalancingWalker = new BalancingWalker(gGyroSensor,
                                           gLeftWheel,
                                           gRightWheel,
                                           gBalancer);
    gLineMonitor     = new LineMonitor(gColorSensor);
    gStarter         = new Starter(gTouchSensor);
    gPidController   = new PidController();
	gAttitudeControl = new AttitudeControl(gBalancingWalker, gGyroSensor);
	gMeasureDistance = new MeasureDistance(gSonarSensor);
	gLookUpGate      = new LookUpGate(gAttitudeControl,gBalancingWalker,gMeasureDistance, gPidController, gLineMonitor);
    gLineTracer      = new LineTracer(gLineMonitor, gBalancingWalker, gPidController);
    gCalibration     = new Calibration(gColorSensor, gGyroSensor, gLineMonitor);
	gRemote          = new Remote(gBalancingWalker);
	gGarage          = new Garage(gBalancingWalker, gLineTracer, gPidController, gLineMonitor);
    gUI             = new UI();
    gLogger         = new Logger();
	gOdmetry		= Odmetry::getInstance();
	gRunManager		= RunManager::getInstance();
	gTailMotor		= TailMotor::getInstance();
	gSound 			= Sound::getInstance();
	gClock			 = new Clock();
	gSeesaw		 			= new Seesaw(gAttitudeControl, gBalancingWalker, gLineTracer, gPidController, gLineMonitor, gGyroSensor, gOdmetry);
	gLineTracerWithStarter 	= new LineTracerWithStarter(gLineTracer, gStarter, gCalibration, gRemote, gLookUpGate , gMeasureDistance, gGarage, gBalancingWalker, gSeesaw);

    // 初期化完了通知
    ev3_led_set_color(LED_ORANGE);
    
	gSound->ready();
}

/**
 * EV3システム破棄
 */
static void user_system_destroy() {
    gLeftWheel.reset();
    gRightWheel.reset();

    delete gLineTracerWithStarter;
    delete gLineTracer;
    delete gStarter;
    delete gLineMonitor;
    delete gBalancingWalker;
    delete gBalancer;
    delete gCalibration;
    delete gPidController;
    delete gUI;
    delete gLogger;

/* 追加 */
    gGyroSensor.reset();
    /* colorとtouchセンサはどうする */
    delete gAttitudeControl;
    delete gMeasureDistance;
    delete gLookUpGate;
    delete gRemote;
    delete gGarage;
	delete gSeesaw;
    delete gOdmetry;
    delete gRunManager;
    delete gTailMotor;
	delete gSound;
	delete gClock;
}

/**
 * トレース実行タイミング
 */
void ev3_cyc_tracer(intptr_t exinf) {
    act_tsk(TRACER_TASK);
}

/**
 * メインタスク
 */
void main_task(intptr_t unused) {
    user_system_create();  // センサやモータの初期化処理

    // 周期ハンドラ開始
    ev3_sta_cyc(EV3_CYC_TRACER);
	ev3_sta_cyc(EV3_CYC_ODMETRY);
	ev3_sta_cyc(EV3_CYC_TAILMOTOR);

	// UIタスク開始
	act_tsk(UI_TASK);


    slp_tsk();  // バックボタンが押されるまで待つ

    // 周期ハンドラ停止
    ev3_stp_cyc(EV3_CYC_TRACER);
	ev3_stp_cyc(EV3_CYC_TAILMOTOR);

    user_system_destroy();  // 終了処理

    ext_tsk();
}

/**
 * ライントレースタスク
 */
void tracer_task(intptr_t exinf) {
    if (ev3_button_is_pressed(BACK_BUTTON)) {
        wup_tsk(MAIN_TASK);  // バックボタン押下
    } else {
//        gLineTracer->run();  // 倒立走行
        gLineTracerWithStarter->run();  // (倒立)走行
    }

#if USE_OUTPUT_LOG
  static int start_flag = 0;
  if ((gLineTracerWithStarter->mState == 6) // 6はLineTracerWithStarter::WALKING
	&& (start_flag == 0)) {
	  ev3_sta_cyc(EV3_CYC_LOGGER);
	  start_flag = 1;
  }
#endif

    ext_tsk();
}
/**
 * テールモータ周期タスク
 */
void ev3_cyc_tailmotor(intptr_t exinf) {
	act_tsk(TAILMOTOR_TASK);
}
/**
 * テールモータタスク
 */
void tailmotor_task(intptr_t exinf) {
	if(gLineTracerWithStarter->TailInit == true) {
		gTailMotor->moveTail();	// 尻尾制御
	}
	ext_tsk();
}
/**
 * オドメトリ周期タスク
 */
void ev3_cyc_odmetry(intptr_t exinf) {
	act_tsk(ODMETRY_TASK);
}
/**
 * オドメトリタスク
 */
void odmetry_task(intptr_t exinf) {
    gOdmetry->updateLocation();  // 倒立走行
    ext_tsk();
}
/**
 * ロガー周期タスク
 */
void ev3_cyc_logger(intptr_t exinf) {
    act_tsk(LOGGER_TASK);
}
/**
 * ロガータスク
 */
#define LOG_NUM 11
void logger_task(intptr_t exinf){
	float gLog[LOG_NUM];
	// gLog[0]  = gOdmetry->getX();					// X座標
	// gLog[1]  = gOdmetry->getY();					// Y座標
	// gLog[2]  = gOdmetry->getTheta();				// 角度
	// gLog[3]  = gColorSensor.getBrightness();		// カラーセンサ値
	// gLog[4]  = gGyroSensor.getAnglerVelocity();	// ジャイロ値
	// gLog[5]  = gBalancer->getForward();			// Forward値
	// gLog[6]  = gBalancer->getTurn();				// Turn値
	// gLog[7]  = gOdmetry->getDeltaTheta();		// 角度の時間変化（フィルタ済み）
	// gLog[8]  = ev3_battery_voltage_mV();			// バッテリー
	// gLog[9]  = gLineTracerWithStarter->debug;	// mState
	// gLog[10] = gTailMotor->mAngle;				// テールモータのアングル
	// gLog[11] = gSonarSensor.getDistance();		// 距離センサ
	// gLog[12] = gRunManager->mZone;				// 走行区間
	// gLog[13] = gRunManager->dDist;				// 走行区間
	// gLog[14] = gRunManager->dLine;				// 走行区間
	// gLog[15] = gRunManager->dCount;				// 走行区間(kaunto)

//	gLogger->putString("time;x;y;theta;deltaTheta;battery;state;sonardist;Zone;dist;Line;gyro;color;forward;turn;Rpwm;Lpwm;\n\r");
	gLogger->putString("time;x;y;theta;deltaTheta;sonar;gyro;color;forward;turn;Rpwm;Lpwm;\n\r");

	gLog[0]  = gOdmetry->getX();					// X座標
	gLog[1]  = gOdmetry->getY();					// Y座標
	gLog[2]  = gOdmetry->getTheta();				// 角度
	gLog[3]  = gOdmetry->getDeltaTheta();		// 角度の時間変化（フィルタ済み）
	gLog[4]  = gSonarSensor.getDistance();		// 距離センサ

	gLog[5] = gGyroSensor.getAnglerVelocity();	// ジャイロ値
	gLog[6] = gColorSensor.getBrightness();
	gLog[7]  = gBalancer->getForward();			// Forward値
	gLog[8]  = gBalancer->getTurn();				// Turn値
	gLog[9]  = gBalancingWalker->getLeftPwm();		//pwm
	gLog[10]  = gBalancingWalker->getRightPwm();	//pwm
//	gLog[11]  = gLineTracerWithStarter->debug;	// mState
//	gLog[12]  = gRunManager->mZone;				// 走行区間
//	gLog[13]  = gRunManager->dDist;				// 走行区間
//	gLog[14]  = gRunManager->dLine;				// 走行区間
//	gLog[15]  = ev3_battery_voltage_mV();		// バッテリー
//	gLog[10] = gRunManager->dCount;				//(kaunto)

    gLogger->sendLog(gLog,LOG_NUM);				// BlueToothを使ってログをteratermに送る
}
/**
 * リモート周期タスク
 */
void ev3_cyc_remote(intptr_t exinf){
	act_tsk(REMOTE_TASK);
}
/**
 * リモートタスク
 */
void remote_task(intptr_t exinf) {

//	char debug[50] = {0};
	gRemote->getsr();										// コマンドを入力
//	sprintf(debug,"\n\ryou put: %c \n\r",gRemote->c_debug); //
//	gUI->putString(debug);									// コマンドを表示

	/* ロボット停止コマンド */
	if (gRemote->c == 's') {
//		if (gClock->now() > 4000) {
			wup_tsk(MAIN_TASK);
			ev3_stp_cyc(EV3_CYC_REMOTE);
//			ev3_stp_cyc(EV3_CYC_TAILMOTOR);
			ev3_stp_cyc(EV3_CYC_ODMETRY);
//			ev3_stp_cyc(EV3_CYC_LOGGER);
//		}
	}

}
/**
 * UIタスク
 */
void ui_task(intptr_t exinf){
//	gUI->putString("\n\rMENU\n\r 1.START\n\r 2.Logger\n\r 3.Remote\n\r 9.STOP\n\r");
	gUI->putString("\n\rMENU\n\r s.START\n\r");
    while(1){
		if( gUI->getState() == UI::LOGGER){
			// Logger起動時はMenu画面を飛ばす
		}else{
			//
		}
        gUI->setState( gUI->getChar() );
        switch( gUI->getState() ){
        case UI::START:
			gLineTracerWithStarter->mStartSignal = true;
			/* ↓デバッグ用 */
#if USE_REMOTE_CONTROL_MODE
			ev3_sta_cyc(EV3_CYC_REMOTE);
			//gUI->putString("\n\rREMOTE MODE\n\r");
#endif

#if USE_OUTPUT_LOG
			/* デバッグ用 */
	        gLogger->init();
	        gOdmetry->clearLocation();
	        ev3_sta_cyc(EV3_CYC_LOGGER);
	        // gUI->putString("\n\rtime,x,y,theta,brightness,gyro,forward,turn,deltaTheta,battery,state,TailAngle,Sonar,Zone,dist,Line\n\r");
//			gUI->putString("\n\rtime,x,y,theta,deltaTheta,battery,state,sonar,Zone,dist,Line,count,gyro\n\r");
#endif
			continue;
        case UI::LOGGER:
            gLogger->init();
            gOdmetry->clearLocation();
//            ev3_sta_cyc(EV3_CYC_LOGGER);
			// gUI->putString("\n\rtime,x,y,theta,brightness,gyro,forward,turn,deltaTheta,battery,state,TailAngle,Sonar,Zone,dist,Line\n\r");
//			gUI->putString("\n\rtime,x,y,theta,deltaTheta,battery,state,sonar,Zone,dist,Line,count,gyro\n\r");
			continue;
        case UI::REMOTE_MODE:
        	 ev3_sta_cyc(EV3_CYC_REMOTE);
			 gUI->putString("\n\rREMOTE MODE\n\r");
        	continue;
		case UI::FINISH:
//			ev3_stp_cyc(EV3_CYC_LOGGER);
            wup_tsk(MAIN_TASK);
            continue;
        default:
        	/* ↓暫定対応：書き込み直後に表示される。おそらくバグのため要解析 */
 //           gUI->putString("\n\rImput 1 to 4");
            continue;
        }
    }
}
