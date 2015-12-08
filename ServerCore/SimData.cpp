#include "stdafx.h"

void SimData::reset()
{
	sdem.reset();
}

std::wstring * SimData::eventJson(std::wstring * simName)
{
	std::wstring * raceevent = new std::wstring(L"{\"simName\":\"");
	raceevent->append(*simName); raceevent->append(L"\",");
	raceevent->append(event.raceOver.json()); raceevent->append(L",");
	raceevent->append(event.trackName.json()); raceevent->append(L",");
	raceevent->append(event.numberOfLaps.json()); raceevent->append(L",");
	raceevent->append(event.session.json()); raceevent->append(L",");
	raceevent->append(event.sessionString.json()); raceevent->append(L",");
	raceevent->append(event.timeLeft.json()); raceevent->append(L",");
	raceevent->append(event.gamePhase.json()); raceevent->append(L",");
	raceevent->append(event.numRedLights.json()); raceevent->append(L",");
	raceevent->append(session.startLight.json()); raceevent->append(L",");
	raceevent->append(event.darkCloud.json()); raceevent->append(L",");
	raceevent->append(event.raining.json()); raceevent->append(L",");
	raceevent->append(event.ambientTemp.json()); raceevent->append(L",");
	raceevent->append(event.trackTemp.json()); raceevent->append(L",");
	raceevent->append(event.onPathWetness.json()); raceevent->append(L",");
	raceevent->append(event.offPathWetness.json()); raceevent->append(L",");
	raceevent->append(telemetry.flagShown.json()); raceevent->append(L",");
	raceevent->append(event.inRealtime.json()); raceevent->append(L",");
	raceevent->append(event.currentTime.json());
	raceevent->append(L"}");
	return raceevent;
}

std::wstring * SimData::performanceJson()
{
	std::wstring * perf = new std::wstring(L"[");
	//perf->append()
	return perf;
}

std::wstring * SimData::scoringJson()
{
	std::wstring * score = new std::wstring(L"[");
	for (int i = 0; i < session.numCars.lint; i++)
	{
		score->append(L"{");
		score->append(scoring[i].driverName.json()); score->append(L",");
		score->append(scoring[i].vehicleClass.json()); score->append(L",");
		score->append(scoring[i].vehicleName.json()); score->append(L",");
		score->append(scoring[i].place.json()); score->append(L",");
		score->append(scoring[i].finishStatus.json()); score->append(L",");
		score->append(scoring[i].finishStatusString.json()); score->append(L",");
		score->append(scoring[i].lastTime.json()); score->append(L",");
		score->append(scoring[i].bestTime.json()); score->append(L",");
		score->append(scoring[i].lapNumber.json()); score->append(L",");
		score->append(scoring[i].bestSector1.json()); score->append(L",");
		score->append(scoring[i].bestSector2.json()); score->append(L",");
		score->append(scoring[i].lastSector1.json()); score->append(L",");
		score->append(scoring[i].lastSector2.json()); score->append(L",");
		score->append(scoring[i].curSector1.json()); score->append(L",");
		score->append(scoring[i].curSector2.json()); score->append(L",");
		score->append(scoring[i].timeBehindNext.json()); score->append(L",");
		score->append(scoring[i].lapsBehindNext.json()); score->append(L",");
		score->append(scoring[i].timeBehindLeader.json()); score->append(L",");
		score->append(scoring[i].lapsBehindLeader.json()); score->append(L",");
		score->append(scoring[i].numPitstops.json()); score->append(L",");
		score->append(scoring[i].numPenalties.json()); score->append(L",");
		score->append(scoring[i].control.json()); score->append(L",");
		score->append(scoring[i].inPits.json()); score->append(L",");
		score->append(scoring[i].pitState.json()); score->append(L",");
		score->append(scoring[i].isPlayer.json()); score->append(L",");
		score->append(scoring[i].currentSector.json()); score->append(L",");
		score->append(scoring[i].position.json()); score->append(L",");
		score->append(scoring[i].velocity.json()); score->append(L",");
		score->append(scoring[i].accel.json()); score->append(L",");
		score->append(scoring[i].headlights.json()); score->append(L",");
		score->append(scoring[i].qualification.json()); score->append(L",");
		score->append(scoring[i].lapStartTime.json()); score->append(L",");
		score->append(scoring[i].currentTime.json());
		score->append(L"},");
	}
	score->pop_back();

	score->append(L"]");

	return score;
}

SimData::SimData()
{
	elementRegistry = &sdem;
}

