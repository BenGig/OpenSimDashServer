#include "stdafx.h"

std::wstring * SimRacingToolsJson1(SimData * sd)
{
	std::wstring * str = new std::wstring();
	str->append(L"{\"jtype\":1,");

	str->append(L"\"RPM\":"); str->append(std::to_wstring(lround(sd->telemetry.engineRPM.flt))); str->append(L",");
	str->append(L"\"Speed\":"); str->append(std::to_wstring(lround(sd->telemetry.speed.flt))); str->append(L",");
	str->append(L"\"Gear\":"); str->append(sd->telemetry.gear.toString()); str->append(L"}");

	return str;
}

std::wstring * SimRacingToolsJson3(SimData * sd)
{
	std::wstring * str = new std::wstring();
	str->append(L"{\"jtype\":3,");

	str->append(L"\"Sim\":\"\",");
	str->append(L"\"Session\":"); str->append(sd->event.session.toString()); str->append(L",");
	str->append(L"\"Speedunit\":"); str->append(std::wstring(L"\"km/h\"")); str->append(L",");
	str->append(L"\"Tempunit\":"); str->append(std::wstring(L"\"°C\"")); str->append(L",");
	str->append(L"\"Presunit\":"); str->append(std::wstring(L"\"KPa\"")); str->append(L",");
	str->append(L"\"Cars\":"); str->append(sd->session.numCars.toString()); str->append(L",");
	str->append(L"\"Car\":\""); str->append(sd->ownCar->vehicleName.toString()); str->append(L"\",");
	str->append(L"\"Group\":\""); str->append(sd->ownCar->vehicleClass.toString()); str->append(L"\",");
	str->append(L"\"Sectors\":"); str->append(sd->event.sectorCount.toString()); str->append(L",");
	str->append(L"\"MAXRPM\":"); str->append(sd->telemetry.maxRPM.toString()); str->append(L",");
	if (sd->telemetry.engineRPM.flt > sd->telemetry.maxRPM.flt * 0.95)
		str->append(L"\"ShiftIndicator\":true,");
	else
		str->append(L"\"ShiftIndicator\":false,");
	str->append(L"\"FuelMAX\":"); str->append(sd->telemetry.maxFuel.toString()); str->append(L",");
	str->append(L"\"SuspensionMAX\":-1}");

	return str;
}

