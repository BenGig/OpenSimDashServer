#pragma once

#include "stdafx.h"

#define MAX_GRID 50
#define MAX_WHEEL_PARAMS 30

class Event
{
public:
	Event();

	SimDataBool raceOver = SimDataBool(L"raceOver", false);
	SimDataString trackName = SimDataString(L"trackName", L""); 
	SimDataString driverName = SimDataString(L"driverName", L"");
	SimDataInteger numberOfLaps = SimDataInteger(L"numberOfLaps", 0);
	SimDataInteger session = SimDataInteger(L"session", 0);
	SimDataString sessionString = SimDataString(L"sessionString", L"");
	SimDataTime currentTime = SimDataTime(L"currentTime", 0);
	SimDataTime timeLeft = SimDataTime(L"timeLeft", 0);
	SimDataInteger sectorCount = SimDataInteger(L"sectorCount", 0); // AC only
	SimDataTime endTime = SimDataTime(L"endTime", 0);
	SimDataInteger gamePhase = SimDataInteger(L"gamePhase", 0);

	// rFactor
	SimDataInteger numRedLights = SimDataInteger(L"numRedLights", 0);
	SimDataFloating darkCloud = SimDataFloating(L"darkCloud", 0);
	SimDataFloating raining = SimDataFloating(L"raining", 0);
	SimDataFloating ambientTemp = SimDataFloating(L"ambientTemp", 0);
	SimDataFloating trackTemp = SimDataFloating(L"trackTemp", 0);
	SimDataVector wind = SimDataVector(L"wind");
	SimDataFloating onPathWetness = SimDataFloating(L"onPathWetness", 0);
	SimDataFloating offPathWetness = SimDataFloating(L"offPathWetness", 0);
	SimDataBool inRealtime = SimDataBool(L"inRealTime", false);

	SimDataFloating lapDist = SimDataFloating(L"lapDist", 0);
};

class Session
{
public:
	Session();

	SimDataInteger numCars = SimDataInteger(L"numCars", 0);
	// rFactor
	SimDataInteger yellowFlagState = SimDataInteger(L"yellowFlagState", 0);
	SimDataInteger sectorFlag1 = SimDataInteger(L"sectorFlag1", 0);
	SimDataInteger sectorFlag2 = SimDataInteger(L"sectorFlag2", 0);
	SimDataInteger sectorFlag3 = SimDataInteger(L"sectorFlag3", 0);
	SimDataInteger startLight = SimDataInteger(L"startLight", 0);
};

class Driver
{
public:
	SimDataString vehicleName = SimDataString(L"vehicleName", L"");
	SimDataString driverName = SimDataString(L"driverName", L"");
	SimDataInteger place = SimDataInteger(L"place", 0);
	SimDataInteger finishStatus = SimDataInteger(L"finishStatus", 0); 	// 0=none, 1=finished, 2=dnf, 3=dq
	SimDataString finishStatusString = SimDataString(L"finishStatusString", L"");
	SimDataTime lastTime = SimDataTime(L"lastTime", 0);
	SimDataTime bestTime = SimDataTime(L"bestTime", 0);
	SimDataInteger lapNumber = SimDataInteger(L"lapNumber", 0);
	SimDataTime lapStartTime = SimDataTime(L"lapStartTime", 0);
	SimDataTime currentTime = SimDataTime(L"currentTime", 0);
	SimDataTime bestSector1 = SimDataTime(L"bestSector1", 0);
	SimDataTime bestSector2 = SimDataTime(L"bestSector2", 0);
	SimDataTime lastSector1 = SimDataTime(L"lastSector1", 0);
	SimDataTime lastSector2 = SimDataTime(L"lastSector2", 0);
	SimDataTime curSector1 = SimDataTime(L"curSector1", 0);
	SimDataTime curSector2 = SimDataTime(L"curSector2", 0);
	SimDataTime timeBehindNext = SimDataTime(L"timeBehindNext", 0);
	SimDataTime lapsBehindNext = SimDataTime(L"lapsBehindNext", 0);
	SimDataTime timeBehindLeader = SimDataTime(L"timeBehindLeader", 0);
	SimDataTime lapsBehindLeader = SimDataTime(L"lapsBehindLeader", 0);
	SimDataInteger numPitstops = SimDataInteger(L"numPitStops", 0);
	SimDataInteger numPenalties = SimDataInteger(L"numPenalties", 0);
	SimDataInteger control = SimDataInteger(L"control", 0);		// who's in control: -1=nobody (shouldn't get this), 0=local player, 1=local AI, 2=remote, 3=replay (shouldn't get this)
	SimDataString vehicleClass = SimDataString(L"vehicleClass", L"");		// vehicle class
	SimDataBool inPits = SimDataBool(L"inPits", true);
	SimDataInteger pitState = SimDataInteger(L"pitState", 0); 	// 0=none, 1=request, 2=entering, 3=stopped, 4=exiting
	SimDataBool isPlayer = SimDataBool(L"isPlayer", true);
	SimDataInteger currentSector = SimDataInteger(L"currentSector", 0);
	SimDataVector position = SimDataVector(L"position");
	SimDataVector velocity = SimDataVector(L"velocity");
	SimDataVector accel = SimDataVector(L"acceleration");
	SimDataInteger headlights = SimDataInteger(L"headlights", 0);
	SimDataInteger qualification = SimDataInteger(L"qualification", 0);	
	SimDataInteger primaryFlag = SimDataInteger(L"primaryFlag", 0);	// currently only 0=green or 6=blue
};

