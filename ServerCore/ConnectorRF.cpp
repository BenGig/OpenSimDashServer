#include "stdafx.h"

ConnectorRF::ConnectorRF()
{
	rfVersion = 0;

	/*
	RF_SESSION_TYPE[0] = RF_SESSION_TYPE_TESTDAY;
	RF_SESSION_TYPE[1] = RF_SESSION_TYPE_PRACTICE;
	RF_SESSION_TYPE[2] = RF_SESSION_TYPE_PRACTICE;
	RF_SESSION_TYPE[3] = RF_SESSION_TYPE_PRACTICE;
	RF_SESSION_TYPE[4] = RF_SESSION_TYPE_PRACTICE;
	RF_SESSION_TYPE[5] = RF_SESSION_TYPE_QUALIFICATION;
	RF_SESSION_TYPE[6] = RF_SESSION_TYPE_QUALIFICATION;
	RF_SESSION_TYPE[7] = RF_SESSION_TYPE_QUALIFICATION;
	RF_SESSION_TYPE[8] = RF_SESSION_TYPE_QUALIFICATION;
	RF_SESSION_TYPE[9] = RF_SESSION_TYPE_WARMUP;
	RF_SESSION_TYPE[10] = RF_SESSION_TYPE_RACE;
	RF_SESSION_TYPE[11] = RF_SESSION_TYPE_RACE;
	RF_SESSION_TYPE[12] = RF_SESSION_TYPE_RACE;
	RF_SESSION_TYPE[13] = RF_SESSION_TYPE_RACE;
	*/
}

bool ConnectorRF::connect()
{
	if (td.connect())
	{
		td.read();
		rfVersion = td.data.rFactorVersion;
		if (rfVersion > 0)
		{
			sd.event.simName.str = std::wstring(L"rFactor " + std::to_wstring(rfVersion));
			read();
			sd.markOwnCar();
			return true;
		}
	}
	return false;
}

int ConnectorRF::check()
{
	if (td.connect())
	{
		td.read();
		int rfVer = td.data.rFactorVersion;
		td.disconnect();
		sd.reset();
		return rfVer;
	}
	else
		return 0;
}

std::wstring * ConnectorRF::simName()
{
	return new std::wstring(L"rFactor " + std::to_wstring(rfVersion));
	// might not be ready in time (written in receiver thread)
	// return &sd.event.simName.str; 
}
void ConnectorRF::sessionToStr() {
	if (rfVersion == 1) {
		if (sd.event.session.lint == 0)
		{
			sd.event.sessionString.str = std::wstring(L"Testday");
		}
		else if (sd.event.session.lint <= 4) {
			sd.event.sessionString.str = std::wstring(L"Practice");
		}
		else if (sd.event.session.lint <= 5) {
			sd.event.sessionString.str = std::wstring(L"Qualify");
		}
		else if (sd.event.session.lint <= 6) {
			sd.event.sessionString.str = std::wstring(L"Warmup");
		}
		else
			sd.event.sessionString.str = std::wstring(L"Race");
	}
	else {
	 if (sd.event.session.lint == 0)
	 {
		 sd.event.sessionString.str = std::wstring(L"Testday");
	 }
	 else if (sd.event.session.lint <= 4) {
		 sd.event.sessionString.str = std::wstring(L"Practice");
	 }
	 else if (sd.event.session.lint <= 8) {
		 sd.event.sessionString.str = std::wstring(L"Qualify");
	 }
	 else if (sd.event.session.lint <= 9) {
		 sd.event.sessionString.str = std::wstring(L"Warmup");
	 }
	 else
		 sd.event.sessionString.str = std::wstring(L"Race");
	}
}

