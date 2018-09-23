#ifndef EV3_APP_RUNMANAGER_H_
#define EV3_APP_RUNMANAGER_H_

#include "Odmetry.h"
#include "Mat.h"
#include "Sound.h"

class RunManager {
public:
    RunManager();
    static RunManager *getInstance(){
        static RunManager runManager;
        return &runManager;
    }
    // ~State();
    enum Section {
        SECTION_ERROR,    // エラー
        STRAIGHT_ZONE,
        CURB_ZONE,      // 走行区間
    	LOOSE_CURVE_ZONE,
    	TIGHT_CURVE_ZONE,
        GRAY,
        FINISHED      // 走行区間終了
    };
    enum Line {
        STRAIGHT,         // 直線
        LEFT_CURVE,      // 左曲がりカーブ
        RIGHT_CURVE,     // 右曲がりカーブ
        LINE_ERROR      // エラー
    };
    enum Zone {
        START,  // Start
        ZONE1,  // First Streat
        ZONE2,  // First Right Curb
        ZONE3,  // First Left Curb
        ZONE4,  // Second Right Curb
        ZONE5,  // Final Streat
        ZONE6,  // After Goal left curb
        ZONE7,   // last straight to lookupgate
        ZONE8,       // kaidan heno tsunagi
        ZONE9,
        ZONE10,
        ZONE11,
        ZONE12
    };
    void setOrigin();               // 距離を測る時の原点を設定する
    float getDistanceFromOrigin();  // 原点からの距離を測定する
    Line determineLine();           // 直線か曲線か判断する
    Section determineCourse();      // 現在の走行区間を判断する
    Zone mZone;                     // 最終的にはprivateに配置したい
    float mDeltaTheta;              // フィルタ済みの角度の時間変化→最終的にはprivateに配置

    uint8_t dLine;    // ライン情報（直線、右曲線、左曲線）
    float dDist;    // 距離
    uint16_t dCount;

    int8_t calcTurnByTheta(float deviation);
    void setPID(float kp, float ki, float kd);
    bool detectGray();
private:

    // 距離測定時の原点
    float mXOrigin;
    float mYOrigin;
    // 現在位置
    float mX;
    float mY;
    // 原点から現在位置までの距離
    float mDistance;

    // Odmetryを使ったPID用変数
    int32_t mDeviation;
    int32_t mIntegral;
    float mKp;
    float mKi;
    float mKd;

    Mat mMat;
    Sound *mSound = Sound::getInstance();

    Odmetry *mOdmetry = Odmetry::getInstance();
};


#endif  // EV3_APP_RUNMANAGER_H_
