//--------------------------------------------------------------------------
//
// Write rFactor2 data to memory mapped file
//
// Bengt Giger 2015
//
// License: MIT
//
//--------------------------------------------------------------------------

//#include <conio.h>
#include "stdafx.h"

const long OpenDashPlugin::rFactorVersion = 2;

// plugin information

extern "C" __declspec(dllexport)
const char * __cdecl GetPluginName() { return("OpenDashPlugin"); }

extern "C" __declspec(dllexport)
PluginObjectType __cdecl GetPluginType() { return(PO_INTERNALS); }

extern "C" __declspec(dllexport)
int __cdecl GetPluginVersion() { return(6); } // InternalsPluginV06 functionality

extern "C" __declspec(dllexport)
PluginObject * __cdecl CreatePluginObject() { return((PluginObject *) new OpenDashPlugin); }

extern "C" __declspec(dllexport)
void __cdecl DestroyPluginObject(PluginObject *obj) { delete((OpenDashPlugin *)obj); }


void OpenDashPlugin::Log(const char * const msg)
{
	FILE *fo;

	fo = fopen("c:\\tmp\\rf2transferOutput.txt", "a");
	if (fo != NULL)
	{
		fprintf(fo, "%s\n", msg);
		fclose(fo);
	}
}

void OpenDashPlugin::Startup(long version)
{
	memMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		transferBufferSize,                // maximum object size (low-order DWORD)
		transferBufferName);                 // name of mapping object

	if (memMapFile == NULL)
	{
		Log(TEXT("Could not open memory mapping file\n"));
		return;
	}
	// Log("Memory mapping file opened");

	pBuf = (LPTSTR)MapViewOfFile(memMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		transferBufferSize);

	if (pBuf == NULL)
	{
		Log(TEXT("Could not map view of file.\n"));

		CloseHandle(memMapFile);
		return;
	}

	data.scoringValid = false;
	data.telemetryValid = false;
	data.inSession = false;
	data.rFactorVersion = rFactorVersion;


	return;
}


void OpenDashPlugin::Shutdown()
{
	UnmapViewOfFile(pBuf);
	CloseHandle(memMapFile);
}


void OpenDashPlugin::EnterRealtime()
{
	// start up timer every time we enter realtime
	data.driving = true;
	data.telemetryValid = true;
	WriteDataToMemory();
}


void OpenDashPlugin::ExitRealtime()
{
	data.driving = false;
	WriteDataToMemory();
}

void OpenDashPlugin::StartSession()
{
	data.inSession = true;
	WriteDataToMemory();
}

void OpenDashPlugin::EndSession()
{
	data.inSession = false;
	WriteDataToMemory();
}


