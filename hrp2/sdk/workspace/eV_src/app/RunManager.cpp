#include "RunManager.h"
#include "app.h"

// テストコース時はTEST_COURSEを「１」 に、本番時は「０」にしてください。
#define TEST_COURSE (1)
//#define RUN_COURSE RUN_RIGHT_COURSE

// 次のゾーンに入ったかを判断する際に、コース形状を考慮に入れるかを選択する。
#define CHECK_COURSE(x) (line == (x))
//#define CHECK_COURSE(x) (1)

#if RUN_COURSE == RUN_LEFT_COURSE && TEST_COURSE
// テスト Lコース
#define ZONE1_LEN 260
#define ZONE2_LEN 139
#define ZONE3_LEN 151
#define ZONE4_LEN 56
#define ZONE5_LEN 52
#define ZONE6_LEN 59
#define ZONE7_LEN 228
#define ZONE8_LEN 30
#define ZONE9_LEN 52
#define GRAY_LEN  13

#elif RUN_COURSE == RUN_LEFT_COURSE && !TEST_COURSE
// 本番 Lコース
#define ZONE1_LEN 260
#define ZONE2_LEN 141
#define ZONE3_LEN 171
#define ZONE4_LEN 56
#define ZONE5_LEN 52
#define ZONE6_LEN 59
#define ZONE7_LEN 248
#define ZONE8_LEN 55
#define ZONE9_LEN 52
#define GRAY_LEN  13

#elif RUN_COURSE == RUN_RIGHT_COURSE && TEST_COURSE
// テスト Rコース
#define ZONE1_LEN 215
#define ZONE2_LEN 78
#define ZONE3_LEN 78
#define ZONE4_LEN 41
#define ZONE5_LEN 43 //48
#define ZONE6_LEN 34
#define ZONE7_LEN 40 // 52
#define ZONE8_LEN 45 //50
#define ZONE9_LEN 45 //50
#define ZONE10_LEN 230 //257
#define ZONE11_LEN 30 //48
#define ZONE12_LEN 10 //48
#define GRAY_LEN  13

#elif RUN_COURSE == RUN_RIGHT_COURSE && !TEST_COURSE
// 本番 Rコース
#define ZONE1_LEN 258
#define ZONE2_LEN 81
#define ZONE3_LEN 81
#define ZONE4_LEN 43
#define ZONE5_LEN 55
#define ZONE6_LEN 55
#define ZONE7_LEN 65
#define ZONE8_LEN 53
#define ZONE9_LEN 53
#define ZONE10_LEN 257
#define ZONE11_LEN 48
#define GRAY_LEN  13

#endif

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
#if RUN_COURSE == RUN_LEFT_COURSE

RunManager::Section RunManager::determineCourse() {
    static uint8_t line;    // ライン情報（直線、右曲線、左曲線）
    static uint16_t dist;    // 距離

    line = determineLine();
    dist = (uint16_t)getDistanceFromOrigin();
    dLine = line;
    dDist = dist;

    switch(mZone) {
    case START:
        mZone = ZONE1;
        setOrigin();
        return STRAIGHT_ZONE;

    case ZONE1:
        if(dist > ZONE1_LEN) {
//        if(dist > ZONE1_LEN && CHECK_COURSE(LEFT_CURVE)) {
            mZone = ZONE2;
            setOrigin();
//            mSound->ok();
        }
        return STRAIGHT_ZONE;

    case ZONE2:
        if(dist > ZONE2_LEN && CHECK_COURSE(STRAIGHT)) {
            mZone = ZONE3;
            setOrigin();
//            mSound->ok();
        }
        return LOOSE_CURVE_ZONE;

    case ZONE3:
        if(dist > ZONE3_LEN) {
//       	if(dist > ZONE3_LEN && CHECK_COURSE(RIGHT_CURVE)) {
            mZone = ZONE4;
            setOrigin();
//            mSound->ok();
        }
        return STRAIGHT_ZONE;

    case ZONE4:
        if(dist > ZONE4_LEN && CHECK_COURSE(STRAIGHT)) {
            mZone = ZONE5;
            setOrigin();
//            mSound->ok();
        }
        return TIGHT_CURVE_ZONE;

    case ZONE5:
        if(dist > ZONE5_LEN) {
//       	if(dist > ZONE5_LEN && CHECK_COURSE(RIGHT_CURVE)) {
            mZone = ZONE6;
            setOrigin();
//            mSound->ok();
        }
        return STRAIGHT_ZONE;

    case ZONE6:
        if(dist > ZONE6_LEN && CHECK_COURSE(STRAIGHT)) {
            mZone = ZONE7;
            setOrigin();
//            mSound->ok();
        }
        return TIGHT_CURVE_ZONE;

    case ZONE7:
        if(dist > ZONE7_LEN) {
//       	if(dist > ZONE7_LEN && CHECK_COURSE(LEFT_CURVE)) {
            mZone = ZONE8;
            setOrigin();
//            mSound->ok();
        }
        return STRAIGHT_ZONE;

    case ZONE8:
        if(CHECK_COURSE(LEFT_CURVE)) {
            setOrigin();
//            mSound->ok();
            mZone = ZONE9;
        }
        return TIGHT_CURVE_ZONE;

    case ZONE9:
        if(dist > ZONE8_LEN) {
            setOrigin();
//            mSound->ok();
            mZone = ZONE10;
        }
        return TIGHT_CURVE_ZONE;

    case ZONE10:
        return FINISHED;
    default:
        return SECTION_ERROR;
    }
}