class Wheel
{
private:
	std::wstring position;
	int numElements = 0;
public:
	Wheel();
	void SetPosition(wchar_t * position);

	SimDataElement * elements[MAX_WHEEL_PARAMS];

	//SimDataString position = SimDataString(L"pos", L"");

	SimDataFloating tireLoad = SimDataFloating(L"tireLoad", 0);  			// Newton
	SimDataFloating gripFract = SimDataFloating(L"gripFract", 0); 			// an approximation of what fraction of the contact patch is sliding
	SimDataFloating tirePressure = SimDataFloating(L"tirePressure", 0);  		// kPa
	SimDataFloating tireTempInside = SimDataFloating(L"tireTempInside", 0);		// Kelvin
	SimDataFloating tireTempMiddle = SimDataFloating(L"tireTempMiddle", 0);		// Kelvin
	SimDataFloating tireTempOutside = SimDataFloating(L"tireTempOutside", 0);	// Kelvin
	SimDataFloating tireWear = SimDataFloating(L"tireWear", 0);				// wear (0.0-1.0, fraction of maximum) ... this is not necessarily proportional with grip loss
	SimDataFloating brakeTemp = SimDataFloating(L"brakeTemp", 0);			// Celsius
	SimDataFloating wheelRotation = SimDataFloating(L"wheelRotation", 0);			// radians/sec
	SimDataFloating suspensionDeflection = SimDataFloating(L"suspensionDeflection", 0); // meters
	SimDataFloating tempAvg = SimDataFloating(L"tempAvg", 0);				// rough average of temperature samples from carcass (Kelvin)

	SimDataFloating camber = SimDataFloating(L"camber", 0);								// AC, rF2
																						// rF2: radians (positive is left for left-side wheels, right for right-side wheels)

																						// AC only
	SimDataFloating dirtLevel = SimDataFloating(L"dirtLevel", 0);						// AC only
	SimDataFloating radius = SimDataFloating(L"radius", 0);								// AC only
	SimDataFloating suspensionMaxTravel = SimDataFloating(L"suspensionMaxTravel", 0);	// AC only

																						// rFactor
	SimDataFloating rideHeight = SimDataFloating(L"rideHeight", 0);			// meters
	SimDataFloating lateralForce = SimDataFloating(L"lateralForce", 0);		// Newtons
	SimDataBool flat = SimDataBool(L"flat", false);
	SimDataBool detached = SimDataBool(L"detached", false);

