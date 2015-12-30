#pragma once

#include "stdafx.h"

bool ConnectorAC::connect()
{
	if (td.connect())
	{
		sd.ownCar = new Driver;
		sd.ownCar->registerDriver();
		if (td.read())
		{
			sd.event.simName.str = std::wstring(L"Assetto Corsa");
			read();
			return true;
		}
	}
	return false;
}

int ConnectorAC::check()
{
	if (td.connect())
	{
		if (td.read())
		{
			td.disconnect();
			sd.reset();
			return 1;
		}
	}
	return 0;
}

std::wstring * ConnectorAC::simName()
{
	return new std::wstring(L"Assetto Corsa");
}

void ConnectorAC::sessionToStr() {
	switch (td.data.pfGraphics->session)
	{
	case AC_UNKNOWN:
		sd.event.sessionString.str.assign(L"Unknown session");
		break;
	case AC_PRACTICE:
		sd.event.sessionString.str.assign(L"Practice");
		break;
	case AC_QUALIFY:
		sd.event.sessionString.str.assign(L"Qualify");
		break;
	case AC_RACE:
		sd.event.sessionString.str.assign(L"Race");
		break;
	case AC_HOTLAP:
		sd.event.sessionString.str.assign(L"Hotlap");
		break;
	case AC_TIME_ATTACK:
		sd.event.sessionString.str.assign(L"Time attack");
		break;
	case AC_DRIFT:
		sd.event.sessionString.str.assign(L"Drift");
		break;
	case AC_DRAG:
		sd.event.sessionString.str.assign(L"Drag");
		break;
	default:
		sd.event.sessionString.str.assign(L"Unknown session");
	}
}