Event::Event()
{
	trackName.registerMe();
	driverName.registerMe();
	numberOfLaps.registerMe();
	lapDist.registerMe();
	session.registerMe();
	sessionString.registerMe();
	timeLeft.registerMe();
	sectorCount.registerMe();
	endTime.registerMe();
	gamePhase.registerMe();
	numRedLights.registerMe();
	darkCloud.registerMe();
	raining.registerMe();
	ambientTemp.registerMe();
	trackTemp.registerMe();
	wind.registerMe();
	onPathWetness.registerMe();
	offPathWetness.registerMe();
	inRealtime.registerMe();
	currentTime.registerMe();
}

Session::Session()
{
	numCars.registerMe();
	yellowFlagState.registerMe();
	sectorFlag1.registerMe();
	sectorFlag2.registerMe();
	sectorFlag3.registerMe();
	startLight.registerMe();
}



Car::Car()
{	
	wheels[0].setPosition(L"FL");
	wheels[1].setPosition(L"FR");
	wheels[2].setPosition(L"RL");
	wheels[3].setPosition(L"RR");

	maxRPM.registerMe();
	fuel.registerMe();
	gear.registerMe();
	engineRPM.registerMe();
	speed.registerMe();
	unfilteredThrottle.registerMe();
	unfilteredBrake.registerMe();
	unfilteredClutch.registerMe();
	unfilteredSteering.registerMe();
	maxTorque.registerMe();
	maxPower.registerMe();
	abs.registerMe();
	drs.registerMe();
	tc.registerMe();
	heading.registerMe();
	pitch.registerMe();
	roll.registerMe();
	cgHeight.registerMe();
	numberOfTyresOut.registerMe();
	distanceTraveled.registerMe();
	normalizedCarPosition.registerMe();
	timeBehindNext.registerMe();
	lapsBehindNext.registerMe();
	timeBehindLeader.registerMe();
	lapsBehindLeader.registerMe();
	maxFuel.registerMe();
	filteredThrottle.registerMe();
	filteredBrake.registerMe();
	filteredSteering.registerMe();
	filteredClutch.registerMe();
	front3rdDeflection.registerMe();
	rear3rdDeflection.registerMe();
	frontWingHeight.registerMe();
	frontRideHeight.registerMe();
	rearRideHeight.registerMe();
	drag.registerMe();
	frontDownforce.registerMe();
	rearDownforce.registerMe();
	engineTorque.registerMe();
	headlights.registerMe();
	speedLimiter.registerMe();
	maxGears.registerMe();
	frontTireCompoundIndex.registerMe();
	rearTireCompoundIndex.registerMe();
	fuelCapacity.registerMe();
	frontFlapActivated.registerMe();
	rearFlapActivated.registerMe();
	rearFlapLegalStatus.registerMe();
	ignitionStarter.registerMe();
	frontTireCompoundName.registerMe();
	rearTireCompoundName.registerMe();
	speedLimiterAvailable.registerMe();
	antiStallActivated.registerMe();
	rearBrakeBias.registerMe();
	turboBoostPressure.registerMe();
}

Wheel::Wheel()
{
	elements[0] = &tireLoad;
	elements[1] = &gripFract;
	elements[2] = &tireTempInside;
	elements[3] = &tireTempMiddle;
	elements[4] = &tireTempOutside;
	elements[5] = &tireWear;
	elements[6] = &brakeTemp;
	elements[7] = &wheelRotation;
	elements[8] = &suspensionDeflection;
	elements[9] = &tempAvg;
	elements[10] = &camber;
	elements[11] = &dirtLevel;
	elements[12] = &radius;
	elements[13] = &suspensionMaxTravel;
	elements[14] = &rideHeight;
	elements[15] = &lateralForce;
	elements[16] = &flat;
	elements[17] = &detached;
	elements[18] = &suspensionForce;
	elements[19] = &brakePressure;
	elements[20] = &lateralPatchVel;
	elements[21] = &longitudinalPatchVel;
	elements[22] = &lateralGroundVel;
	elements[23] = &longitudinalGroundVel;
	elements[24] = &longitudinalForce;
	elements[25] = &verticalTireDeflection;
	elements[26] = &wheelYLocation;
	elements[27] = &toe;
	elements[28] = &tireInnerLayerInnerTemp;
	elements[29] = &tireInnerLayerMidTemp;
	elements[30] = &tireInnerLayerOuterTemp;
};

void Wheel::setPosition(wchar_t * p)
{
	std::wstring * pos = new std::wstring(p);
	pos->append(L"_");

	for (int i = 0; i < MAX_WHEEL_PARAMS + 1; i++)
	{
		if (elements[i] != NULL)
		{
			elements[i]->label.insert(0, *pos); elements[i]->registerMe();
		}
	}
}
