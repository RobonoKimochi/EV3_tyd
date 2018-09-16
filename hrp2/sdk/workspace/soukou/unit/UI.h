#ifndef EV3_UNIT_UI_H_
#define EV3_UNIT_UI_H_

#include "BlueTooth.h"
#include "BalancerCpp.h"

#define TOP_MENU    0
#define SET_FORWARD 1
#define SET_TURN    2
#define DP_LIMIT    0.00001F

class UI : public BlueTooth{
public:
    UI();
    enum COMMAND {
        NOP,                // No Operation
        START,       		// リモートスタート
        LOGGER,             // ルックアップゲート
        REMOTE_MODE,        // ラジコン走行
        FINISH,             // 終了
    };
    int16_t getIntegerFromCUI();
    float getFloatFromCUI();
    void setState(uint8_t);
    COMMAND getState();
    // 以下、３つのメソッドは有効性が薄いのでリファクタリングで除外する
    void raiseDotFlag();
    void dropDotFlag();
    bool checkDotFlag();

private:
    COMMAND mState;
    bool mDotFlag;
};
#endif  // EV3_UNIT_UI_H_
