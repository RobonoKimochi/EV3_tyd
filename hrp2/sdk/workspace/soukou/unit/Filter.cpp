#include "Filter.h"

#define MA_NUM 10     // 移動平均サンプル数

float Filter::MovingAverage(float in){
    static float x[MA_NUM];
    static bool isFilled = false;
    float out;
    unsigned char i;
    if( isFilled == false){             // 初期化時に配列を初期値で埋める
        for(i = 0; i < MA_NUM; i++){
            x[i] = in;
        }
        isFilled = true;
    }
    for(i = 0; i < MA_NUM - 1; i++){
        x[i] = x[i+1];
    }
    x[MA_NUM - 1] = in;

    out = 0;
    for(i = 0; i < MA_NUM; i++){
        out += x[i];
    }
    out = out / MA_NUM;

    return (out);
}
#define k 0.02F     // フィルタ係数 exp(-2πfdt)->(f=100Hz,dt=0.004sec)
// カットオフ周波数を引数で切り分ける？
float Filter::LowPassFilter(float in){
    // static float old_out = in;
    float out;

    out = ( k * in + (1 - k) * old_out );

    old_out = out;

    return (out);
}