	// rF2
	SimDataFloating suspensionForce = SimDataFloating(L"suspensionForce", 0);		// pushrod load in Newtons
	SimDataFloating brakePressure = SimDataFloating(L"brakePressure", 0);		// currently 0.0-1.0, depending on driver input and brake balance; will convert to true brake pressure (kPa) in future
	SimDataFloating lateralPatchVel = SimDataFloating(L"lateralPatchVel", 0);       // lateral velocity at contact patch
	SimDataFloating longitudinalPatchVel = SimDataFloating(L"longitudinalPatchVel", 0);  // longitudinal velocity at contact patch
	SimDataFloating lateralGroundVel = SimDataFloating(L"lateralGroundVel", 0);      // lateral velocity at contact patch
	SimDataFloating longitudinalGroundVel = SimDataFloating(L"longitudinalGroundVel", 0); // longitudinal velocity at contact patch
	SimDataFloating longitudinalForce = SimDataFloating(L"longitudinalForce", 0);     // Newtons
	SimDataFloating verticalTireDeflection = SimDataFloating(L"verticalTireDeflection", 0);// how much is tire deflected from its (speed-sensitive) radius
	SimDataFloating wheelYLocation = SimDataFloating(L"wheelYLocation", 0);        // wheel's y location relative to vehicle y location
	SimDataFloating toe = SimDataFloating(L"toe", 0);                   // current toe angle w.r.t. the vehicle
	SimDataFloating tireInnerLayerInnerTemp = SimDataFloating(L"tireInnerLayerInnerTemp", 0); // rough average of temperature samples from innermost layer of rubber (before carcass) (Kelvin)
	SimDataFloating tireInnerLayerMidTemp = SimDataFloating(L"tireInnerLayerMidTemp", 0);
	SimDataFloating tireInnerLayerOuterTemp = SimDataFloating(L"tireInnerLayerOuterTemp", 0);

};


class Car
{
public:
	Wheel wheels[4];

	Car();

	SimDataFloating maxRPM = SimDataFloating(L"maxRPM", 0);
	SimDataFloating fuel = SimDataFloating(L"fuel", 0);
	SimDataInteger gear = SimDataInteger(L"gear", 0);
	SimDataFloating engineRPM = SimDataFloating(L"engineRPM", 0);
	SimDataFloating speed = SimDataFloating(L"speed", 0);

	SimDataFloating unfilteredThrottle = SimDataFloating(L"unfilteredThrottle", 0);
	SimDataFloating unfilteredBrake = SimDataFloating(L"unfilteredBrake", 0);
	SimDataFloating unfilteredClutch = SimDataFloating(L"unfilteredClutch", 0); 
	SimDataFloating unfilteredSteering = SimDataFloating(L"unfilteredSteering", 0);

	// AC
	SimDataFloating maxTorque = SimDataFloating(L"maxTorque", 0);
	SimDataFloating maxPower = SimDataFloating(L"maxPower", 0);
	SimDataFloating abs = SimDataFloating(L"abs", 0);
	SimDataFloating drs = SimDataFloating(L"drs", 0);
	SimDataFloating tc = SimDataFloating(L"tc", 0);
	SimDataFloating heading = SimDataFloating(L"heading", 0);
	SimDataFloating pitch = SimDataFloating(L"pitch", 0);
	SimDataFloating roll = SimDataFloating(L"roll", 0);
	SimDataFloating cgHeight = SimDataFloating(L"cgHeight", 0);
	//SimDataFloating carDamage[5];
	SimDataInteger numberOfTyresOut = SimDataInteger(L"numberOfTyresOut", 0);
	SimDataFloating distanceTraveled = SimDataFloating(L"distanceTraveled", 0);
	SimDataFloating normalizedCarPosition = SimDataFloating(L"normalizedCarPosition", 0);
	
	// rFactor
	SimDataTime timeBehindNext = SimDataTime(L"timeBehindNext", 0);
	SimDataInteger lapsBehindNext = SimDataInteger(L"lapsBehindNext", 0);
	SimDataTime timeBehindLeader = SimDataTime(L"timeBehindLeader", 0);
	SimDataInteger lapsBehindLeader = SimDataInteger(L"lapsBehindLeader", 0);

