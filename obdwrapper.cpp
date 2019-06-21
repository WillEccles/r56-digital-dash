#include "obdwrapper.h"

COBD obd;

bool DDGetDashData(DashData_s& data_out) {
	return	DDGetBoostPSI(data_out.boost_pressure) &&
			DDGetCoolantTempC(data_out.coolant_temp) &&
			DDGetEngineRPM(data_out.rpm) &&
			DDGetVehicleSpeed(data_out.speed);
}

bool DDInitOBD(byte& version_out) {
	version_out = obd.begin();

	while (!obd.init());

	if (!version_out) return false;
	return true;
}

bool DDGetBoostPSI(float& pressure_out) {
	int manifold;
	int barometer;

	if (!obd.readPID(PID_INTAKE_MAP, manifold)) return false;
	if (!obd.readPID(PID_BAROMETRIC, barometer)) return false;

	pressure_out = (float)(manifold - barometer);
	pressure_out *= 0.14503774f;

	return true;
}

bool DDGetCoolantTempC(int16_t& temp_out) {
	if (!obd.readPID(PID_COOLANT_TEMP, temp_out)) return false;

	return true;
}

bool DDGetEngineRPM(uint16_t& rpm_out) {
	int rpm;

	if (!obd.readPID(PID_RPM, rpm)) return false;

	rpm_out = rpm;

	return true;
}

bool DDGetSpeedKMH(uint8_t& kph_out) {
	int kmh;

	if (!obd.readPID(PID_SPEED, kmh)) return false;

	kph_out = kmh;

	return true;
}

bool DDGetSpeedMPH(uint8_t& mph_out) {
	uint8_t kmh;

	if (!DDGetSpeedKMH(kmh)) return false;

	mph_out = DDKMHtoMPH(kmh);

	return true;
}

bool DDGetVehicleSpeed(VehicleSpeed_s& speed_out) {
	if (!DDGetSpeedKMH(speed_out.kmh)) return false;

	speed_out.mph = DDKMHtoMPH(speed_out.kmh);

	return true;
}

bool DDGetFuelLevel(float& fuel_out) {
	int flevel;

	if (!obd.readPID(PID_FUEL_LEVEL, flevel)) return false;

	// returned value is an int [0, 100]
	// this is a change that I made in my fork of ArduinoOBD
	// PR is open as of 2019-06-20; highly doubt it will be merged
	// i only want a value in [0.0, 1.0]
	fuel_out = (float)flevel / 100.0f;

	return true;
}

void DDGetVoltage(float& volts_out) {
	volts_out = obd.getVoltage();
}

uint8_t DDKMHtoMPH(uint8_t kmh) {
	return (uint8_t)((float)kmh * 0.62137119f);
}
