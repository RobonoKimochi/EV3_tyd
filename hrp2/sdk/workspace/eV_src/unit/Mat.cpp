#include "Mat.h"
#include "math.h"

float Mat::Sin(float x){
    long ix, subix, sign, tval;

    ix = (int)(x * (I_PI / PI));   //単位変換
    sign = ix & I_PI;              //第3,第4象限である
    ix &= (I_PI - 1);              //第1,第2象限に限定
    if(ix > I_HPI) ix = I_PI - ix; //第1象限に限定

    subix = ix & (SUBINDEX - 1);   //線形補完に用いるサブインデックス
    ix >>= SUBBIT;                 //テーブル番号に変換

    //線形補完
    tval = ((long)sin_table[ix]   * (SUBINDEX - subix)
          + (long)sin_table[ix+1] * subix);

    return (sign ? -tval : tval) / (SUBINDEX * 65535.f);
}
float Mat::Cos(float x){
    return Sin(x+PI/2.);
}
float Mat::Dist(float x1,float y1,float x2, float y2){
    return ( sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
}
