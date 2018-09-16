#ifndef EV3_UNIT_MEASUREDISTANCE_H_
#define EV3_UNIT_MEASUREDISTANCE_H_
#include "ev3api.h"
// #include "SensorDriver.h"
#include "SonarSensor.h"

class MeasureDistance
{
public:
	MeasureDistance(const ev3api::SonarSensor sonarSensor);
	bool DetectGate();
	bool ThroughtGate();
	void ThroughInit();
private:

	bool alert;
	bool FindGate;
	
	bool ThroughGate;	// 2018_0907
	uint32_t counter;
	int32_t distance;
	uint32_t AlertCounter;
	uint32_t ThroughtCounter;
//	const int GateDetectDistance = 25;
//	const int GateConfirmDistance = 10;	// �Q�[�g�m�苗�� 2018_0904

	const int GateDetectDistance = 35;	// ��̒l���Ƌ߂���������
	const int GateConfirmDistance = 20;	// �Q�[�g�m�苗�� 2018_0904

	const int GateThroughtDistance = 60;
//	const int GateThroughtDistance = 30;
	ev3api::SonarSensor mSonarSensor;

};
#endif  // MEASUREDISTANCE
