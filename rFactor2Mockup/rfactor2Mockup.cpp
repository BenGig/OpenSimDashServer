// Fake rFactor 2 data provider. Writes shared memory as rF2 would, for testing

#include "stdafx.h"
UnifiedRfData data;

TelemVect wind = { 0.1,0.4,0.2 } , velocity = { 12.1, 14.2,0.4 };

WheelInfo fl = { 520.0, 0.01, 25.0, 340, 345, 350, 0.1, false, false, 467.9, 5.4, 0.08, 0.43, 783.2 };
WheelInfo fr = { 510.0, 0.01, 25.0, 341, 346, 351, 0.1, false, false, 467.9, 5.4, 0.08, 0.43, 783.2 };
WheelInfo rl = { 520.0, 0.01, 25.0, 340, 345, 350, 0.1, false, false, 467.9, 5.4, 0.08, 0.43, 783.2 };
WheelInfo rr = { 510.0, 0.01, 25.0, 341, 346, 351, 0.1, false, false, 467.9, 5.4, 0.08, 0.43, 783.2 };

VehicleScoringInfo vh;


int main()
{
	data.scoringValid = true;
	data.telemetryValid = true;
	data.driving = true;
	data.inSession = true;
	data.rFactorVersion = 2;

	// event data
	data.event.ambientTemp = 310.0;
	data.event.currentTime = 13.0;
	data.event.darkCloud = 0.2;
	data.event.endTime = 20.0;
	data.event.gamePhase = 5;
	data.event.inRealtime = true;
	data.event.lapDist = 4294;
	data.event.maxLaps = 43;
	data.event.numRedLights = 5;
	data.event.numVehicles = 10;
	data.event.offPathWetness = 0.1;
	data.event.onPathWetness = 0.0;
	strcpy_s(data.event.playerName, "Max Throttle");
	data.event.raining = false;
	memcpy(data.event.sectorFlag, "abc", 3);
	data.event.session = 1;
	data.event.startLight = 2;
	strcpy_s(data.event.trackName, "Klausenpass");
	data.event.trackTemp = 302;
	data.event.wind = wind;
	data.event.yellowFlagState = 1;

	// telemetry data
	data.telemetry.antiStallActivated = false;
	data.telemetry.clutchRPM = 8000.0;
	data.telemetry.currentSector = 1;
	data.telemetry.dentSeverity[0] = 1;
	for (int i = 1; i < 8; i++) { data.telemetry.dentSeverity[i] = 0; }
	data.telemetry.detached = true;
	data.telemetry.drag = 0.342;
	data.telemetry.engineMaxRPM = 10000.0;
	data.telemetry.engineOilTemp = 350;
	data.telemetry.engineRPM = 7500.0;
	data.telemetry.engineTorque = 320.0;
	data.telemetry.engineWaterTemp = 370.0;
	data.telemetry.filteredBrake = 0.743;
	data.telemetry.filteredClutch = 0.1;
	data.telemetry.filteredSteering = 0.01;
	data.telemetry.filteredThrottle = 0.96;
	data.telemetry.unfilteredBrake = 0.743;
	data.telemetry.unfilteredClutch = 0.1;
	data.telemetry.unfilteredSteering = 0.01;
	data.telemetry.unfilteredThrottle = 0.96;
	data.telemetry.front3rdDeflection = 0.5;
	data.telemetry.frontDownforce = 79.24;
	data.telemetry.frontFlapActivated = false;
	data.telemetry.frontRideHeight = 0.059;
	data.telemetry.frontTireCompoundIndex = 0;
	strcpy_s(data.telemetry.frontTireCompoundName, "Slick");
	data.telemetry.frontWingHeight = 11.0;
	data.telemetry.fuel = 18.6;
	data.telemetry.fuelCapacity = 60.0;
	data.telemetry.gear = 3;
	data.telemetry.headlights = true;
	data.telemetry.ignitionStarter = 1;
	data.telemetry.lapNumber = 3;
	data.telemetry.lapStartTime = 10;
	data.telemetry.lastImpactMagnitude = 3.2;
	data.telemetry.lastImpactPos = wind; // lazy...
	data.telemetry.localAccel = wind;
	data.telemetry.localRot = wind;
	data.telemetry.localRotAccel = wind;
	data.telemetry.localVel = velocity;
	data.telemetry.maxGears = 6;
	// data.telemetry.orientation
	data.telemetry.overheating = false;
	//data.telemetry.pos
	data.telemetry.rear3rdDeflection = 0.4;
	data.telemetry.rearBrakeBias = 0.48;
	data.telemetry.rearDownforce = 120.9;
	data.telemetry.rearFlapActivated = false;
	data.telemetry.rearFlapLegalStatus = true;
	data.telemetry.rearRideHeight = 0.073;
	data.telemetry.rearTireCompoundIndex = 0;
	strcpy_s(data.telemetry.rearTireCompoundName, "Slick");
	data.telemetry.scheduledPitstops = 2;
	data.telemetry.speedLimiter = false;
	data.telemetry.speedLimiterAvailable = true;
	strcpy_s(data.telemetry.trackName, "Klausenpass");
	data.telemetry.turboBoostPressure = 4.2;
	strcpy_s(data.telemetry.vehicleName, "2CV Turbo");
	data.telemetry.wheel[0] = fl;
	data.telemetry.wheel[0] = fr;
	data.telemetry.wheel[0] = rl;
	data.telemetry.wheel[0] = rr;
	
	for (int i = 0; i < 10; i++)
	{
		data.scoring[i].bestLapTime = 87.532 - (i*.3);
		data.scoring[i].bestSector1 = 20.135 - (i*.1);
		data.scoring[i].bestSector2 = 35.253 - (i*.1);
		data.scoring[i].control = 1;
		data.scoring[i].curSector1 = 20.485 - (i*.1);
		data.scoring[i].curSector2 = 35.399 - (i*.1);
		data.scoring[i].estimatedLapTime = 87.4;
		data.scoring[i].finishStatus = 0;
		data.scoring[i].headlights = 1;
		data.scoring[i].inPits = 0;
		data.scoring[i].isPlayer = false;
		data.scoring[i].lapDist = 300 + (i * 39);
		data.scoring[i].lapsBehindLeader = 0;
		data.scoring[i].lapsBehindNext = 0;
		data.scoring[i].lapStartTime = 10.2;
		data.scoring[i].lastLapTime = 89.243;
		data.scoring[i].lastSector1 = 21.34;
		data.scoring[i].lastSector2 = 36.1;
		data.scoring[i].localVel = velocity;
		data.scoring[i].numPenalties = i;
		data.scoring[i].numPitstops = i;
		data.scoring[i].place = 10-i;
		data.scoring[i].qualification = i;
		data.scoring[i].sector = 1;
		data.scoring[i].speed = 140 + i;
		data.scoring[i].timeBehindLeader = (9 - i)*0.1;
		data.scoring[i].timeBehindNext = 0.1;
		data.scoring[i].totalLaps = 20 - i;
		strcpy_s(data.scoring[i].vehicleName, "2CV Turbo");
	}
	strcpy_s(data.scoring[0].driverName, "Peter Out");
	strcpy_s(data.scoring[1].driverName, "Desmond Dash");
	strcpy_s(data.scoring[2].driverName, "Slim Chance");
	strcpy_s(data.scoring[3].driverName, "Miles Behind");
	strcpy_s(data.scoring[4].driverName, "Sid Spoiler");
	strcpy_s(data.scoring[5].driverName, "Wilma Cargo");
	strcpy_s(data.scoring[6].driverName, "Dan Dipstick");
	strcpy_s(data.scoring[7].driverName, "Max Throttle");
	strcpy_s(data.scoring[8].driverName, "Percy Veer");
	strcpy_s(data.scoring[9].driverName, "Gloria Slap");

	data.scoring[4].inPits = 1;
	data.scoring[9].lapsBehindLeader = 1;
	data.scoring[9].lapsBehindNext = 1;
	data.scoring[0].isPlayer = true;


	HANDLE memMapFile;
	LPCTSTR pBuf;

	memMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		transferBufferSize,                // maximum object size (low-order DWORD)
		transferBufferName);                 // name of mapping object

	if (memMapFile == NULL)
	{
		std::cout << "Could not open memory mapping file";
		return 1;
	}
	// Log("Memory mapping file opened");

	pBuf = (LPTSTR)MapViewOfFile(memMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		transferBufferSize);

	if (pBuf == NULL)
	{
		std::cout << "Could not map view of file.\n";

		CloseHandle(memMapFile);
		return 1;
	}

	std::cout << "Memory mapping ready.\n";

	try
	{
		for (;;)
		{
			CopyMemory((PVOID)pBuf, &data, sizeof(data));

			Sleep(10);

			if (data.telemetry.engineRPM < data.telemetry.engineMaxRPM)
				data.telemetry.engineRPM += 100;
			else
				data.telemetry.engineRPM = 1500;
		}
	}
	catch ( ... )
	{
		UnmapViewOfFile(pBuf);
		CloseHandle(memMapFile);
	}

	// int answer;
	// std::cin >> answer;

	return 0;
}