	// rF 2
	SimDataFloating maxFuel = SimDataFloating(L"maxFuel", 0);						// rF2, AC
	SimDataFloating filteredThrottle = SimDataFloating(L"filteredThrottle", 0);
	SimDataFloating filteredBrake = SimDataFloating(L"filteredBrake", 0);
	SimDataFloating filteredSteering = SimDataFloating(L"filteredSteering", 0);
	SimDataFloating filteredClutch = SimDataFloating(L"filteredClutch", 0);

	SimDataFloating front3rdDeflection = SimDataFloating(L"front3rdDeflection", 0);    // deflection at front 3rd spring
	SimDataFloating rear3rdDeflection = SimDataFloating(L"rear3rdDeflection", 0);     // deflection at rear 3rd spring

																					  // Aerodynamics
	SimDataFloating frontWingHeight = SimDataFloating(L"frontWingHeight", 0);       // front wing height
	SimDataFloating frontRideHeight = SimDataFloating(L"frontRideHeight", 0);       // front ride height
	SimDataFloating rearRideHeight = SimDataFloating(L"rearRideHeight", 0);        // rear ride height
	SimDataFloating drag = SimDataFloating(L"drag", 0);                  // drag
	SimDataFloating frontDownforce = SimDataFloating(L"frontDownforce", 0);        // front downforce
	SimDataFloating rearDownforce = SimDataFloating(L"rearDownforce", 0);         // rear downforce

	SimDataFloating engineTorque = SimDataFloating(L"tireInnerLayerOuterTemp", 0);          // current engine torque (including additive torque)
	SimDataBool headlights = SimDataBool(L"headlights", false);

	SimDataInteger speedLimiter = SimDataInteger(L"speedLimiter", 0);   // whether speed limiter is on
	SimDataInteger maxGears = SimDataInteger(L"maxGears", 0);       // maximum forward gears
	SimDataInteger frontTireCompoundIndex = SimDataInteger(L"frontTireCompoundIndex", 0);   // index within brand
	SimDataInteger rearTireCompoundIndex = SimDataInteger(L"rearTireCompoundIndex", 0);    // index within brand
	SimDataFloating fuelCapacity = SimDataFloating(L"fuelCapacity", 0);          // capacity in liters
	SimDataInteger frontFlapActivated = SimDataInteger(L"frontFlapActivated", 0);       // whether front flap is activated
	SimDataInteger rearFlapActivated = SimDataInteger(L"rearFlapActivated", 0);        // whether rear flap is activated
	SimDataInteger rearFlapLegalStatus = SimDataInteger(L"rearFlapLegalStatus", 0);      // 0=disallowed, 1=criteria detected but not allowed quite yet, 2=allowed
	SimDataInteger ignitionStarter = SimDataInteger(L"ignitionStarter", 0);          // 0=off 1=ignition 2=ignition+starter
	SimDataString frontTireCompoundName = SimDataString(L"frontTireCompoundName", L"");         // name of front tire compound
	SimDataString rearTireCompoundName = SimDataString(L"rearTireCompoundName", L"");          // name of rear tire compound
	SimDataInteger speedLimiterAvailable = SimDataInteger(L"speedLimiterAvailable", 0);    // whether speed limiter is available
	SimDataInteger antiStallActivated = SimDataInteger(L"antiStallActivated", 0);       // whether (hard) anti-stall is activated
	SimDataFloating rearBrakeBias = SimDataFloating(L"rearBrakeBias", 0);                   // fraction of brakes on rear
	SimDataFloating turboBoostPressure = SimDataFloating(L"turboBoostPressure", 0);              // current turbo boost pressure if available
	SimDataString flagShown = SimDataString(L"flagShown", L"");
};

class SimData {
public:
	Event event;
	Session session;
	Car telemetry;
	Driver scoring[MAX_GRID];
	Driver * ownCar;

	SimDataElementsManager * elementRegistry;

	void Reset();
	std::wstring * ScoringJson();
	std::wstring * EventJson(std::wstring * simName);
	std::wstring * PerformanceJson();

	SimData();
	~SimData() { Reset(); };
};

