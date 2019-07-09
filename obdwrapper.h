#ifndef DD_OBD_WRAPPER_H
#define DD_OBD_WRAPPER_H

#include <OBD2UART.h>

struct VehicleSpeed_s {
	uint8_t kmh;	// Vehicle speed in KMH [0, 255]
	uint8_t mph;	// Vehicle speed in MPH [0, 158]
};

// A structure designed to hold the most important data for the digital dash
struct DashData_s {
	float		boost_pressure;	// Boost pressure in PSI (converted from kPa)
	int16_t		coolant_temp;	// Coolant temperature in degress Celsius [-40, 215]
	int16_t		oil_temp;		// Oil temperature in degress Celsius [-40, 215]
	uint16_t	rpm;			// Engine rpm
	float		fuel;			// Fuel level [0.0, 100.0]
	float		voltage;		// Battery voltage
	char*		VIN;			// Vehicle Identification Number

	VehicleSpeed_s speed;		// Vehicle speed
};

// A structure that contains whether or not each OBD-II PID is supported.
struct SupportedCodes_s {
	bool intake_map;
	bool coolant_temp;
	bool oil_temp;
	bool rpm;
	bool fuel;
	bool voltage;
	bool speed;
};

// Defined elsewhere, but contains all (relevant) usable codes.
extern SupportedCodes_s SupportedCodes;

// This is here if it's necessary to use it, but you should avoid it.
// The wrapper manages this object by itself, it's only here so you can
// get custom PIDs if necessary.
extern COBD obd;

/* Initialize, returning version info and init status (true = good) */
bool DDInitOBD(byte& verison_out);

/* Get all the important data for the digital dash at once */
bool DDGetDashData(DashData_s& data_out);

/* Get boost pressure in PSI */
bool DDGetBoostPSI(float& pressure_out);

/* Get boost pressure in kPa */
bool DDGetBoostKPA(float& pressure_out);

/* Get coolant temperature in degrees Celsius (-40 to +215) */
bool DDGetCoolantTempC(int16_t& temp_out);

/* Get oil temperature in degrees Celsius (-40 to +215) */
bool DDGetOilTempC(int16_t& temp_out);

/* Get engine RPM */
bool DDGetEngineRPM(uint16_t& rpm_out);

/* Get vehicle speed (KMH: 0-255)*/
bool DDGetSpeedKMH(uint8_t& kph_out);

/* Get vehicle speed (MPH: 0-158) */
bool DDGetSpeedMPH(uint8_t& mph_out);

/* Get vehicle speed as both kmh and mph */
bool DDGetVehicleSpeed(VehicleSpeed_s& speed_out);

/* Get the fuel level remaining as a float from 0.0 to 100.0 */
bool DDGetFuelLevel(float& fuel_out);

/* Get current voltage */
void DDGetVoltage(float& volts_out);

/* Convert KMH to MPH (use this if you use DDGetSpeedKMH and want MPH as well) */
inline uint8_t DDKMHtoMPH(uint8_t kmh);

#endif // DD_OBD_WRAPPER_H
