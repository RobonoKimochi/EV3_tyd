#include "BlueTooth.h"
/**
 * コンストラクタ
 */
 BlueTooth::BlueTooth()
    //  :  mBT(fp)
        {
            // FILE *mBT;
            OpenSerialPort();
 }
 /**
  * エンコーダ値の現在値をoffsetとして取得する
  * @param mRightEncOffset 右車輪のエンコーダ値
  * @param mLeftEncOffset 左車輪のエンコーダ値
  */
 bool BlueTooth::isConnected(){
  if(ev3_bluetooth_is_connected()){
      return true;
  } else {
      return false;
  }
}
  /**
   * エンコーダ値の現在値をoffsetとして取得する
   * @param mRightEncOffset 右車輪のエンコーダ値
   * @param mLeftEncOffset 左車輪のエンコーダ値
   */
void BlueTooth::OpenSerialPort(){
   mBT = ev3_serial_open_file(EV3_SERIAL_BT);
}
  /**
   * 右車輪のエンコーダ値を取得する
   */
uint8_t BlueTooth::getChar(){
  uint8_t c = fgetc(mBT);
//  fputc(c, mBT); /* エコーバック */
  return c;
}
uint8_t BlueTooth::getNumber(){
  uint8_t c = 0;
  while(c < '0' || c > '9'){
      c = fgetc(mBT);
  }
  // fputc(c, mBT); /* エコーバック */
  return (c - '0');
}
void BlueTooth::putChar(char& c){
  fputc(c,mBT);
}
void BlueTooth::putString(const char *s){
  fputs(s,mBT);
}
uint8_t BlueTooth::getValue(){
    int num = 0;
    scanf("%3d",&num);
    return num;
}
