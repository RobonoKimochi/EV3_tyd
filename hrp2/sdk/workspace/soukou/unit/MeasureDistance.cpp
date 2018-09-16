#include "MeasureDistance.h"


#define GATE_DETECT_OK_COUNT	(8)		// ゲート接近確定時間
#define THROUGH_DETECT_COUNT	(6000 / 40)	// 障害物非検知6secでゲート通過確定

/**
 * コンストラクタ
 */
 MeasureDistance::MeasureDistance(const ev3api::SonarSensor sonarSensor)
    //  :  mBT(fp)
:alert(false),
FindGate(false),
ThroughGate(false),
counter(0),
distance(0),
AlertCounter(0),
ThroughtCounter(0),
mSonarSensor(sonarSensor)
        {
 }




bool MeasureDistance::DetectGate(){

	if (++counter == 40/4) /* 約40msec周期毎に障害物検知  */
    	{
        	/*
         	* 超音波センサによる距離測定周期は、超音波の減衰特性に依存します。
         	* NXTの場合は、40msec周期程度が経験上の最短測定周期です。
         	* EV3の場合は、要確認
         	*/
        	distance = mSonarSensor.getDistance();
        	if ((distance <= GateDetectDistance) && (distance >= 0))
        	{
            	alert = true; /* 障害物を検知 */
        		if(AlertCounter < 20) {
        			AlertCounter++;
        		}
        	}
        	else
        	{
            	alert = false; /* 障害物無し */
        		if(AlertCounter > 1) {
        			AlertCounter--;
        		}
        	}
        	counter = 0;
    	}
    	
#if 0
	if(AlertCounter > 10) {
		FindGate = true;
	} else {
		FindGate = false;
	}
#else

	// 接近して、かつ距離が近づいたのを1度でも検知したら確定
	if( (AlertCounter >= GATE_DETECT_OK_COUNT) && (distance >= 0) && (distance < GateConfirmDistance) )
    {
		FindGate = true;
	} else {
		FindGate = false;
	}
#endif

	return (FindGate);
}

bool MeasureDistance::ThroughtGate(){

	if (++counter == 40/4) /* 約40msec周期毎に障害物検知  */
    	{
        	/*
         	* 超音波センサによる距離測定周期は、超音波の減衰特性に依存します。
         	* NXTの場合は、40msec周期程度が経験上の最短測定周期です。
         	* EV3の場合は、要確認
         	*/
        	distance = mSonarSensor.getDistance();
        	if ((distance <= GateThroughtDistance) && (distance >= 0))
        	{
            	alert = true; /* 障害物を検知 */
        		if(ThroughtCounter < 20) {
        			ThroughtCounter++;
        		}
        	}
        	else
        	{
            	alert = false; /* 障害物無し */
        		if(ThroughtCounter > 1) {
        			ThroughtCounter--;
        		}
        	}
        	counter = 0;
    	}
	if(ThroughtCounter > 10) {
		FindGate = true;
	} else {
		FindGate = false;
	}
	return (FindGate);
}

void MeasureDistance::ThroughInit()
{
	counter = 0;
	ThroughtCounter = 0;		// カウンタをリセット
   	alert = false; /* 障害物無し */
	ThroughGate = false;
	
}
