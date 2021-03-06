#include "stdafx.h"

SimData::SimData()
{
	elementRegistry = &sdem;
}

void SimData::reset()
{
	sdem.reset();
}

void SimData::markOwnCar()
{
	for (int i = 0; i < session.numCars.lint; i++)
	{
		if (scoring[i].isPlayer.bl) {
			ownCar->registerDriver();
			return;
		}
	}

}

int compareScoring(const void * par1, const void * par2)
{
	// Sort array of pointers to Driver objects. Sort routine hands over
	// pointers to array elements: pointer to pointer 
	Driver ** sc1 = (Driver **)par1; Driver ** sc2 = (Driver **)par2;
	Driver * s1 = *sc1; Driver * s2 = *sc2;
	if (s1->place.lint > s2->place.lint)
		return 1;
	else
		return -1;
}

void SimData::sortScoring()
{
	std::qsort(&ranking, session.numCars.lint, sizeof(Driver*), compareScoring);
}

// Show time or laps, if lapped
std::wstring formatGap(double gap, long myLaps, long otherLaps)
{
	if (abs(myLaps - otherLaps) <= 1)
		return timeToString(gap, true, false);

	std::wstring str = std::to_wstring(abs(myLaps - otherLaps));
	str.append(L" Lap");
	if (abs(myLaps - otherLaps) > 1)
		str.append(L"s");
	return str;
}

// generally available values calculated based on raw values
void SimData::deriveValues()
{
	bool lapLimited = false;
	// may not be ready when launching
	if (session.numCars.lint <= 1)
		return;
	int i = 0;
	while (!ranking[i]->isPlayer.bl)
		i += 1; // our driver record

	// Fuel estimation
	if (event.sessionString.str == L"Race")
	{
		if (event.numberOfLaps.lint < 2147483647)
			lapLimited = true;
		// detect refueling
		if (telemetry.fuelLoaded.flt < telemetry.fuel.flt)
		{
			telemetry.fuelLoaded.flt = telemetry.fuel.flt;
			if (lapLimited)
				telemetry.fuelLoadedAt.flt = ownCar->lapNumber.lint;
			else
				telemetry.fuelLoadedAt.flt = event.currentTime.flt;
		}
		if (lapLimited)
			if (ownCar->lapNumber.lint - telemetry.fuelLoadedAt.flt != 0)
				telemetry.fuelNeeded.flt = (telemetry.fuelLoaded.flt - telemetry.fuel.flt) / (ownCar->lapNumber.lint - telemetry.fuelLoadedAt.flt)*(event.numberOfLaps.lint - ownCar->lapNumber.lint);
		else
			if (event.currentTime.flt - telemetry.fuelLoadedAt.flt != 0)
				telemetry.fuelNeeded.flt = (telemetry.fuelLoaded.flt - telemetry.fuel.flt) / (event.currentTime.flt - telemetry.fuelLoadedAt.flt)*(event.endTime.flt - event.currentTime.flt);
	}


	// Gaps to previous and next. Compose complete string (time or lap, lap or laps)
	if (i == 0)
	{
		// we are first
		ownCar->gapPrevious.str = L"-";
		if (event.sessionString.str == L"Race")
			ownCar->gapNext.str = formatGap(ranking[i + 1]->timeBehindNext.flt, ownCar->lapNumber.lint, ranking[i + 1]->lapNumber.lint);
		else
			ownCar->gapNext.str = timeToString(abs(ownCar->bestTime.flt - ranking[i + 1]->bestTime.flt), true, false);
		return;
	}
	if (i == session.numCars.lint - 1)
	{
		// we are last
		if (event.sessionString.str == L"Race")
			ownCar->gapPrevious.str = formatGap(ownCar->timeBehindNext.flt, ranking[i - 1]->lapNumber.lint, ownCar->lapNumber.lint);
		else
			ownCar->gapPrevious.str = timeToString(abs(ownCar->bestTime.flt - ranking[i - 1]->bestTime.flt), true, false);
		ownCar->gapNext.str = L"-";
		return;
	}
	if (event.sessionString.str == L"Race")
	{
		ownCar->gapPrevious.str = formatGap(ownCar->timeBehindNext.flt, ranking[i - 1]->lapNumber.lint, ownCar->lapNumber.lint);
		ownCar->gapNext.str = formatGap(ranking[i + 1]->timeBehindNext.flt, ownCar->lapNumber.lint, ranking[i + 1]->lapNumber.lint);
	}
	else
	{
		ownCar->gapPrevious.str = timeToString(abs(ownCar->bestTime.flt - ranking[i - 1]->bestTime.flt), true, false);
		ownCar->gapNext.str = timeToString(abs(ownCar->bestTime.flt - ranking[i + 1]->bestTime.flt), true, false);
	}
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
	raceevent->append(event.durationLeft.json()); raceevent->append(L",");
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
		score->append(scoring[i].formerTime.json()); score->append(L",");
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

Event::Event()
{
	simName.registerMe();
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
	durationLeft.registerMe();
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

void Driver::registerDriver()
{
	vehicleName.registerMe();
	driverName.registerMe();
	place.registerMe();
	finishStatus.registerMe();
	finishStatusString.registerMe();
	lastTime.registerMe();
	bestTime.registerMe();
	formerTime.registerMe();
	split.registerMe();
	lapNumber.registerMe();
	lapStartTime.registerMe();
	currentTime.registerMe();
	bestSector1.registerMe();
	bestSector2.registerMe();
	lastSector1.registerMe();
	lastSector2.registerMe();
	curSector1.registerMe();
	curSector2.registerMe();
	timeBehindNext.registerMe();
	lapsBehindNext.registerMe();
	gapPrevious.registerMe();
	gapNext.registerMe();
	timeBehindLeader.registerMe();
	lapsBehindLeader.registerMe();
	numPitstops.registerMe();
	numPenalties.registerMe();
	control.registerMe();
	vehicleClass.registerMe();
	inPits.registerMe();
	pitState.registerMe();
	isPlayer.registerMe();
	currentSector.registerMe();
	position.registerMe();
	velocity.registerMe();
	accel.registerMe();
	headlights.registerMe();
	qualification.registerMe();
	primaryFlag.registerMe();
}

Car::Car()
{	
	wheels[0].setPosition(L"FL");
	wheels[1].setPosition(L"FR");
	wheels[2].setPosition(L"RL");
	wheels[3].setPosition(L"RR");

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
	overheating.registerMe();
	oilTemp.registerMe();
	waterTemp.registerMe();
	fuelLoaded.registerMe();
	fuelLoadedAt.registerMe();
	fuelNeeded.registerMe();
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