void OpenDashPlugin::UpdateTelemetry(const TelemInfoV01 &info)
{
	// Log("Updating telemetry");
	data.telemetryValid = true;

	data.telemetry.lapNumber = info.mLapNumber;
	data.telemetry.lapStartTime = info.mLapStartET;
	strcpy(data.telemetry.vehicleName, info.mVehicleName);
	strcpy(data.telemetry.trackName, info.mTrackName);
	data.telemetry.gear = info.mGear;
	data.telemetry.engineRPM = info.mEngineRPM;
	data.telemetry.engineWaterTemp = info.mEngineWaterTemp;
	data.telemetry.engineOilTemp = info.mEngineOilTemp;
	data.telemetry.clutchRPM = info.mClutchRPM;
	data.telemetry.unfilteredThrottle = info.mUnfilteredThrottle;
	data.telemetry.unfilteredBrake = info.mUnfilteredBrake;
	data.telemetry.unfilteredSteering = info.mUnfilteredSteering;
	data.telemetry.unfilteredClutch = info.mUnfilteredClutch;
	for (int i = 0; i < 4; i++)
	{
		data.telemetry.wheel[i].brakeTemp = info.mWheel[i].mBrakeTemp;
		data.telemetry.wheel[i].detached = info.mWheel[i].mDetached;
		data.telemetry.wheel[i].flat = info.mWheel[i].mFlat;
		data.telemetry.wheel[i].gripFract = info.mWheel[i].mGripFract;
		data.telemetry.wheel[i].pressure = info.mWheel[i].mPressure;
		memcpy(data.telemetry.wheel[i].temperature, info.mWheel[i].mTemperature, sizeof(info.mWheel[i].mTemperature));
		data.telemetry.wheel[i].tireLoad = info.mWheel[i].mTireLoad;
		data.telemetry.wheel[i].wear = info.mWheel[i].mWear;
		data.telemetry.wheel[i].rotation = info.mWheel[i].mRotation;
		data.telemetry.wheel[i].suspensionDeflection = info.mWheel[i].mSuspensionDeflection;
		data.telemetry.wheel[i].rideHeight = info.mWheel[i].mRideHeight;
		data.telemetry.wheel[i].lateralForce = info.mWheel[i].mLateralForce;
		data.telemetry.wheel[i].suspensionForce = info.mWheel[i].mSuspForce;
		data.telemetry.wheel[i].brakePressure = info.mWheel[i].mBrakePressure;
		data.telemetry.wheel[i].lateralPatchVel = info.mWheel[i].mLateralPatchVel;
		data.telemetry.wheel[i].longitudinalPatchVel = info.mWheel[i].mLongitudinalPatchVel;
		data.telemetry.wheel[i].lateralGroundVel = info.mWheel[i].mLateralGroundVel;
		data.telemetry.wheel[i].longitudinalGroundVel = info.mWheel[i].mLongitudinalGroundVel;
		data.telemetry.wheel[i].camber = info.mWheel[i].mCamber;
		data.telemetry.wheel[i].longitudinalForce = info.mWheel[i].mLongitudinalForce;
		data.telemetry.wheel[i].verticalTireDeflection = info.mWheel[i].mVerticalTireDeflection;
		data.telemetry.wheel[i].wheelYLocation = info.mWheel[i].mWheelYLocation;
		data.telemetry.wheel[i].toe = info.mWheel[i].mToe;
		data.telemetry.wheel[i].tireCarcassTemperature = info.mWheel[i].mTireCarcassTemperature;
		memcpy(data.telemetry.wheel[i].tireInnerLayerTemperature, info.mWheel[i].mTireInnerLayerTemperature, sizeof(info.mWheel[i].mTireInnerLayerTemperature));
	}
	data.telemetry.fuel = info.mFuel;
	data.telemetry.engineMaxRPM = info.mEngineMaxRPM;
	data.telemetry.scheduledPitstops = info.mScheduledStops;
	data.telemetry.overheating = info.mOverheating;
	data.telemetry.detached = info.mDetached;  
	memcpy(data.telemetry.dentSeverity, info.mDentSeverity, sizeof(info.mDentSeverity));
	data.telemetry.lastImpactTime = info.mLastImpactET;
	data.telemetry.lastImpactPos.Set(info.mLastImpactPos.x, info.mLastImpactPos.y, info.mLastImpactPos.z);
	data.telemetry.lastImpactMagnitude = info.mLastImpactMagnitude;

	data.telemetry.pos.Set(info.mPos.x, info.mPos.y, info.mPos.z);
	data.telemetry.localVel.Set(info.mLocalVel.x, info.mLocalVel.y, info.mLocalVel.z);
	data.telemetry.localAccel.Set(info.mLocalAccel.x, info.mLocalAccel.y, info.mLocalAccel.z);
	memcpy(data.telemetry.orientation, info.mOri, sizeof(info.mOri));
	data.telemetry.localRot.Set(info.mLocalRot.x, info.mLocalRot.y, info.mLocalRot.z);
	data.telemetry.localRotAccel.Set(info.mLocalRotAccel.x, info.mLocalRotAccel.y, info.mLocalRotAccel.z);

	// Additional data provided by rFactor 2
	data.telemetry.filteredThrottle = info.mFilteredThrottle;
	data.telemetry.filteredBrake = info.mFilteredBrake;
	data.telemetry.filteredSteering = info.mFilteredSteering;
	data.telemetry.filteredClutch = info.mFilteredClutch;
	data.telemetry.front3rdDeflection = info.mFront3rdDeflection;
	data.telemetry.rear3rdDeflection = info.mRear3rdDeflection;
	data.telemetry.frontWingHeight = info.mFrontWingHeight;
	data.telemetry.frontRideHeight = info.mFrontRideHeight;
	data.telemetry.rearRideHeight = info.mRearRideHeight;
	data.telemetry.drag = info.mDrag;
	data.telemetry.frontDownforce = info.mFrontDownforce;
	data.telemetry.rearDownforce = info.mRearDownforce;
	data.telemetry.engineTorque = info.mEngineTorque;
	data.telemetry.currentSector = info.mCurrentSector;
	data.telemetry.headlights = info.mHeadlights;
	data.telemetry.speedLimiter = info.mSpeedLimiter;
	data.telemetry.maxGears = info.mMaxGears;
	data.telemetry.frontTireCompoundIndex = info.mFrontTireCompoundIndex;
	data.telemetry.rearTireCompoundIndex = info.mRearTireCompoundIndex;
	data.telemetry.fuelCapacity = info.mFuelCapacity;
	data.telemetry.frontFlapActivated = info.mFrontFlapActivated;
	data.telemetry.rearFlapActivated = info.mRearFlapActivated;
	data.telemetry.rearFlapLegalStatus = info.mRearFlapLegalStatus;
	data.telemetry.ignitionStarter = info.mIgnitionStarter;
	strcpy(data.telemetry.frontTireCompoundName, info.mFrontTireCompoundName);
	strcpy(data.telemetry.rearTireCompoundName, info.mRearTireCompoundName);
	data.telemetry.speedLimiterAvailable = info.mSpeedLimiterAvailable;
	data.telemetry.antiStallActivated = info.mAntiStallActivated;
	data.telemetry.rearBrakeBias = info.mRearBrakeBias; 
	data.telemetry.turboBoostPressure = info.mTurboBoostPressure;

	WriteDataToMemory();
}


