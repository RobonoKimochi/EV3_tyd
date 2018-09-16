/******************************************************************************
 *  PidController.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/06/12
 *  Implementation of the Class BalancingWalker
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "app.h"
#include "PidController.h"

/**
 * コンストラクタ
 */
PidController::PidController()
    : mDeviation(0),
      mIntegral(0),
      mKp(2.00),
      mKi(0.00),
      mKd(5.00) {
}

/**
 * デストラクタ
 */
PidController::~PidController() {
}

/**
 * PID制御でのTurn値を算出する
 * @param deviation  ラインの閾値との差分（偏差）
 */
int8_t PidController::calControlledVariable(int8_t deviation) {
// #define KPID_P_FACTOR       (float)( 2.0)
// #define KPID_I_FACTOR       (float)( 0.00)
// #define KPID_D_FACTOR       (float)( 5.00)
#if RUN_COURSE == RUN_LEFT_COURSE
#define KPID_EDGE_FACTOR    (int32_t)(-1)        // ライントレース方向 1 or -1
#elif RUN_COURSE == RUN_RIGHT_COURSE
#define KPID_EDGE_FACTOR    (int32_t)(1)        // ライントレース方向 1 or -1
#endif

#define KPID_TURN_LIMIT     (int32_t)(100)      // 旋回指示値 限界値

    int32_t brightness_P;                       // P成分
    int32_t brightness_I;                       // I成分
    int32_t brightness_D;                       // D成分
    int32_t turn_P;                             // P項演算値
    int32_t turn_I;                             // I項演算値
    int32_t turn_D;                             // D項演算値
    int32_t turn;                               // 旋回命令: -100 (左旋回) ～ 100 (右旋回)

    brightness_P = (int32_t)deviation;
    brightness_I = mIntegral + brightness_P;
    brightness_D = brightness_P - mDeviation;

    mDeviation = brightness_P;                              // 次回演算用に記憶
    mIntegral = brightness_I;

    /* P項演算 */
    turn_P = (int32_t)(mKp * (float)brightness_P);
    turn_P *= KPID_EDGE_FACTOR;

    /* I項演算 */
    turn_I = (int32_t)(mKi * (float)brightness_I);
    turn_I *= KPID_EDGE_FACTOR;

    /* D項演算 */
    turn_D = (int32_t)(mKd * (float)brightness_D);
    turn_D *= KPID_EDGE_FACTOR;

    /* 旋回指示値設定   */
    turn = turn_P + turn_I + turn_D;

    if (turn >= KPID_TURN_LIMIT) {
        turn = KPID_TURN_LIMIT;
    } else if (turn <= (-1 * KPID_TURN_LIMIT)) {
        turn = -1 * KPID_TURN_LIMIT;
    }

    return turn;
}

/**
 * PIDのパラメータを設定する
 * @param kp P制御値
 * @param ki I制御値
 * @param kd D制御値
 */
void PidController::setPID(float kp, float ki, float kd) {
    mKp = kp;
    mKi = ki;
    mKd = kd;
}

int8_t PidController::LeancalControlledVariable(int8_t deviation) {
// #define KPID_P_FACTOR       (float)( 2.0)
// #define KPID_I_FACTOR       (float)( 0.00)
// #define KPID_D_FACTOR       (float)( 5.00)
#if RUN_COURSE == RUN_LEFT_COURSE
#define KPID_EDGE_FACTOR    (int32_t)(-1)        // ライントレース方向 1 or -1
#elif RUN_COURSE == RUN_RIGHT_COURSE
#define KPID_EDGE_FACTOR    (int32_t)(1)        // ライントレース方向 1 or -1
#endif
#define KPID_TURN_LIMIT     (int32_t)(100)      // 旋回指示値 限界値

    int32_t brightness_P;                       // P成分
    int32_t brightness_I;                       // I成分
    int32_t brightness_D;                       // D成分
    int32_t turn_P;                             // P項演算値
    int32_t turn_I;                             // I項演算値
    int32_t turn_D;                             // D項演算値
    int32_t turn;                               // 旋回命令: -100 (左旋回) ～ 100 (右旋回)

    brightness_P = (int32_t)deviation;
    brightness_I = mIntegral + brightness_P;
    brightness_D = brightness_P - mDeviation;

    mDeviation = brightness_P;                              // 次回演算用に記憶
    mIntegral = brightness_I;

    /* P項演算 */
    turn_P = (int32_t)(mLKp * (float)brightness_P);
    turn_P *= KPID_EDGE_FACTOR;

    /* I項演算 */
    turn_I = (int32_t)(mLKi * (float)brightness_I);
    turn_I *= KPID_EDGE_FACTOR;

    /* D項演算 */
    turn_D = (int32_t)(mLKd * (float)brightness_D);
    turn_D *= KPID_EDGE_FACTOR;

    /* 旋回指示値設定   */
    turn = turn_P + turn_I + turn_D;

    if (turn >= KPID_TURN_LIMIT) {
        turn = KPID_TURN_LIMIT;
    } else if (turn <= (-1 * KPID_TURN_LIMIT)) {
        turn = -1 * KPID_TURN_LIMIT;
    }

    return turn;
}
