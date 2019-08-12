#include "obdwrapper.h"

/* External Variables */

COBD obd;

//static char VIN[18] = "";

static int barom = 0;

bool DDGetDashData(DashData_s& data_out) {
	return	DDGetBoostPSI(data_out.boost_pressure) &&
			DDGetCoolantTempC(data_out.coolant_temp) &&
			//DDGetOilTempC(data_out.oil_temp) &&
			DDGetEngineRPM(data_out.rpm) &&
			DDGetVehicleSpeed(data_out.speed) &&
			DDGetFuelLevel(data_out.fuel);// &&
			//DDGetTurboRPM(data_out.turbo_rpm) &&
			//DDGetTurboTempC(data_out.turbo_temp);
}

bool DDInitOBD() {
	if (!obd.begin()) return false;

	obd.setBaudRate(115200UL);

	if (!obd.init()) return false;

	return true;
}

bool DDGetBoostKPA(float& pressure_out) {
	int manifold;
	int barom;

	if (!obd.readPID(PID_INTAKE_MAP, manifold)) return false;
	if (!obd.readPID(PID_BAROMETRIC, barom)) return false;

	pressure_out = (float)manifold - (float)barom;

	return true;
}

bool DDGetBoostPSI(float& pressure_out) {
	int manifold;
	int barom;

	if (!obd.readPID(PID_INTAKE_MAP, manifold)) return false;
	if (!obd.readPID(PID_BAROMETRIC, barom)) return false;

	pressure_out = KPA_PSI_TABLE[manifold] - KPA_PSI_TABLE[barom];

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

	fuel_out = PERCENT_TABLE[flevel];

	return true;
}

void DDGetVoltage(float& volts_out) {
	volts_out = obd.getVoltage();
}

uint8_t DDKMHtoMPH(uint8_t kmh) {
	return (uint8_t)((float)kmh * 0.62137119f);
}

bool DDGetTurboRPM(uint32_t& rpm_out) {
	int r;

	if (!obd.readPID(PID_TURBO_RPM, r)) return false;

	rpm_out = (uint32_t)r;

	return true;
}

bool DDGetTurboTempC(uint16_t& temp_out) {
	int r;

	if (!obd.readPID(PID_TURBO_A_TEMP, r)) return false;

	temp_out = (uint16_t)r;

	return true;
}
