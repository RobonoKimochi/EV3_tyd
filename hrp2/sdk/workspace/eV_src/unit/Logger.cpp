#include "Logger.h"
#include "string.h"

/**
 * コンストラクタ
 */
Logger::Logger()
    // : mLabel_num(0)
{
    mClock = new Clock();
    OpenSerialPort();
    if( isConnected() == true){
        // putString("BlueTooth is connected!\n\r");
    } else{
        // mBlueTooth->putString("conect dekite naiyo\n\r");
    }
}
void Logger::init(){
    resetClock();
    // sprintf(mBuffer,"%s %s %s %s\n\r",mLabel[0],mLabel[1],mLabel[2],mLabel[3]);
    // putString(mBuffer);
}
//
// void Logger::setLog(const char& label, unsigned char num);
//     if( mLabel_num <= 0 || mLabel_num > MAX_LABEL_NUM){
//         putString("Over MAX_LABEL_NUM!!\n\r")
//         // LENGTHの警告も必要
//     }
//     if( mLabel_num < MAX_LABEL_NUM ){
//         strcpy(mLabel[mLabel_num], label)
//         mLabel_num++;
//     }
//
// }
void Logger::sendLog(float data[], unsigned char num){
    unsigned char i;
    float time = (float)mClock->now() / 1000.0f;

    sprintf(mBuffer,"%07.3f", time);

    for(i = 0; i < num; i++){
    	int buf = strlen(mBuffer);
    	sprintf(&mBuffer[buf],";%07.3f", data[i]);

    }

    int buf = strlen(mBuffer);
    sprintf(&mBuffer[buf],";%s", "\n\r");	// 末尾に改行コードを加える

    putString(mBuffer);
}

void Logger::resetClock(){
    mClock->reset();
}
