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
private:

	bool alert;
	bool FindGate;
	uint32_t counter;
	int32_t distance;
	uint32_t AlertCounter;
	uint32_t ThroughtCounter;
	const int GateDetectDistance = 25;
	const int GateThroughtDistance = 60;
	ev3api::SonarSensor mSonarSensor;

};
#endif  // MEASUREDISTANCE