#elif RUN_COURSE == RUN_RIGHT_COURSE

RunManager::Section RunManager::determineCourse(){
    static uint8_t line;    // ライン情報（直線、右曲線、左曲線）
    static uint16_t dist;    // 距離

    line = determineLine();
    dist = (uint16_t)getDistanceFromOrigin();
    dLine = line;
    dDist = dist;

    switch(mZone) {
    case START:
        mZone = ZONE1;
        setOrigin();
        return STRAIGHT_ZONE;

    case ZONE1:
        if(dist > ZONE1_LEN) {
//       	if(dist > ZONE1_LEN && CHECK_COURSE(LEFT_CURVE)) {
            mZone = ZONE2;
            setOrigin();
//            mSound->ok();
        }
        return STRAIGHT_ZONE;

    case ZONE2:
        if(dist > ZONE2_LEN && CHECK_COURSE(LEFT_CURVE)) {
            mZone = ZONE3;
            setOrigin();
            //mSound->ok();
        }
        return LOOSE_CURVE_ZONE;

    case ZONE3:
    	if(dist > ZONE3_LEN){
//        if(dist > ZONE3_LEN && CHECK_COURSE(STRAIGHT)){
            mZone = ZONE4;
            setOrigin();
 //           mSound->ok();
        }
        return LOOSE_CURVE_ZONE;

    case ZONE4:
        if(dist > ZONE4_LEN) {
//       	if(dist > ZONE4_LEN && CHECK_COURSE(LEFT_CURVE)) {
            mZone = ZONE5;
            setOrigin();
//            mSound->ready();
        }
//        return STRAIGHT_ZONE;
        return STRAIGHT_ZONE;

    case ZONE5:
        if(dist > ZONE5_LEN && line == STRAIGHT) {
//      if(dist > ZONE5_LEN && line == LEFT_CURVE) {
//       	if(dist > ZONE5_LEN && CHECK_COURSE(RIGHT_CURVE)) {
            mZone = ZONE6;
            setOrigin();
//            mSound->ready();
        }
        return TIGHT_CURVE_ZONE;

    case ZONE6:
        if(dist > ZONE6_LEN && CHECK_COURSE(STRAIGHT)) {
            mZone = ZONE7;
            setOrigin();
            //mSound->ok();
        }
        return TIGHT_CURVE_ZONE;

    case ZONE7:
        if(dist > ZONE7_LEN) {
//       	if(dist > ZONE7_LEN && CHECK_COURSE(RIGHT_CURVE)) {
            mZone = ZONE8;
            setOrigin();
            mSound->ok();
        }
        return STRAIGHT_ZONE;

    case ZONE8:
        if(dist > ZONE8_LEN && CHECK_COURSE(RIGHT_CURVE)) {
            mZone = ZONE9;
            setOrigin();
            //mSound->ready();
        }
        return TIGHT_CURVE_ZONE;

    case ZONE9:
        if(dist > ZONE9_LEN && CHECK_COURSE(STRAIGHT)) {
            mZone = ZONE10;
            setOrigin();
            //mSound->ready();
        }
        return TIGHT_CURVE_ZONE;

    case ZONE10:
      	if(dist > ZONE10_LEN) { /// && CHECK_COURSE(LEFT_CURVE)) {
            mZone = ZONE11;
            setOrigin();
            //mSound->ok();
        }
        return STRAIGHT_ZONE;

    case ZONE11:
        if(dist > ZONE11_LEN) {
            mZone = ZONE12;
            setOrigin();
            //mSound->ok();
        }
        return TIGHT_CURVE_ZONE;

    case ZONE12:
        if(dist > ZONE12_LEN) {
            mZone = ZONE13;
            setOrigin();
            mSound->ok();
        }
        return SLOW;

    case ZONE13:
    	return FINISHED;

    default:
        return SECTION_ERROR;
    }
}

#endif

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