bool ConnectorRF::read()
{
	if (td.read())
	{
		if (slowReady())
		{
			// event info
			rfVersion = td.data.rFactorVersion; 
			convertFrom8bit(td.data.event.trackName, &sd.event.trackName.str);
			convertFrom8bit(td.data.event.playerName, &sd.event.driverName.str);
			sd.event.numberOfLaps.lint = td.data.event.maxLaps;
			sd.event.session.lint = td.data.event.session;
			sessionToStr();
			if (sd.event.gamePhase.lint == 0)
				sd.event.timeLeft.flt = -1;
			else
				sd.event.timeLeft.flt = td.data.event.endTime - td.data.event.currentTime;
			sd.event.numRedLights.lint = td.data.event.numRedLights;
			sd.event.endTime.flt = td.data.event.endTime;
			sd.event.gamePhase.lint = td.data.event.gamePhase;
			if (sd.event.gamePhase.lint == 8)
				sd.event.raceOver.bl = true;
			sd.event.darkCloud.flt = td.data.event.darkCloud;
			sd.event.raining.flt = td.data.event.raining;
			sd.event.ambientTemp.flt = td.data.event.ambientTemp;
			sd.event.trackTemp.flt = td.data.event.trackTemp;
			sd.event.wind.x = td.data.event.wind.x;
			sd.event.wind.y = td.data.event.wind.y;
			sd.event.wind.z = td.data.event.wind.z;
			sd.event.onPathWetness.flt = td.data.event.onPathWetness;
			sd.event.offPathWetness.flt = td.data.event.offPathWetness;
			sd.event.inRealtime.bl = td.data.event.inRealtime;
			sd.event.lapDist.flt = td.data.event.lapDist;
			sd.event.currentTime.flt = td.data.event.currentTime;

			sd.telemetry.maxGears.lint = td.data.telemetry.maxGears;
			convertFrom8bit(td.data.telemetry.frontTireCompoundName, &sd.telemetry.frontTireCompoundName.str);
			convertFrom8bit(td.data.telemetry.rearTireCompoundName, &sd.telemetry.rearTireCompoundName.str);

			// our scoring data may be not yet valid during sim startup
			if (sd.ownCar != NULL)
			{
				// time or lap limited?
				sd.event.durationLeft.str = L"";
				if (sd.event.numberOfLaps.lint < 2147483647)
				{	// lap limited
					sd.event.durationLeft.str.append(L"Laps: ");
					sd.event.durationLeft.str.append(std::to_wstring(sd.event.numberOfLaps.lint - sd.ownCar->lapNumber.lint));
				}
				if (sd.event.timeLeft.flt > -2147483647)
				{	// time limited
					if (sd.event.durationLeft.str.length() > 0)
						sd.event.durationLeft.str.append(L" ");
					sd.event.durationLeft.str.append(L"Time: ");
					sd.event.durationLeft.str.append(timeToString(sd.event.timeLeft.flt, false, true));
				}

				// calculate current lap and sector split times
				sd.ownCar->currentTime.flt = sd.event.currentTime.flt - sd.ownCar->lapStartTime.flt;

				switch (sd.ownCar->currentSector.lint)
				{
				case 1:
					if (sd.ownCar->bestTime.flt != 0 && sd.ownCar->formerTime.flt != 0)
						sd.ownCar->split.flt = sd.ownCar->bestTime.flt - sd.ownCar->formerTime.flt;
					break;
				case 2:
					if (sd.ownCar->bestSector1.flt != 0 && sd.ownCar->lastSector1.flt != 0)
						sd.ownCar->split.flt = sd.ownCar->bestSector1.flt - sd.ownCar->lastSector1.flt;
					break;
				case 3:
					if (sd.ownCar->bestSector2.flt != 0 && sd.ownCar->lastSector2.flt != 0)
						sd.ownCar->split.flt = sd.ownCar->bestSector2.flt - sd.ownCar->lastSector2.flt;
				}
				sd.ownCar->split.str->assign(std::to_wstring(sd.ownCar->split.flt).c_str());
				sd.ownCar->split.str->erase(sd.ownCar->split.str->find_first_of(L".", 0) + 4, sd.ownCar->split.str->length());
				
			}
			slowFetched();
		}
		
		if (quickReady())
		{
			// RF_SESSION info
			sd.session.numCars.lint = td.data.event.numVehicles;
			sd.session.yellowFlagState.lint = td.data.event.yellowFlagState;
			sd.session.sectorFlag1.lint = td.data.event.sectorFlag[0];
			sd.session.sectorFlag2.lint = td.data.event.sectorFlag[1];
			sd.session.sectorFlag3.lint = td.data.event.sectorFlag[2];
			sd.session.startLight.lint = td.data.event.startLight;

			// scoring
			for (int i = 0; i < sd.session.numCars.lint; i++)
			{
				convertFrom8bit(td.data.scoring[i].vehicleName, &sd.scoring[i].vehicleName.str);
				convertFrom8bit(td.data.scoring[i].driverName, &sd.scoring[i].driverName.str);

				sd.scoring[i].place.lint = td.data.scoring[i].place;
				sd.scoring[i].finishStatus.lint = td.data.scoring[i].finishStatus;
				switch (td.data.scoring[i].finishStatus)
				{
				case 0:
					sd.scoring[i].finishStatusString.str = std::wstring(L"");
					break;
				case 1:
					sd.scoring[i].finishStatusString.str = std::wstring(L"F");
					break;
				case 2:
					sd.scoring[i].finishStatusString.str = std::wstring(L"DNF");
					break;
				case 3:
					sd.scoring[i].finishStatusString.str = std::wstring(L"DQ");
					break;
				default:
					break;
				}
				// save last lap time when starting line crossed
				if (sd.scoring[i].lastTime.flt != td.data.scoring[i].lastLapTime)
				{
					sd.scoring[i].formerTime.flt = sd.scoring[i].lastTime.flt;
				}
				sd.scoring[i].lastTime.flt = td.data.scoring[i].lastLapTime;
				sd.scoring[i].bestTime.flt = td.data.scoring[i].bestLapTime;
				sd.scoring[i].lapNumber.lint = td.data.scoring[i].totalLaps;
				sd.scoring[i].lapStartTime.flt = td.data.scoring[i].lapStartTime;
				sd.scoring[i].currentTime.flt = td.data.event.currentTime - td.data.scoring[i].lapStartTime;
				sd.scoring[i].bestSector1.flt = td.data.scoring[i].bestSector1;
				sd.scoring[i].bestSector2.flt = td.data.scoring[i].bestSector2;
				sd.scoring[i].lastSector1.flt = td.data.scoring[i].lastSector1;
				sd.scoring[i].lastSector2.flt = td.data.scoring[i].lastSector2;
				sd.scoring[i].curSector1.flt = td.data.scoring[i].curSector1;
				sd.scoring[i].curSector2.flt = td.data.scoring[i].curSector2;
				sd.scoring[i].timeBehindNext.flt = td.data.scoring[i].timeBehindNext;
				sd.scoring[i].lapsBehindNext.lint = td.data.scoring[i].lapsBehindNext;
				sd.scoring[i].timeBehindLeader.flt = td.data.scoring[i].timeBehindLeader;
				sd.scoring[i].lapsBehindLeader.lint = td.data.scoring[i].lapsBehindLeader;
				sd.scoring[i].numPitstops.lint = td.data.scoring[i].numPitstops;
				sd.scoring[i].numPenalties.lint = td.data.scoring[i].numPenalties;
				sd.scoring[i].control.lint = td.data.scoring[i].control;
				convertFrom8bit(td.data.scoring[i].vehicleClass, &sd.scoring[i].vehicleClass.str);
				sd.scoring[i].inPits.bl = td.data.scoring[i].inPits;
				sd.scoring[i].isPlayer.bl = td.data.scoring[i].isPlayer;
				if (sd.scoring[i].isPlayer.bl)
					sd.ownCar = &sd.scoring[i];

				sd.scoring[i].pitState.lint = td.data.scoring[i].pitState;

				if (td.data.scoring[i].sector == 0)
					sd.scoring[i].currentSector.lint = 3;
				else
					sd.scoring[i].currentSector.lint = td.data.scoring[i].sector;

				sd.scoring[i].position.x = td.data.scoring[i].pos.x;
				sd.scoring[i].position.y = td.data.scoring[i].pos.y;
				sd.scoring[i].position.z = td.data.scoring[i].pos.z;

				sd.scoring[i].headlights.lint = td.data.scoring[i].headlights;
				sd.scoring[i].qualification.lint = td.data.scoring[i].qualification;
				sd.scoring[i].primaryFlag.lint = td.data.scoring[i].primaryFlag;

				// Pointer to driver object for sorting
				sd.ranking[i] = &sd.scoring[i];
			}
			// sort scoring according to standings
			sd.sortScoring();

			// telemetry
			for (int i = 0; i < 4; i++)
			{
				sd.telemetry.wheels[i].brakePressure.flt = td.data.telemetry.wheel[i].brakePressure;
				if (td.data.telemetry.wheel[i].brakeTemp <= 20)
					sd.telemetry.wheels[i].brakeTemp.flt = 20; // may be Kelvin or zero if in pits
				else
					sd.telemetry.wheels[i].brakeTemp.flt = td.data.telemetry.wheel[i].brakeTemp - 273.16;
				sd.telemetry.wheels[i].camber.flt = td.data.telemetry.wheel[i].camber;
				sd.telemetry.wheels[i].detached.bl = td.data.telemetry.wheel[i].detached;
				sd.telemetry.wheels[i].flat.bl = td.data.telemetry.wheel[i].flat;
				sd.telemetry.wheels[i].gripFract.flt = td.data.telemetry.wheel[i].gripFract;
				sd.telemetry.wheels[i].lateralForce.flt = td.data.telemetry.wheel[i].lateralForce;
				sd.telemetry.wheels[i].lateralGroundVel.flt = td.data.telemetry.wheel[i].lateralGroundVel;
				sd.telemetry.wheels[i].lateralPatchVel.flt = td.data.telemetry.wheel[i].lateralPatchVel;
				sd.telemetry.wheels[i].longitudinalForce.flt = td.data.telemetry.wheel[i].longitudinalForce;
				sd.telemetry.wheels[i].longitudinalGroundVel.flt = td.data.telemetry.wheel[i].longitudinalGroundVel;
				sd.telemetry.wheels[i].longitudinalPatchVel.flt = td.data.telemetry.wheel[i].longitudinalPatchVel;
				sd.telemetry.wheels[i].rideHeight.flt = td.data.telemetry.wheel[i].rideHeight;
				sd.telemetry.wheels[i].suspensionDeflection.flt = td.data.telemetry.wheel[i].suspensionDeflection;
				sd.telemetry.wheels[i].suspensionForce.flt = td.data.telemetry.wheel[i].suspensionForce;
				sd.telemetry.wheels[i].tireLoad.flt = td.data.telemetry.wheel[i].tireLoad;
				sd.telemetry.wheels[i].tirePressure.flt = td.data.telemetry.wheel[i].pressure;
				sd.telemetry.wheels[i].tireWear.flt = td.data.telemetry.wheel[i].wear;
				sd.telemetry.wheels[i].wheelRotation.flt = td.data.telemetry.wheel[i].rotation;
				sd.telemetry.wheels[i].tempAvg.flt = td.data.telemetry.wheel[i].tireCarcassTemperature - 273.16;

				if (i == 1 || i == 3)
				{
					sd.telemetry.wheels[i].tireInnerLayerInnerTemp.flt = td.data.telemetry.wheel[i].tireInnerLayerTemperature[0] - 273.16;
					sd.telemetry.wheels[i].tireInnerLayerMidTemp.flt = td.data.telemetry.wheel[i].tireInnerLayerTemperature[1] - 273.16;
					sd.telemetry.wheels[i].tireInnerLayerOuterTemp.flt = td.data.telemetry.wheel[i].tireInnerLayerTemperature[2] - 273.16;
					sd.telemetry.wheels[i].tireTempInside.flt = td.data.telemetry.wheel[i].temperature[0] - 273.16;
					sd.telemetry.wheels[i].tireTempMiddle.flt = td.data.telemetry.wheel[i].temperature[1] - 273.16;
					sd.telemetry.wheels[i].tireTempOutside.flt = td.data.telemetry.wheel[i].temperature[2] - 273.16;
				}
				else
				{
					sd.telemetry.wheels[i].tireInnerLayerInnerTemp.flt = td.data.telemetry.wheel[i].tireInnerLayerTemperature[2] - 273.16;
					sd.telemetry.wheels[i].tireInnerLayerMidTemp.flt = td.data.telemetry.wheel[i].tireInnerLayerTemperature[1] - 273.16;
					sd.telemetry.wheels[i].tireInnerLayerOuterTemp.flt = td.data.telemetry.wheel[i].tireInnerLayerTemperature[0] - 273.16;
					sd.telemetry.wheels[i].tireTempInside.flt = td.data.telemetry.wheel[i].temperature[2] - 273.16;
					sd.telemetry.wheels[i].tireTempMiddle.flt = td.data.telemetry.wheel[i].temperature[1] - 273.16;
					sd.telemetry.wheels[i].tireTempOutside.flt = td.data.telemetry.wheel[i].temperature[0] - 273.16;
				}
			}
			sd.telemetry.fuel.flt = td.data.telemetry.fuel;
			if (rfVersion == 2)
				sd.telemetry.fuel.max = td.data.telemetry.fuelCapacity;

			if (td.data.telemetry.gear < 0)
				sd.telemetry.gear.str = L"R";
			else if (td.data.telemetry.gear == 0)
				sd.telemetry.gear.str = L"N";
			else
				sd.telemetry.gear.str = std::to_wstring(td.data.telemetry.gear);

			sd.telemetry.engineRPM.flt = td.data.telemetry.engineRPM;
			sd.telemetry.engineRPM.max = td.data.telemetry.engineMaxRPM;
			sd.telemetry.speed.flt = 3.6 * sqrt(pow(td.data.telemetry.localVel.x, 2) + pow(td.data.telemetry.localVel.y, 2) + pow(td.data.telemetry.localVel.z, 2));
			sd.telemetry.overheating.bl = td.data.telemetry.overheating;
			sd.telemetry.waterTemp.flt = td.data.telemetry.engineWaterTemp;
			sd.telemetry.oilTemp.flt = td.data.telemetry.engineOilTemp;

			sd.telemetry.unfilteredThrottle.flt = td.data.telemetry.unfilteredThrottle;
			sd.telemetry.unfilteredBrake.flt = td.data.telemetry.unfilteredBrake;
			sd.telemetry.unfilteredSteering.flt = td.data.telemetry.unfilteredSteering;
			sd.telemetry.unfilteredClutch.flt = td.data.telemetry.unfilteredClutch;

			sd.telemetry.filteredThrottle.flt = td.data.telemetry.filteredThrottle;
			sd.telemetry.filteredBrake.flt = td.data.telemetry.filteredBrake;
			sd.telemetry.filteredSteering.flt = td.data.telemetry.filteredSteering;
			sd.telemetry.filteredClutch.flt = td.data.telemetry.filteredClutch;

			sd.telemetry.front3rdDeflection.flt = td.data.telemetry.front3rdDeflection;
			sd.telemetry.rear3rdDeflection.flt = td.data.telemetry.rear3rdDeflection;

			sd.telemetry.frontWingHeight.flt = td.data.telemetry.frontWingHeight;
			sd.telemetry.frontRideHeight.flt = td.data.telemetry.frontRideHeight;
			sd.telemetry.rearRideHeight.flt = td.data.telemetry.rearRideHeight;
			sd.telemetry.drag.flt = td.data.telemetry.drag;
			sd.telemetry.frontDownforce.flt = td.data.telemetry.frontDownforce;
			sd.telemetry.rearDownforce.flt = td.data.telemetry.rearDownforce;

			sd.telemetry.engineTorque.flt = td.data.telemetry.engineTorque;
			sd.telemetry.headlights.bl = td.data.telemetry.headlights;

			sd.telemetry.frontTireCompoundIndex.lint = td.data.telemetry.frontTireCompoundIndex;
			sd.telemetry.rearTireCompoundIndex.lint = td.data.telemetry.rearTireCompoundIndex;
			sd.telemetry.frontFlapActivated.lint = td.data.telemetry.frontFlapActivated;
			sd.telemetry.rearFlapActivated.lint = td.data.telemetry.rearFlapActivated;
			sd.telemetry.ignitionStarter.lint = td.data.telemetry.ignitionStarter;
			sd.telemetry.speedLimiterAvailable.lint = td.data.telemetry.speedLimiterAvailable;
			sd.telemetry.antiStallActivated.lint = td.data.telemetry.antiStallActivated;
			sd.telemetry.rearBrakeBias.flt = td.data.telemetry.rearBrakeBias;
			sd.telemetry.turboBoostPressure.flt = td.data.telemetry.rearBrakeBias;

			// different handling for rF1 and rf2
			if (rfVersion == 1)
			{
				if (td.data.event.sectorFlag[sd.ownCar->currentSector.lint] > 0)
					sd.telemetry.flagShown.str = std::wstring(L"yellow");
				else if (sd.ownCar->currentSector.lint == 3 && td.data.event.sectorFlag[0] > 0)
					sd.telemetry.flagShown.str = std::wstring(L"yellow");
				else
				{
					sd.telemetry.flagShown.str = std::wstring(L""); 
				
					if (sd.ownCar->primaryFlag.lint == 6)
						sd.telemetry.flagShown.str = std::wstring(L"blue");
				}

			}
			if (rfVersion == 2)
			{
				// During launch of rFactor, event is valid but telemetry not yet
				if (td.data.event.numVehicles > 0)
				{
					if ((int)td.data.event.sectorFlag[sd.ownCar->currentSector.lint - 1] == 1)
						sd.telemetry.flagShown.str = std::wstring(L"yellow");
					else
					{
						sd.telemetry.flagShown.str = std::wstring(L"");
						if (sd.ownCar->primaryFlag.lint == 6)
							sd.telemetry.flagShown.str = std::wstring(L"blue");
					}

				}
			}

		} //  quickReady
		sd.deriveValues();
		return true;
	} // if read()
	else
	{
		return false;
	}
}

void ConnectorRF::disconnect()
{
	td.disconnect();
	sd.reset();
}
