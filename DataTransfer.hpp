#pragma once

//‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹
//›                                                                         ﬁ
//› Module: Data Transfer Header File                                       ﬁ
//›                                                                         ﬁ
//› Description: Exchange data written in shared memory                     ﬁ
//›                                                                         ﬁ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
#include "InternalsPlugin.hpp"

struct TelemVect
{
	float x, y, z;

	void Set(const float a, const float b, const float c) { x = a; y = b; z = c; }
};

// One data set for each car
struct VehicleScoringInfo
{
	bool isPlayer;
	char driverName[32];
	char vehicleName[64];
	short totalLaps;			// laps completed
	signed char sector; 		// 0=sector3, 1=sector1, 2=sector2 (don't ask why)
	signed char finishStatus; 	// 0=none, 1=finished, 2=dnf, 3=dq
	double lapDist;
	double pathLateral; 		// lateral position with respect to *very approximate* "center" path
	double trackEdge;			// track edge (w.r.t. "center" path) on same side of track as vehicle
	double bestSector1;
	double bestSector2;
	double bestLapTime;
	double lastSector1;
	double lastSector2;
	double lastLapTime;
	double curSector1;
	double curSector2;
	short numPitstops;
	short numPenalties;
	bool inPits;
	unsigned char place;		// 1-based position
	double timeBehindNext;
	long lapsBehindNext;
	double timeBehindLeader;
	long lapsBehindLeader;
	double lapStartTime;
	double speed;				// calculated speed in m/s
	signed char control;		// who's in control: -1=nobody (shouldn't get this), 0=local player, 1=local AI, 2=remote, 3=replay (shouldn't get this)
	char vehicleClass[32];		// vehicle class

	// Position and derivatives
	TelemVect pos;				// world position in meters
	TelemVect localVel;		// velocity (meters/sec) in local vehicle coordinates
	TelemVect localAccel;		// acceleration (meters/sec^2) in local vehicle coordinates

	// Orientation and derivatives
	TelemVect orientation[3];	// rows of orientation matrix (use TelemQuat conversions if desired), also converts local
								// vehicle vectors into world X, Y, or Z using dot product of rows 0, 1, or 2 respectively
	TelemVect localRot;		// rotation (radians/sec) in local vehicle coordinates
	TelemVect localRotAccel;	// rotational acceleration (radians/sec^2) in local vehicle coordinates
	
	// Extension for rFactor 2
	long slotId;				// slot ID (note that it can be re-used in multiplayer after someone leaves)
	unsigned char headlights;
	unsigned char pitState; 	// 0=none, 1=request, 2=entering, 3=stopped, 4=exiting
	long qualification;			// 1-based, can be -1 when invalid
	double timeIntoLap;			// estimated time into lap
	double estimatedLapTime;	// estimated laptime used for 'time behind' and 'time into lap' (note: this may changed based on vehicle and setup!?)
	char pitGroup[24];			// pit group (same as team name unless pit is shared)
	unsigned char primaryFlag;	// currently only 0=green or 6=blue
	unsigned char upgradePack[16];  // Coded upgrades

};

// Part of player telemetry
struct WheelInfo
{
	double tireLoad;  			// Newton
	double gripFract;			// an approximation of what fraction of the contact patch is sliding
	double pressure;  			// kPa
	double temperature[3];		// Kelvin, left/center/right (not inside/center/outside!)
	double wear;				// wear (0.0-1.0, fraction of maximum) ... this is not necessarily proportional with grip loss
	bool flat;
	bool detached;
	double brakeTemp;			// Celsius
	double rotation;			// radians/sec
	double suspensionDeflection; // meters
	double rideHeight;			// meters
	double lateralForce;		// Newtons
	
	// Extension for rFactor 2
	double suspensionForce;		// pushrod load in Newtons
	double brakePressure;		// currently 0.0-1.0, depending on driver input and brake balance; will convert to true brake pressure (kPa) in future
	double lateralPatchVel;       // lateral velocity at contact patch
	double longitudinalPatchVel;  // longitudinal velocity at contact patch
	double lateralGroundVel;      // lateral velocity at contact patch
	double longitudinalGroundVel; // longitudinal velocity at contact patch
	double camber;                // radians (positive is left for left-side wheels, right for right-side wheels)
	double longitudinalForce;     // Newtons
	double verticalTireDeflection;// how much is tire deflected from its (speed-sensitive) radius
	double wheelYLocation;        // wheel's y location relative to vehicle y location
	double toe;                   // current toe angle w.r.t. the vehicle
	double tireCarcassTemperature;       // rough average of temperature samples from carcass (Kelvin)
	double tireInnerLayerTemperature[3]; // rough average of temperature samples from innermost layer of rubber (before carcass) (Kelvin)

};

