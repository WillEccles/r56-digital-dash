#include "obdwrapper.h"

/* External Variables */

COBD obd;
SupportedCodes_s SupportedCodes;

//static char VIN[18] = "";

bool DDGetDashData(DashData_s& data_out) {
	return	DDGetBoostPSI(data_out.boost_pressure) &&
			DDGetCoolantTempC(data_out.coolant_temp) &&
			//DDGetOilTempC(data_out.oil_temp) &&
			DDGetEngineRPM(data_out.rpm) &&
			DDGetVehicleSpeed(data_out.speed) &&
			DDGetFuelLevel(data_out.fuel);
}

bool DDInitOBD() {
	if (!obd.begin()) return false;

	obd.setBaudRate(115200UL);

	if (!obd.init()) return false;

	// get the VIN for use later (this is better than reading it more than once)
	//obd.getVIN(VIN, 18);
//	if (VIN[17]) { // VINs are 17 characters, and the 18th should be the null terminator
		// not sure that the getVIN function does this, it seems it might not
//		VIN[17] = '\0';
//	}

	// get the supported codes
	SupportedCodes.oil_temp = obd.isValidPID(PID_ENGINE_OIL_TEMP);
	SupportedCodes.turbo_rpm = obd.isValidPID(PID_TURBO_RPM);
	SupportedCodes.turbo_temp = obd.isValidPID(PID_TURBO_A_TEMP);

	return true;
}

bool DDGetBoostKPA(float& pressure_out) {
	int manifold;

	if (!obd.readPID(PID_INTAKE_MAP, manifold)) return false;

	pressure_out = (float)manifold;

	return true;
}

bool DDGetBoostPSI(float& pressure_out) {
	bool status = DDGetBoostKPA(pressure_out);

	pressure_out *= 0.14503774f;

	return true;
}

bool DDGetCoolantTempC(int16_t& temp_out) {
	if (!obd.readPID(PID_COOLANT_TEMP, temp_out)) return false;

	return true;
}

bool DDGetOilTempC(int16_t& temp_out) {
	if (!obd.readPID(PID_ENGINE_OIL_TEMP, temp_out)) return false;

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
	fuel_out = (float)flevel;

	return true;
}

void DDGetVoltage(float& volts_out) {
	volts_out = obd.getVoltage();
}

uint8_t DDKMHtoMPH(uint8_t kmh) {
	return (uint8_t)((float)kmh * 0.62137119f);
}