std::wstring * SimRacingToolsJson2(SimData * sd)
{
	std::wstring * str = new std::wstring();
	str->append(L"{\"jtype\":2,");

	str->append(L"\"BestRPM\":-1,");
	str->append(L"\"Topspeed\":-1,");
	str->append(L"\"Current\":"); str->append(sd->ownCar->currentTime.toString()); str->append(L",");
	str->append(L"\"Last\":"); str->append(sd->ownCar->lastTime.toString()); str->append(L",");
	str->append(L"\"Best\":"); str->append(sd->ownCar->currentTime.toString()); str->append(L",");
	str->append(L"\"CurrentSector\":"); str->append(sd->ownCar->currentSector.toString()); str->append(L",");
	str->append(L"\"SectorTimes\":\"");
	str->append(sd->ownCar->lastSector1.toString()); str->append(L"; ");
	str->append(sd->ownCar->lastSector2.toString()); str->append(L"; ");
	str->append(std::to_wstring(sd->ownCar->lastTime.flt - sd->ownCar->lastSector1.flt - sd->ownCar->lastSector2.flt)); str->append(L"\",");
	str->append(L"\"BestSectorTimes\":\"");
	str->append(sd->ownCar->bestSector1.toString()); str->append(L"; ");
	str->append(sd->ownCar->bestSector2.toString()); str->append(L"; ");
	str->append(std::to_wstring(sd->ownCar->bestTime.flt - sd->ownCar->bestSector1.flt - sd->ownCar->bestSector2.flt)); str->append(L"\",");
	str->append(L"\"Laps\":"); str->append(sd->ownCar->lapNumber.toString()); str->append(L",");
	str->append(L"\"Pos\":"); str->append(sd->ownCar->place.toString()); str->append(L",");
	str->append(L"\"Damage\":[\"t1\":0,\"t2\":0,\"t3\":0,\"t4\":0,\"t5\":0],");
	str->append(L"\"CamberRAD\":[\"FL\":"); 
	str->append(sd->telemetry.wheels[0].camber.toString()); str->append(L",");
	str->append(L"\"FR\":");
	str->append(sd->telemetry.wheels[1].camber.toString()); str->append(L",");
	str->append(L"\"RL\":");
	str->append(sd->telemetry.wheels[2].camber.toString()); str->append(L",");
	str->append(L"\"RR\":");
	str->append(sd->telemetry.wheels[3].camber.toString()); str->append(L"]},");
	str->append(L"\"Suspension\":[\"FL\":");
	str->append(sd->telemetry.wheels[0].rideHeight.toString()); str->append(L",");
	str->append(L"\"FR\":");
	str->append(sd->telemetry.wheels[1].rideHeight.toString()); str->append(L",");
	str->append(L"\"RL\":");
	str->append(sd->telemetry.wheels[2].rideHeight.toString()); str->append(L",");
	str->append(L"\"RR\":");
	str->append(sd->telemetry.wheels[3].rideHeight.toString()); str->append(L"]},");
	str->append(L"\"WheelLoad\":[\"FL\":");
	str->append(sd->telemetry.wheels[0].tireLoad.toString()); str->append(L",");
	str->append(L"\"FR\":");
	str->append(sd->telemetry.wheels[1].tireLoad.toString()); str->append(L",");
	str->append(L"\"RL\":");
	str->append(sd->telemetry.wheels[2].tireLoad.toString()); str->append(L",");
	str->append(L"\"RR\":");
	str->append(sd->telemetry.wheels[3].tireLoad.toString()); str->append(L"]},");

	str->append(L"\"TyreTemp\":[");
	str->append(L"\"left\":");
	str->append(sd->telemetry.wheels[0].tireInnerLayerOuterTemp.toString());
	str->append(L",\"middle\":");
	str->append(sd->telemetry.wheels[0].tireInnerLayerMidTemp.toString());
	str->append(L",\"right\":");
	str->append(sd->telemetry.wheels[0].tireInnerLayerInnerTemp.toString());
	str->append(L"], [");
	str->append(L"\"right\":");
	str->append(sd->telemetry.wheels[1].tireInnerLayerOuterTemp.toString());
	str->append(L",\"middle\":");
	str->append(sd->telemetry.wheels[1].tireInnerLayerMidTemp.toString());
	str->append(L",\"left\":");
	str->append(sd->telemetry.wheels[1].tireInnerLayerInnerTemp.toString());
	str->append(L"], [");
	str->append(L"\"left\":");
	str->append(sd->telemetry.wheels[2].tireInnerLayerOuterTemp.toString());
	str->append(L",\"middle\":");
	str->append(sd->telemetry.wheels[2].tireInnerLayerMidTemp.toString());
	str->append(L",\"right\":");
	str->append(sd->telemetry.wheels[2].tireInnerLayerInnerTemp.toString());
	str->append(L"], [");
	str->append(L"\"right\":");
	str->append(sd->telemetry.wheels[3].tireInnerLayerOuterTemp.toString());
	str->append(L",\"middle\":");
	str->append(sd->telemetry.wheels[3].tireInnerLayerMidTemp.toString());
	str->append(L",\"left\":");
	str->append(sd->telemetry.wheels[3].tireInnerLayerInnerTemp.toString());
	str->append(L"]");

	str->append(L"\"TyrePress\":[\"FL\":");
	str->append(sd->telemetry.wheels[0].tirePressure.toString()); str->append(L",");
	str->append(L"\"FR\":");
	str->append(sd->telemetry.wheels[1].tirePressure.toString()); str->append(L",");
	str->append(L"\"RL\":");
	str->append(sd->telemetry.wheels[2].tirePressure.toString()); str->append(L",");
	str->append(L"\"RR\":");
	str->append(sd->telemetry.wheels[3].tirePressure.toString()); str->append(L"]},");

	str->append(L"\"TyreWear\":[\"FL\":");
	str->append(sd->telemetry.wheels[0].tireWear.toString()); str->append(L",");
	str->append(L"\"FR\":");
	str->append(sd->telemetry.wheels[1].tireWear.toString()); str->append(L",");
	str->append(L"\"RL\":");
	str->append(sd->telemetry.wheels[2].tireWear.toString()); str->append(L",");
	str->append(L"\"RR\":");
	str->append(sd->telemetry.wheels[3].tireWear.toString()); str->append(L"]},");

	str->append(L"\"TyreDirt\":[\"FL\":");
	str->append(sd->telemetry.wheels[0].dirtLevel.toString()); str->append(L",");
	str->append(L"\"FR\":");
	str->append(sd->telemetry.wheels[1].dirtLevel.toString()); str->append(L",");
	str->append(L"\"RL\":");
	str->append(sd->telemetry.wheels[2].dirtLevel.toString()); str->append(L",");
	str->append(L"\"RR\":");
	str->append(sd->telemetry.wheels[3].dirtLevel.toString()); str->append(L"]},");

	str->append(L"\"Tyre\":"); str->append(sd->telemetry.rearTireCompoundName.toString()); str->append(L",");
	str->append(L"\"Fuel\":"); str->append(sd->telemetry.fuel.toString()); str->append(L",");
	str->append(L"\"AvgFuel\":-1,");
	str->append(L"\"FuelLapsLeft\":-1,");
	str->append(L"\"Distance\":-1,");
	str->append(L"\"LapDistance\":"); str->append(sd->event.lapDist.toString()); str->append(L",");
	str->append(L"\"InPit\":"); str->append(sd->ownCar->inPits.toString()); str->append(L",");
	str->append(L"\"InPit\":"); str->append(sd->event.numberOfLaps.toString()); str->append(L",");
	str->append(L"\"ET\":"); str->append(std::to_wstring(sd->event.endTime.flt - sd->event.timeLeft.flt)); str->append(L",");
	str->append(L"\"ET\":"); str->append(sd->event.endTime.toString()); str->append(L",");
	str->append(L"\"EW\":-1,");
	str->append(L"\"TimeLeft\":"); str->append(sd->event.timeLeft.toString()); str->append(L",");
	str->append(L"\"Status\":"); str->append(sd->ownCar->finishStatus.toString()); str->append(L",");
	str->append(L"\"PenaltyTime\":-1,");
	str->append(L"\"Penalty\":-1,");
	str->append(L"\"Flag\":"); str->append(sd->ownCar->primaryFlag.toString());
	str->append(L"}");
	return str;
}