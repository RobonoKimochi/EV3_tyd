#include "RunManager.h"
#include "app.h"

/**
 * コンストラクタ
 * @param lineMonitor     ライン判定
 * @param balancingWalker 倒立走行
 */
RunManager::RunManager()
    : mZone(START),
      mDeltaTheta(0),
      mXOrigin(0),
      mYOrigin(0),
      mX(0),
      mY(0),
      mDistance(0),
      mDeviation(0),
      mIntegral(0),
      mKp(10.00),
      mKi(0.00),
      mKd(0.00)
{

}
/**
 * 現在走行しているコースを判断する関数
 * (前回区間から一定距離以上走行) かつ (ラインの状態が正しい）
 * の2つの条件から区間の切り替わりを判断します
 * 最終的に最後のストレート区間を認識したのちに
 * 難関突破のアプリケーションに切り替えを行います。
 */
RunManager::Section RunManager::determineCourse(){
    static uint8_t line;    // ライン情報（直線、右曲線、左曲線）
    static uint16_t dist;    // 距離
    // static uint16_t count = 0;
    // count++;                // 32s => 4ms * 250
    // if(count > 8000){
        // mZone = ZONE7;
        // return FINISHED;
    // }
    line = determineLine();
    dist = (uint16_t)getDistanceFromOrigin();
    dLine = line;
    dDist = dist;
    // dCount = count;

    switch(mZone){
    case START:
        mZone = ZONE1;      // Zone1 に切り替え
        // mSound.play();      // 効果音を再生
        setOrigin();        // 原点の更新
        // mSound.play();      // 効果音を再生
        // 余裕があれば、このタイミングでPID値と速度を変えてスピードアップを図りたい
        return STRAIGHT_ZONE;
        // return SECTION_ERROR
    case ZONE1:
        if( dist > 180){
            mZone = ZONE2;  // Zone2 に切り替え
            // mSound->play();      // 効果音を再生
            setOrigin();    // 原点の更新
        }
        return STRAIGHT_ZONE;
    case ZONE2:
        // mSound.play();      // 効果音を再生
        if( dist > 1 && line == RIGHT_CURVE){
            mZone = ZONE3;  // Zone2 に切り替え
            // mSound->play();      // 効果音を再生
            setOrigin();    // 原点の更新
        }
        return CURB_ZONE;
    case ZONE3:
#if RUN_COURSE == RUN_LEFT_COURSE
        if( dist > 20 && line == LEFT_CURVE){
#elif RUN_COURSE == RUN_RIGHT_COURSE
        if( dist > 10 && line == LEFT_CURVE){
#endif
            mZone = ZONE4;  // Zone3 に切り替え
            // mSound->play();      // 効果音を再生
            setOrigin();    // 原点の更新
        }
        return CURB_ZONE;
    case ZONE4:
#if RUN_COURSE == RUN_LEFT_COURSE
        if( dist > 20 && line == RIGHT_CURVE){
#elif RUN_COURSE == RUN_RIGHT_COURSE
        if( dist > 10 && line == RIGHT_CURVE){
#endif
            mZone = ZONE5;  // Zone4 に切り替え
            // mSound->play();      // 効果音を再生
            setOrigin();    // 原点の更新
        }
        return CURB_ZONE;
    case ZONE5:
#if RUN_COURSE == RUN_LEFT_COURSE
        if( dist > 20 && line == STRAIGHT){
#elif RUN_COURSE == RUN_RIGHT_COURSE
        if( dist > 10 && line == STRAIGHT){
#endif
            mZone = ZONE6;  // Zone5 に切り替え
            // mSound->play();      // 効果音を再生
            setOrigin();    // 原点の更新
        }
        return CURB_ZONE;
    case ZONE6:
#if RUN_COURSE == RUN_LEFT_COURSE
        if( dist > 150.){
#elif RUN_COURSE == RUN_RIGHT_COURSE
        if( dist > 90){
#endif
            mZone = ZONE7;  // Zone5 に切り替え
            // mSound->play();      // 効果音を再生
            setOrigin();    // 原点の更新
        }
        return STRAIGHT_ZONE;
    case ZONE7:
#if RUN_COURSE == RUN_LEFT_COURSE
        if( dist > 1 && line == LEFT_CURVE){
            mZone = ZONE8;  // Zone5 に切り替え
            // mSound->play();      // 効果音を再生
            setOrigin();    // 原点の更新
        }

        return GRAY;
#elif RUN_COURSE == RUN_RIGHT_COURSE
        if( detectGray() == true){
            return GRAY;
        }
        return FINISHED;
#endif
    case ZONE8:
        if( dist > 20){
            return GRAY;
        }
        return FINISHED;
    default:
        return SECTION_ERROR;// Error
    }
}

/**
 * 角度の時間変化率から現在の走行ラインが
 * 直線、右曲線、左曲線かを判断する
 */
RunManager::Line RunManager::determineLine(){
    mDeltaTheta = mOdmetry->getDeltaTheta();
    if( mDeltaTheta > -0.0015 && mDeltaTheta < 0.0015){
        return STRAIGHT;
    }else if( mDeltaTheta <= -0.0015){
        return RIGHT_CURVE;
    }else if( mDeltaTheta >= 0.0015){
        return LEFT_CURVE;
    }else{
        return LINE_ERROR;
    }
}
/**
 * 灰色検知
 */
bool RunManager::detectGray(){
    mDeltaTheta = mOdmetry->getDeltaTheta();
    if( mDeltaTheta < -0.0005){
        return true;
    }
    return false;
}
/**
 * 現在地を原点を設定する
 */
void RunManager::setOrigin(){
    mXOrigin = mOdmetry->getX();
    mYOrigin = mOdmetry->getY();
}
/**
 * 設定した原点から現在地までの直線距離を返す
 */
float RunManager::getDistanceFromOrigin(){
    mX = mOdmetry->getX();
    mY = mOdmetry->getY();
    return (mMat.Dist(mXOrigin,mYOrigin,mX,mY));
}

int8_t RunManager::calcTurnByTheta(float deviation) {
// #define KPID_P_FACTOR       (float)( 2.0)
// #define KPID_I_FACTOR       (float)( 0.00)
// #define KPID_D_FACTOR       (float)( 5.00)
// #define KPID_EDGE_FACTOR    (int32_t)(-1)        // ライントレース方向 1 or -1

#define KPID_TURN_LIMIT     (int32_t)(100)      // 旋回指示値 限界値

    float theta_P;                       // P成分
    float theta_I;                       // I成分
    float theta_D;                       // D成分
    int32_t turn_P;                             // P項演算値
    int32_t turn_I;                             // I項演算値
    int32_t turn_D;                             // D項演算値
    int32_t turn;                               // 旋回命令: -100 (左旋回) ～ 100 (右旋回)

    theta_P = deviation;
    theta_I = mIntegral + theta_P;
    theta_D = theta_P - mDeviation;

    mDeviation = theta_P;                              // 次回演算用に記憶
    mIntegral = theta_I;

    /* P項演算 */
    turn_P = (int32_t)(mKp * theta_P);
    // turn_P *= KPID_EDGE_FACTOR;

    /* I項演算 */
    turn_I = (int32_t)(mKi * theta_I);
    // turn_I *= KPID_EDGE_FACTOR;

    /* D項演算 */
    turn_D = (int32_t)(mKd * theta_D);
    // turn_D *= KPID_EDGE_FACTOR;

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
void RunManager::setPID(float kp, float ki, float kd) {
    mKp = kp;
    mKi = ki;
    mKd = kd;
}
