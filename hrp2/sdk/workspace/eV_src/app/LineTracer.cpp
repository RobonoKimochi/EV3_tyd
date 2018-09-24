/******************************************************************************
 *  LineTracer.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "LineTracer.h"

/**
 * コンストラクタ
 * @param lineMonitor     ライン判定
 * @param balancingWalker 倒立走行
 */
LineTracer::LineTracer(LineMonitor* lineMonitor,
                       BalancingWalker* balancingWalker,
                       PidController* pidController)
    : debug(0),
      mLineMonitor(lineMonitor),
      mBalancingWalker(balancingWalker),
      mPidController(pidController),
      mIsInitialized(false),
      mIsFinished(false),
      percent(1.0) {
}

/**
 * デストラクタ
 */
LineTracer::~LineTracer() {
}

/**
 * ライントレースする
 */
void LineTracer::run() {
    if (mIsInitialized == false) {
        mBalancingWalker->init();
        mIsInitialized = true;
    }
    // コース状況を判断する
    mSection = mRunManager->determineCourse();

#if USE_DEBUG_MODE

#if USE_CONSTANT_PID
    /* デバッグ用処理 */
    /* 走行区間によるPIDの切り替えを無効化 */
    mSection = RunManager::STRAIGHT_ZONE;
#endif
#endif

        int direction = calcDirection();
#if 1
    	//両輪のPwm値がマイナスの時に符号を逆にする。主にスタート用
    	if((mBalancingWalker->getLeftPwm() < 0) && (mBalancingWalker->getRightPwm() < 0)) {
    		direction = -direction / 2;
    		direction = direction * (1.0f - percent);
    		percent = percent * 0.9f;
     	}
#endif

#if 1 /* ターン値ガード */

    	if (direction > TURN_GUARD) {
    		direction = TURN_GUARD;
    	} else if (direction < -TURN_GUARD) {
    		direction = -TURN_GUARD;
    	}
#endif

#if !USE_LINE_TRACE
    direction = 0;
#endif

/* PID STRAIGHT_ZONE test */
/* P 初期値:0.2  前回：0.1  今回：0.1      */
/* I 初期値:0    前回：0    今回：0        */
/* D 初期値:9    前回：14    今回：12       */

#if USE_DEBUG_MODE
    mPidController->setPID(0.1, 0, 12);
    mBalancingWalker->setCommand(CONSTANT_FORWARD_VAL, direction);	//■■速度は暫定
    mBalancingWalker->run();

#if USE_GRAY_THRESH
    mLineMonitor->LineThresholdGray();
#endif
#if LOOKUPGATE_TEST_MODE
    mIsFinished = true;
#endif

#else
    if( mSection == RunManager::STRAIGHT_ZONE){
    	mPidController->setPID(0.2, 0, 9.0);
        mBalancingWalker->setCommand(CONSTANT_FORWARD_VAL, direction);
        mBalancingWalker->run();
    }else if(mSection == RunManager::CURB_ZONE){
/* PID CURB_ZONE test */
/* P 初期値:0.5   前回：0.8  今回：0.7       */
/* I 初期値:0     前回：0    今回：0        */
/* D 初期値:10    前回：10   今回：12       */
    	mPidController->setPID(0.42, 0, 12.0);
        mBalancingWalker->setCommand(CONSTANT_FORWARD_VAL, direction);
        mBalancingWalker->run();
    }else if(mSection == RunManager::LOOSE_CURVE_ZONE){
    	mPidController->setPID(0.39, 0, 20.0);
        mBalancingWalker->setCommand(CONSTANT_FORWARD_VAL, direction);
        mBalancingWalker->run();
    }else if(mSection == RunManager::TIGHT_CURVE_ZONE){
    	mPidController->setPID(0.57, 0, 20.0);
        mBalancingWalker->setCommand(CONSTANT_FORWARD_VAL, direction);
        mBalancingWalker->run();
    }else if(mSection == RunManager::SLOW){
    	mLineMonitor->LineThresholdGray();
    	mPidController->setPID(0.5, 0, 40.0);
        mBalancingWalker->setCommand(40, direction);	//■■速度は暫定
        mBalancingWalker->run();
    }else if( mSection == RunManager::FINISHED){
        mIsFinished = true;
    	mLineMonitor->LineThresholdGray();
    	mPidController->setPID(1.0, 0, 10.0);
        mBalancingWalker->setCommand(20, direction);	//■■速度は暫定
        mBalancingWalker->run();
    }

#endif

    // アイデア②→LineTracer内でスタートから車庫まで（ソース管理が難しい?）
    // }else if(mSection == RunManager::LOOKUPGATE){
    //     // ルックアップゲート
    // }else if(mSection == RunManager::STAIRS){
    //     // 階段
    // }else if(mSection == RunManager::GARAGE){
    //     // 車庫入れ
    // }
}

/**
 * ライントレースする
 * (バランス制御をしない）
 */
void LineTracer::runWithoutBalancing() {
    if (mIsInitialized == false) {
        mBalancingWalker->init();
        mIsInitialized = true;
    }
    // コース状況を判断する
//    mSection = mRunManager->determineCourse();
//    if( mSection == RunManager::LINETRACE){
        int direction = calcDirection();
        mBalancingWalker->setCommand(50, direction);

        mBalancingWalker->runWithoutBalancing();
//    }else if( mSection == RunManager::FINISHED){
        mIsFinished = true;
//    }
    // アイデア②→LineTracer内でスタートから車庫まで（ソース管理が難しい?）
    // }else if(mSection == RunManager::LOOKUPGATE){
    //     // ルックアップゲート
    // }else if(mSection == RunManager::STAIRS){
    //     // 階段
    // }else if(mSection == RunManager::GARAGE){
    //     // 車庫入れ
    // }
}

bool LineTracer::detectGray() {
	
	return true;

}
/**
 * 走行体の向きを計算する
 * @param isOnLine true:ライン上/false:ライン外
 * @retval 30  ライン上にある場合(右旋回指示)
 * @retval -30 ライン外にある場合(左旋回指示)
 */
int LineTracer::calcDirection() {
    // ★PID走行
   return mPidController->calControlledVariable(mLineMonitor->getDeviation());
}
/**
 * 走行区間が終了したか戻り値で返す
 */
bool LineTracer::isFinished() {
    if( mIsFinished == true){
        return true;
    }else{
        return false;
    }
}
/**
 * @param ki I制御値
 * @param kd D制御値
 */
// void LineTracer::setPID(float kp, float ki, float kd) {
//     mPidController->setPID(kp,ki,kd);
//     // return;
// }