struct TelemetryInfo
{
	long lapNumber;
	double lapStartTime;
	char vehicleName[64];
	char trackName[64];
	long gear;
	double engineRPM;
	double engineWaterTemp;
	double engineOilTemp;
	double clutchRPM;
	WheelInfo wheel[4]; // front left, front right, rear left, rear right
	double fuel;
	double engineMaxRPM; // rev limit
	unsigned char scheduledPitstops;
	bool overheating;
	bool detached;               // whether any parts (besides wheels) have been detached
	unsigned char dentSeverity[8]; // dent severity at 8 locations around the car (0=none, 1=some, 2=more)
	double lastImpactTime;
	double lastImpactMagnitude;
	TelemVect lastImpactPos;

	// Position and derivatives
	TelemVect pos;               // world position in meters
	TelemVect localVel;          // velocity (meters/sec) in local vehicle coordinates
	TelemVect localAccel;        // acceleration (meters/sec^2) in local vehicle coordinates

	// Orientation and derivatives
	TelemVect orientation[3];            // rows of orientation matrix (use TelemQuat conversions if desired), also converts local
                                 // vehicle vectors into world X, Y, or Z using dot product of rows 0, 1, or 2 respectively
	TelemVect localRot;          // rotation (radians/sec) in local vehicle coordinates
	TelemVect localRotAccel;     // rotational acceleration (radians/sec^2) in local vehicle coordinates

	  // Driver input
	double unfilteredThrottle;    // ranges  0.0-1.0
	double unfilteredBrake;       // ranges  0.0-1.0
	double unfilteredSteering;    // ranges -1.0-1.0 (left to right)
	double unfilteredClutch;      // ranges  0.0-1.0

	// Additional data provided by rFactor 2
	
	// Filtered input (various adjustments for rev or speed limiting, TC, ABS?, speed sensitive steering, clutch work for semi-automatic shifting, etc.)	
	double filteredThrottle;
	double filteredBrake;
	double filteredSteering;
	double filteredClutch;

	double front3rdDeflection;    // deflection at front 3rd spring
	double rear3rdDeflection;     // deflection at rear 3rd spring

	// Aerodynamics
	double frontWingHeight;       // front wing height
	double frontRideHeight;       // front ride height
	double rearRideHeight;        // rear ride height
	double drag;                  // drag
	double frontDownforce;        // front downforce
	double rearDownforce;         // rear downforce

	double engineTorque;          // current engine torque (including additive torque)
	long currentSector;           // the current sector (zero-based) with the pitlane stored in the sign bit (example: entering pits from third sector gives 0x80000002)	
	bool headlights;
	unsigned char speedLimiter;   // whether speed limiter is on
	unsigned char maxGears;       // maximum forward gears
	unsigned char frontTireCompoundIndex;   // index within brand
	unsigned char rearTireCompoundIndex;    // index within brand
	double fuelCapacity;          // capacity in liters
	unsigned char frontFlapActivated;       // whether front flap is activated
	unsigned char rearFlapActivated;        // whether rear flap is activated
	unsigned char rearFlapLegalStatus;      // 0=disallowed, 1=criteria detected but not allowed quite yet, 2=allowed
	unsigned char ignitionStarter;          // 0=off 1=ignition 2=ignition+starter
	char frontTireCompoundName[18];         // name of front tire compound
	char rearTireCompoundName[18];          // name of rear tire compound
	unsigned char speedLimiterAvailable;    // whether speed limiter is available
	unsigned char antiStallActivated;       // whether (hard) anti-stall is activated
	double rearBrakeBias;                   // fraction of brakes on rear
	double turboBoostPressure;              // current turbo boost pressure if available
};

struct EventInfo
{
	// Event info
	char trackName[64]; //char[64]
	long session; // 0=testday 1-4=practice 5-8=qual 9=warmup 10-13=race
	double currentTime;
	double endTime;
	long maxLaps;
	double lapDist;					// current distance around track
	long numVehicles;
	unsigned char gamePhase;
	signed char yellowFlagState;
	signed char sectorFlag[3];
	unsigned char startLight;       // start light frame (number depends on track)
	unsigned char numRedLights;     // number of red lights in start sequence
	bool inRealtime; // driving vs. monitor
	char playerName[32];

	double darkCloud;
	double raining;
	double ambientTemp;
	double trackTemp;
	TelemVect wind;
	double onPathWetness;
	double offPathWetness;
};

static TCHAR transferBufferName[] = TEXT("\rf2transferMappingObject");
static int transferBufferSize = 64000;


// Written to shared memory each time rFactor updates values
struct UnifiedRfData
{
	long rFactorVersion;		// 1 or 2
	bool telemetryValid;		// telemetry data received at least once
	bool scoringValid;			// scoring data received at least once
	bool driving;				// on track
	bool inSession;				// session active
	EventInfo event;			// event and environment
	TelemetryInfo telemetry;	// detailed player info
	VehicleScoringInfo scoring[100];	// all about opponents
};