void OpenDashPlugin::UpdateScoring(const ScoringInfoV01 &info)
{
	// Note: function is called twice per second now (instead of once per second in previous versions)

	data.scoringValid = true;

	strcpy(data.event.trackName, info.mTrackName);
	data.event.session = info.mSession;
	data.event.currentTime = info.mCurrentET;
	data.event.endTime = info.mEndET;
	data.event.maxLaps = info.mMaxLaps;
	data.event.lapDist = info.mLapDist;
	data.event.numVehicles = info.mNumVehicles;
	data.event.gamePhase = info.mGamePhase;
	data.event.yellowFlagState = info.mYellowFlagState;
	memcpy(data.event.sectorFlag, info.mSectorFlag, sizeof(info.mSectorFlag));
	data.event.startLight = info.mStartLight;
	data.event.numRedLights = info.mNumRedLights;
	data.event.inRealtime = info.mInRealtime;
	strcpy(data.event.playerName, info.mPlayerName);
	data.event.darkCloud = info.mDarkCloud;
	data.event.raining = info.mRaining;
	data.event.ambientTemp = info.mAmbientTemp;
	data.event.trackTemp = info.mTrackTemp;
	data.event.wind.Set(info.mWind.x, info.mWind.y, info.mWind.z);
	data.event.onPathWetness = info.mOnPathWetness;
	data.event.offPathWetness = info.mOffPathWetness;

	for (int i = 0; i < info.mNumVehicles;  i++)
	{
		data.scoring[i].isPlayer = info.mVehicle[i].mIsPlayer;
		strcpy(data.scoring[i].driverName, info.mVehicle[i].mDriverName);
		strcpy(data.scoring[i].vehicleName, info.mVehicle[i].mVehicleName);
		data.scoring[i].totalLaps = info.mVehicle[i].mTotalLaps;
		data.scoring[i].sector = info.mVehicle[i].mSector; // 0=sector3, 1=sector1, 2=sector2 (don't ask why)
		data.scoring[i].finishStatus = info.mVehicle[i].mFinishStatus; // 0=none, 1=finished, 2=dnf, 3=dq
		data.scoring[i].lapDist = info.mVehicle[i].mLapDist;
		data.scoring[i].pathLateral = info.mVehicle[i].mPathLateral;
		data.scoring[i].trackEdge = info.mVehicle[i].mTrackEdge;
		data.scoring[i].bestSector1 = info.mVehicle[i].mBestSector1;
		data.scoring[i].bestSector2 = info.mVehicle[i].mBestSector2;
		data.scoring[i].bestLapTime = info.mVehicle[i].mBestLapTime;
		data.scoring[i].lastSector1 = info.mVehicle[i].mLastSector1;
		data.scoring[i].lastSector2 = info.mVehicle[i].mLastSector2;
		data.scoring[i].lastLapTime = info.mVehicle[i].mLastLapTime;
		data.scoring[i].curSector1 = info.mVehicle[i].mCurSector1;
		data.scoring[i].curSector2 = info.mVehicle[i].mCurSector2;
		data.scoring[i].numPitstops = info.mVehicle[i].mNumPitstops;
		data.scoring[i].numPenalties = info.mVehicle[i].mNumPenalties;
		data.scoring[i].inPits = info.mVehicle[i].mInPits;
		data.scoring[i].place = info.mVehicle[i].mPlace;
		data.scoring[i].timeBehindNext = info.mVehicle[i].mTimeBehindNext;
		data.scoring[i].lapsBehindNext = info.mVehicle[i].mLapsBehindNext;
		data.scoring[i].timeBehindLeader = info.mVehicle[i].mTimeBehindLeader;
		data.scoring[i].lapsBehindLeader = info.mVehicle[i].mLapsBehindLeader;
		data.scoring[i].lapStartTime = info.mVehicle[i].mLapStartET;
		data.scoring[i].speed = sqrt(pow(info.mVehicle[i].mLocalVel.x, 2) + pow(info.mVehicle[i].mLocalVel.y, 2) + pow(info.mVehicle[i].mLocalVel.z, 2));
		data.scoring[i].control = info.mVehicle[i].mControl;
		data.scoring[i].pos.Set(info.mVehicle[i].mPos.x, info.mVehicle[i].mPos.y, info.mVehicle[i].mPos.z);
		data.scoring[i].localVel.Set(info.mVehicle[i].mLocalVel.x, info.mVehicle[i].mLocalVel.y, info.mVehicle[i].mLocalVel.z);
		data.scoring[i].localAccel.Set(info.mVehicle[i].mLocalAccel.x, info.mVehicle[i].mLocalAccel.y, info.mVehicle[i].mLocalAccel.z);
		memcpy(data.scoring[i].orientation, info.mVehicle[i].mOri, sizeof(info.mVehicle[i].mOri));
		data.scoring[i].localRot.Set(info.mVehicle[i].mLocalRot.x, info.mVehicle[i].mLocalRot.y, info.mVehicle[i].mLocalRot.z);
		data.scoring[i].localRotAccel.Set(info.mVehicle[i].mLocalRotAccel.x, info.mVehicle[i].mLocalRotAccel.y, info.mVehicle[i].mLocalRotAccel.z);

		
		// Extension for rFactor 2
		data.scoring[i].slotId = info.mVehicle[i].mID;
		data.scoring[i].timeIntoLap = info.mVehicle[i].mTimeIntoLap;
		data.scoring[i].estimatedLapTime = info.mVehicle[i].mEstimatedLapTime;
		strcpy(data.scoring[i].pitGroup, info.mVehicle[i].mPitGroup);
		memcpy(data.scoring[i].upgradePack, info.mVehicle[i].mUpgradePack, sizeof(info.mVehicle[i].mUpgradePack));
		data.scoring[i].headlights = info.mVehicle[i].mHeadlights;
		data.scoring[i].pitState = info.mVehicle[i].mPitState; // 0=none, 1=request, 2=entering, 3=stopped, 4=exiting
		data.scoring[i].qualification = info.mVehicle[i].mQualification;
		data.scoring[i].primaryFlag = info.mVehicle[i].mFlag; // currently only 0=green or 6=blue
	}

	WriteDataToMemory();
}

void OpenDashPlugin::WriteDataToMemory()
{
	CopyMemory((PVOID)pBuf, &data, sizeof(data));
}