bool ConnectorAC::read()
{
	if (td.read())
	{
		if (slowReady())
		{
			acVersion.assign(td.data.pfStatic->acVersion);
			sd.event.trackName.str.assign(td.data.pfStatic->track);

			sd.event.driverName.str.assign(td.data.pfStatic->playerName);
			sd.event.driverName.str.append(L" ");
			sd.event.driverName.str.append(td.data.pfStatic->playerSurname);

			sd.ownCar->vehicleClass.str.assign(td.data.pfStatic->carModel);
			sd.event.numberOfLaps.lint = td.data.pfGraphics->numberOfLaps;
			sd.event.session.lint = td.data.pfGraphics->session;
			sessionToStr();
			sd.event.timeLeft.flt = td.data.pfGraphics->sessionTimeLeft;
			td.data.pfGraphics->isInPit == 1 ? sd.event.inRealtime.bl = true : sd.event.inRealtime.bl = true;
			sd.event.durationLeft.str = std::wstring(L"Time: ") + timeToString(td.data.pfGraphics->sessionTimeLeft, false, true);
			sd.event.numberOfLaps.lint = 2147483647; // no lap delimited races in AC

			sd.telemetry.frontTireCompoundName.str = std::wstring(td.data.pfGraphics->tyreCompound);
			sd.telemetry.rearTireCompoundName.str = std::wstring(td.data.pfGraphics->tyreCompound);

			sd.ownCar->lastTime.str->assign(td.data.pfGraphics->lastTime);
			sd.ownCar->bestTime.str->assign(td.data.pfGraphics->bestTime);
			sd.ownCar->split.str->assign(td.data.pfGraphics->split);
			sd.ownCar->lapNumber.lint = td.data.pfGraphics->completedLaps;
			sd.ownCar->place.lint = td.data.pfGraphics->position;


			slowFetched();
		}

		if (quickReady())
		{
			sd.session.numCars.lint = td.data.pfStatic->numCars;

			for (int i = 0; i < 4; i++)
			{
				sd.telemetry.wheels[i].camber.flt = td.data.pfPhysics->camberRAD[i];
				sd.telemetry.wheels[i].gripFract.flt = 1 - td.data.pfPhysics->wheelSlip[i]; // TODO: check
				sd.telemetry.wheels[i].suspensionDeflection.flt = td.data.pfPhysics->suspensionTravel[i];
				sd.telemetry.wheels[i].tireLoad.flt = td.data.pfPhysics->wheelLoad[i];
				sd.telemetry.wheels[i].tirePressure.flt = td.data.pfPhysics->wheelsPressure[i];
				sd.telemetry.wheels[i].wheelRotation.flt = td.data.pfPhysics->wheelAngularSpeed[i];
				sd.telemetry.wheels[i].dirtLevel.flt = td.data.pfPhysics->tyreDirtyLevel[i];
				sd.telemetry.wheels[i].tireWear.flt = td.data.pfPhysics->tyreWear[i];
				sd.telemetry.wheels[i].tempAvg.flt = td.data.pfPhysics->tyreCoreTemperature[i];
				// AC is missing detailed tire temperature data, set all to what we have
				sd.telemetry.wheels[i].tireInnerLayerInnerTemp.flt = td.data.pfPhysics->tyreCoreTemperature[i];
				sd.telemetry.wheels[i].tireInnerLayerMidTemp.flt = td.data.pfPhysics->tyreCoreTemperature[i];
				sd.telemetry.wheels[i].tireInnerLayerOuterTemp.flt = td.data.pfPhysics->tyreCoreTemperature[i];
				sd.telemetry.wheels[i].tireTempInside.flt = td.data.pfPhysics->tyreCoreTemperature[i];
				sd.telemetry.wheels[i].tireTempMiddle.flt = td.data.pfPhysics->tyreCoreTemperature[i];
				sd.telemetry.wheels[i].tireTempOutside.flt = td.data.pfPhysics->tyreCoreTemperature[i];
				sd.telemetry.wheels[i].suspensionMaxTravel.flt = td.data.pfStatic->suspensionMaxTravel[i];
			}
			sd.telemetry.fuel.flt = td.data.pfPhysics->fuel;
			sd.telemetry.fuel.max = td.data.pfStatic->maxFuel;
			if (td.data.pfPhysics->gear == 0)
				sd.telemetry.gear.str = L"R";
			else if (td.data.pfPhysics->gear == 1)
				sd.telemetry.gear.str = L"N";
			else
				sd.telemetry.gear.str = std::to_wstring(td.data.pfPhysics->gear-1);
			sd.telemetry.engineRPM.flt = td.data.pfPhysics->rpms;
			sd.telemetry.engineRPM.max = td.data.pfStatic->maxRpm;
			sd.telemetry.speed.flt = td.data.pfPhysics->speedKmh;

			sd.telemetry.unfilteredThrottle.flt = td.data.pfPhysics->gas;
			sd.telemetry.unfilteredBrake.flt = td.data.pfPhysics->brake;
			sd.telemetry.drs.flt = td.data.pfPhysics->drs;
			sd.telemetry.tc.flt = td.data.pfPhysics->tc;
			sd.telemetry.heading.flt = td.data.pfPhysics->heading;
			sd.telemetry.pitch.flt = td.data.pfPhysics->pitch;
			sd.telemetry.roll.flt = td.data.pfPhysics->roll;
			sd.telemetry.cgHeight.flt = td.data.pfPhysics->cgHeight;
			sd.telemetry.abs.flt = td.data.pfPhysics->abs;
			td.data.pfPhysics->pitLimiterOn == 1 ? sd.telemetry.speedLimiter.bl = true : sd.telemetry.speedLimiter.bl = false;

			sd.ownCar->accel.x = td.data.pfPhysics->accG[0] / 9.81;
			sd.ownCar->accel.y = td.data.pfPhysics->accG[1] / 9.81;
			sd.ownCar->accel.z = td.data.pfPhysics->accG[2] / 9.81;
			sd.ownCar->velocity.x = td.data.pfPhysics->velocity[0];
			sd.ownCar->velocity.y = td.data.pfPhysics->velocity[1];
			sd.ownCar->velocity.z = td.data.pfPhysics->velocity[2];
			sd.ownCar->position.x = td.data.pfGraphics->carCoordinates[0];
			sd.ownCar->position.y = td.data.pfGraphics->carCoordinates[1];
			sd.ownCar->position.z = td.data.pfGraphics->carCoordinates[2];
			sd.ownCar->currentTime.str->assign(td.data.pfGraphics->currentTime);
			td.data.pfGraphics->isInPit == 1 ? sd.ownCar->inPits.bl = true : sd.ownCar->inPits.bl = false;
			sd.ownCar->currentSector.lint = td.data.pfGraphics->currentSectorIndex;
			sd.telemetry.distanceTraveled.flt = td.data.pfGraphics->distanceTraveled;
			sd.telemetry.normalizedCarPosition.flt = td.data.pfGraphics->normalizedCarPosition;
		}

		sd.deriveValues();
		return true;
	}
	return false;
}

void ConnectorAC::disconnect()
{
	td.disconnect();
	sd.reset();
	delete sd.ownCar;
}
