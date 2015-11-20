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
// plugin information
unsigned g_uPluginID = 0;
char     g_szPluginName[] = "OpenDashPlugin - 2005.11.30";
unsigned g_uPluginVersion = 001;
unsigned g_uPluginObjectCount = 1;
InternalsPluginInfo g_PluginInfo;


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

InternalsPluginInfo::InternalsPluginInfo()
{
	// put together a name for this plugin
	sprintf(m_szFullName, "%s - %s", g_szPluginName, InternalsPluginInfo::GetName());
}

const char*    InternalsPluginInfo::GetName()     const { return OpenDashPlugin::GetName(); }
const char*    InternalsPluginInfo::GetFullName() const { return m_szFullName; }
const char*    InternalsPluginInfo::GetDesc()     const { return "Example Internals Plugin"; }
const unsigned InternalsPluginInfo::GetType()     const { return OpenDashPlugin::GetType(); }
const char*    InternalsPluginInfo::GetSubType()  const { return OpenDashPlugin::GetSubType(); }
const unsigned InternalsPluginInfo::GetVersion()  const { return OpenDashPlugin::GetVersion(); }
void*          InternalsPluginInfo::Create()      const { return new OpenDashPlugin(); }

// InternalsPlugin class

const char OpenDashPlugin::m_szName[] = "InternalsPlugin";
const char OpenDashPlugin::m_szSubType[] = "Internals";
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

PluginObjectInfo * OpenDashPlugin::GetInfo()
{
	PluginObjectInfo * poi = new InternalsPluginInfo();
	return poi;	
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
		memcpy(data.telemetry.wheel[i].temperature, info.mWheel[i].mTemperature, sizeof(info.mWheel[i].mTemperature));
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
	data.telemetry.lastImpactPos = info.mLastImpactPos;
	data.telemetry.lastImpactMagnitude = info.mLastImpactMagnitude;

	data.telemetry.pos = info.mPos;
	data.telemetry.localVel = info.mLocalVel;
	data.telemetry.localAccel = info.mLocalAccel;
	data.telemetry.localRot = info.mLocalRot;
	data.telemetry.localRotAccel = info.mLocalRotAccel;
	

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
	data.event.wind = info.mWind;
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
		data.scoring[i].pos = info.mVehicle[i].mPos;
		data.scoring[i].localVel = info.mVehicle[i].mLocalVel;
		data.scoring[i].localAccel = info.mVehicle[i].mLocalAccel;
		// TODO: test copy result
		data.scoring[i].localRot = info.mVehicle[i].mLocalRot;
		data.scoring[i].localRotAccel = info.mVehicle[i].mLocalRotAccel;
	}

	WriteDataToMemory();
}

void OpenDashPlugin::WriteDataToMemory()
{
	CopyMemory((PVOID)pBuf, &data, sizeof(data));
}