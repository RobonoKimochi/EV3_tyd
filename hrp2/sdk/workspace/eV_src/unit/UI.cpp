#include "UI.h"

/**
 * コンストラクタ
 */
UI::UI():
    mState(NOP),
    mDotFlag(false)
{
    OpenSerialPort();
    if( isConnected() == true){
//        putString("@ BlueTooth is connected!\n\r");
    } else{
        // mBlueTooth->putString("conect dekite naiyo\n\r");
    }
}
void UI::setState(uint8_t state){
    switch(state){
    case 's':
        mState = START;
        break;
    case 'l':
        mState = LOGGER;
        break;
    case 'r':
        mState = REMOTE_MODE;
        break;
    case 'f':
        mState = FINISH;
        break;
    default:
        mState = NOP;
        break;
    }
}
UI::COMMAND UI::getState(){
    return mState;
}
void UI::raiseDotFlag(){
    mDotFlag = true;
}
void UI::dropDotFlag(){
    mDotFlag = false;
}
bool UI::checkDotFlag(){
    return mDotFlag;
}
float UI::getFloatFromCUI(){
    uint8_t c = 0;
    float fnum = 0;
    float dp = 0.1;                 // dp = decimal point (小数点)
    while(1){
        c = getChar();
        if(c == '\r'){              // Enterキー押下
            break;
        }
        if(c == '.' && checkDotFlag() == false){              // ドット押下
            raiseDotFlag();
            dp = 0.1;
            continue;
        }
        if(c < '0' || c > '9'){     // 数字0-9,Enter以外
            putString("\n\r");      // 改行
            fnum = 0;                // num をクリア
            dropDotFlag();
            dp = 0.1;
            continue;
        }
        if( checkDotFlag() == false ){
            fnum = fnum * 10 + c - '0';
        } else if(dp > DP_LIMIT){
            fnum = fnum + (c - '0') * dp;
            dp /= 10;
        } else{
            putString("\b \b");      // BackSpace
        }
    }
    dropDotFlag();
    return (fnum);
}
int16_t UI::getIntegerFromCUI(){
    uint16_t c = 0;
    uint16_t num = 0;
    bool mflag = false;             // マイナスフラグ
    while(1){
        c = getChar();
        if(c == '\r'){              // Enterキー押下
            break;
        }
        if(c== '-' && num == 0 && mflag == false){  // この条件では、0入力後のマイナス(ex. 0-25etc...)を除外できない
            mflag = true;
            continue;
        }
        if(c < '0' || c > '9'){     // 数字0-9以外
            putString("\n\r");      // 改行
            num = 0;                // num をクリア
            mflag = false;          // マイナスフラグをクリア
            continue;
        }
        num = num * 10 + ( c - '0');
        if( num > 255 || num < -255){
            putString("\n\rBeyond the limit\n\r");  // 範囲外
        }
    }
    if(mflag == false){
        return (num);
    } else{
        return ( -1 * num);
    }
}
