/******************************************************************************
 *  LineTracer.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_GARAGE_H_
#define EV3_APP_GARAGE_H_

// unit内のヘッダをインクルード
#include "LineTracer.h"
#include "BalancingWalker.h"

#include "MotorDriver.h"
#include "RunManager.h"
#include "TailMotor.h"
#include "Odmetry.h"

//#define DOLEAN
class Garage
{
	public:
		Garage(	BalancingWalker* balancingWalker, // コンストラクタ
				LineTracer* lineTracer,
				PidController* pidController,
				LineMonitor* lineMonitor);

		/**
		* @brief 			状態に対応した処理を実行。
		*
		* @param			無し
		*
		* @return 			無し
		*
		* @detail 			状態に応じた処理関数を呼び出す。
		*/
		void StateGarage();

		/**
		* @brief 			初期化処理
		*
		* @param			無し
		*
		* @return 			無し
		*
		* @detail 			ガレージ処理開始時の原点設定し、
		*					ガレージまで進行に状態遷移
		*/
		void Initialize();

		/**
		* @brief 			ガレージ内の停止位置まで走行処理
		*
		* @param			無し
		*
		* @return 			無し
		*
		* @detail 			ガレージの尻尾倒立地点までライントレース制御で進行。
		*					地点到達後、傾斜中に状態遷移。
		*/
		void GoToGarage();

		/**
		* @brief 			調整処理
		*
		* @param			無し
		*
		* @return 			無し
		*
		* @detail 			尻尾倒立状態から、ガレージ内の停止地点より
		*					手前で停止した場合、調整を行う。
		*					調整後、停止処理に状態遷移。
		*/
		void Adjust();

		/**
		* @brief 			停止処理
		*
		* @param			無し
		*
		* @return 			無し
		*
		* @detail 			両車輪を停止させる。
		*/
		void Stop();			// 停止処理関数
	private:
		enum State {
        UNDEFINED,
		INIT,		// 初期状態
		WALKING,	// ガレージまで進行状態
		ADJUST,		// 調整制御状態
		STOP		// 停止状態
    	};
    	ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
		ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel();
    	TailMotor *mTailMotor = TailMotor::getInstance();
    	RunManager *mRunManager = RunManager::getInstance();
    	Odmetry *mOdmetry = Odmetry::getInstance();
    	BalancingWalker* mBalancingWalker;
		LineTracer*      mLineTracer;
		PidController*   mPidController;
		LineMonitor*     mLineMonitor;
    	int32_t          mTailAngle;		// テイルの目標角度
    	int32_t          mForward;			// PID制御値
    	float            mLeanPoint;		// 尻尾倒立地点までの距離
    	float            mGaragePoint;		// 停止距離
    	int32_t          mAdjustSpeed;		// 調整時の移動速度
    	State            mState;			// 状態変数
};
#endif  // EV3_APP_LINETRACER_H_
