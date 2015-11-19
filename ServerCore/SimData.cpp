#include "stdafx.h"

void SimData::Reset()
{
	sdem.Reset();
}

std::wstring * SimData::EventJson()
{
	std::wstring * raceevent = new std::wstring(L"{");
	raceevent->append(event.trackName.Json()); raceevent->append(L",");
	raceevent->append(event.numberOfLaps.Json()); raceevent->append(L",");
	raceevent->append(event.session.Json()); raceevent->append(L",");
	raceevent->append(event.timeLeft.Json()); raceevent->append(L",");
	raceevent->append(event.gamePhase.Json()); raceevent->append(L",");
	raceevent->append(event.numRedLights.Json()); raceevent->append(L",");
	raceevent->append(session.startLight.Json()); raceevent->append(L",");
	raceevent->append(event.darkCloud.Json()); raceevent->append(L",");
	raceevent->append(event.raining.Json()); raceevent->append(L",");
	raceevent->append(event.ambientTemp.Json()); raceevent->append(L",");
	raceevent->append(event.trackTemp.Json()); raceevent->append(L",");
	raceevent->append(event.onPathWetness.Json()); raceevent->append(L",");
	raceevent->append(event.offPathWetness.Json());
	raceevent->append(L"}");
	return raceevent;
}

std::wstring * SimData::ScoringJson()
{
	std::wstring * score = new std::wstring(L"[");
	for (int i = 0; i < session.numCars.lint; i++)
	{
		score->append(L"{");
		score->append(scoring[i].driverName.Json()); score->append(L",");
		score->append(scoring[i].vehicleClass.Json()); score->append(L",");
		score->append(scoring[i].vehicleName.Json()); score->append(L",");
		score->append(scoring[i].place.Json()); score->append(L",");
		score->append(scoring[i].finishStatus.Json()); score->append(L",");
		score->append(scoring[i].lastTime.Json()); score->append(L",");
		score->append(scoring[i].bestTime.Json()); score->append(L",");
		score->append(scoring[i].lapNumber.Json()); score->append(L",");
		score->append(scoring[i].sector.Json()); score->append(L",");
		score->append(scoring[i].currentTime.Json()); score->append(L",");
		score->append(scoring[i].bestSector1.Json()); score->append(L",");
		score->append(scoring[i].bestSector2.Json()); score->append(L",");
		score->append(scoring[i].lastSector1.Json()); score->append(L",");
		score->append(scoring[i].lastSector2.Json()); score->append(L",");
		score->append(scoring[i].curSector1.Json()); score->append(L",");
		score->append(scoring[i].curSector2.Json()); score->append(L",");
		score->append(scoring[i].numPitstops.Json()); score->append(L",");
		score->append(scoring[i].numPenalties.Json()); score->append(L",");
		score->append(scoring[i].control.Json()); score->append(L",");
		score->append(scoring[i].inPits.Json()); score->append(L",");
		score->append(scoring[i].pitState.Json()); score->append(L",");
		score->append(scoring[i].isPlayer.Json()); score->append(L",");
		score->append(scoring[i].currentSector.Json()); score->append(L",");
		score->append(scoring[i].position.Json()); score->append(L",");
		score->append(scoring[i].velocity.Json()); score->append(L",");
		score->append(scoring[i].accel.Json()); score->append(L",");
		score->append(scoring[i].headlights.Json()); score->append(L",");
		score->append(scoring[i].qualification.Json()); score->append(L",");
		score->append(scoring[i].primaryFlag.Json());
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
	trackName.Register();
	driverName.Register();
	numberOfLaps.Register();
	session.Register();
	timeLeft.Register();
	sectorCount.Register();
	endTime.Register();
	gamePhase.Register();
	numRedLights.Register();
	darkCloud.Register();
	raining.Register();
	ambientTemp.Register();
	trackTemp.Register();
	wind.Register();
	onPathWetness.Register();
	offPathWetness.Register();
	inRealtime.Register();
}

Session::Session()
{
	numCars.Register();
	yellowFlagState.Register();
	sectorFlag1.Register();
	sectorFlag2.Register();
	sectorFlag3.Register();
	startLight.Register();
}



Car::Car()
{	
	wheels[0].SetPosition(L"FL");
	wheels[1].SetPosition(L"FR");
	wheels[2].SetPosition(L"RL");
	wheels[3].SetPosition(L"RR");

	maxRPM.Register();
	fuel.Register();
	gear.Register();
	engineRPM.Register();
	speed.Register();
	unfilteredThrottle.Register();
	unfilteredBrake.Register();
	unfilteredClutch.Register();
	unfilteredSteering.Register();
	maxTorque.Register();
	maxPower.Register();
	abs.Register();
	drs.Register();
	tc.Register();
	heading.Register();
	pitch.Register();
	roll.Register();
	cgHeight.Register();
	numberOfTyresOut.Register();
	distanceTraveled.Register();
	normalizedCarPosition.Register();
	lapStartTime.Register();
	timeBehindNext.Register();
	lapsBehindNext.Register();
	timeBehindLeader.Register();
	lapsBehindLeader.Register();
	maxFuel.Register();
	lapDist.Register();
	filteredThrottle.Register();
	filteredBrake.Register();
	filteredSteering.Register();
	filteredClutch.Register();
	front3rdDeflection.Register();
	rear3rdDeflection.Register();
	frontWingHeight.Register();
	frontRideHeight.Register();
	rearRideHeight.Register();
	drag.Register();
	frontDownforce.Register();
	rearDownforce.Register();
	engineTorque.Register();
	headlights.Register();
	speedLimiter.Register();
	maxGears.Register();
	frontTireCompoundIndex.Register();
	rearTireCompoundIndex.Register();
	fuelCapacity.Register();
	frontFlapActivated.Register();
	rearFlapActivated.Register();
	rearFlapLegalStatus.Register();
	ignitionStarter.Register();
	frontTireCompoundName.Register();
	rearTireCompoundName.Register();
	speedLimiterAvailable.Register();
	antiStallActivated.Register();
	rearBrakeBias.Register();
	turboBoostPressure.Register();
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

void Wheel::SetPosition(wchar_t * p)
{
	std::wstring * pos = new std::wstring(p);
	pos->append(L"_");

	for (int i = 0; i < MAX_WHEEL_PARAMS + 1; i++)
	{
		if (elements[i] != NULL)
		{
			elements[i]->label.insert(0, *pos); elements[i]->Register();
		}
	}
}
