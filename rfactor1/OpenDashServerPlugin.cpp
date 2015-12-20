//‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹
//›                                                                         ﬁ
//› Write rFactor2 data to memory mapped file                               ﬁ
//›                                                                         ﬁ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

//#include <conio.h>
#include <stdio.h>
#include <math.h>
#include "InternalsPlugin.hpp"
#include "OpenDashServerPlugin.hpp"

const long OpenDashPlugin::rFactorVersion = 1;

// plugin information
unsigned g_uPluginID = 0;
char     g_szPluginName[] = "OpenDashPlugin";
unsigned g_uPluginVersion = 001;
unsigned g_uPluginObjectCount = 1;
OpenSimDashPluginInfo g_PluginInfo;


// interface to plugin information
extern "C" __declspec(dllexport)
const char* __cdecl GetPluginName() { return g_szPluginName; }

extern "C" __declspec(dllexport)
unsigned __cdecl GetPluginVersion() { return g_uPluginVersion; }

extern "C" __declspec(dllexport)
unsigned __cdecl GetPluginObjectCount() { return g_uPluginObjectCount; }

// get the plugin-info object used to create the plugin.
extern "C" __declspec(dllexport)
PluginObjectInfo* __cdecl GetPluginObjectInfo(const unsigned uIndex)
{
	switch (uIndex)
	{
	case 0:
		return  &g_PluginInfo;
	default:
		return 0;
	}
}

OpenSimDashPluginInfo::OpenSimDashPluginInfo()
{
	// put together a name for this plugin
	sprintf(m_szFullName, "%s - %s", g_szPluginName, OpenSimDashPluginInfo::GetName());
}

const char*    OpenSimDashPluginInfo::GetName()     const { return OpenDashPlugin::GetName(); }
const char*    OpenSimDashPluginInfo::GetFullName() const { return m_szFullName; }
const char*    OpenSimDashPluginInfo::GetDesc()     const { return "OpenSimDash Connector Plugin"; }
const unsigned OpenSimDashPluginInfo::GetType()     const { return OpenDashPlugin::GetType(); }
const char*    OpenSimDashPluginInfo::GetSubType()  const { return OpenDashPlugin::GetSubType(); }
const unsigned OpenSimDashPluginInfo::GetVersion()  const { return OpenDashPlugin::GetVersion(); }
void*          OpenSimDashPluginInfo::Create()      const { return new OpenDashPlugin(); }

// InternalsPlugin class

const char OpenDashPlugin::m_szName[] = "OpenSimDash Plugin";
const char OpenDashPlugin::m_szSubType[] = "OpenSimDash";
const unsigned OpenDashPlugin::m_uID = 1;
const unsigned OpenDashPlugin::m_uVersion = 3;  // set to 3 for InternalsPluginV3 functionality and added graphical and vehicle info


PluginObjectInfo *OpenDashPlugin::GetInfo()
{
	return &g_PluginInfo;
}




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

void OpenDashPlugin::Startup()
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
		Log("Could not open memory mapping file\n");
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
		Log("Could not map view of file.\n");

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


void OpenDashPlugin::UpdateTelemetry(const TelemInfoV2 &info)
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
		data.telemetry.wheel[i].temperature[0] = info.mWheel[i].mTemperature[0];
		data.telemetry.wheel[i].temperature[1] = info.mWheel[i].mTemperature[1];
		data.telemetry.wheel[i].temperature[2] = info.mWheel[i].mTemperature[2];
		data.telemetry.wheel[i].tireLoad = info.mWheel[i].mTireLoad;
		data.telemetry.wheel[i].wear = info.mWheel[i].mWear;
		data.telemetry.wheel[i].rotation = info.mWheel[i].mRotation;
		data.telemetry.wheel[i].suspensionDeflection = info.mWheel[i].mSuspensionDeflection;
		data.telemetry.wheel[i].rideHeight = info.mWheel[i].mRideHeight;
		data.telemetry.wheel[i].lateralForce = info.mWheel[i].mLateralForce;
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
	data.telemetry.localRot.Set(info.mLocalRot.x, info.mLocalRot.y, info.mLocalRot.z);
	data.telemetry.localRotAccel.Set(info.mLocalRotAccel.x, info.mLocalRotAccel.y, info.mLocalRotAccel.z);
	

	WriteDataToMemory();
}


void OpenDashPlugin::UpdateScoring(const ScoringInfoV2 &info)
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
		data.scoring[i].localRot.Set(info.mVehicle[i].mLocalRot.x, info.mVehicle[i].mLocalRot.y, info.mVehicle[i].mLocalRot.z);
		data.scoring[i].localRotAccel.Set(info.mVehicle[i].mLocalRotAccel.x, info.mVehicle[i].mLocalRotAccel.y, info.mVehicle[i].mLocalRotAccel.z);
	}

	WriteDataToMemory();
}

void OpenDashPlugin::WriteDataToMemory()
{
	CopyMemory((PVOID)pBuf, &data, sizeof(data));
}
