#pragma once

#include "stdafx.h"

const unsigned char RF_GAMEPHASE_BEFORE_SESSION = 0;
const unsigned char RF_GAMEPHASE_RECONAISSANCE = 1;
const unsigned char RF_GAMEPHASE_GRID_WALKTHRU = 2;
const unsigned char RF_GAMEPHASE_FORMATIONLAP = 3;
const unsigned char RF_GAMEPHASE_STARTLIGHT_COUNTDOWN = 4;
const unsigned char RF_GAMEPHASE_GREEN_FLAG = 5;
const unsigned char RF_GAMEPHASE_FULL_YELLOW = 6;
const unsigned char RF_GAMEPHASE_SESSION_STOPPED = 7;
const unsigned char RF_GAMEPHASE_SESSION_OVER = 8;

// yellow flag states (full course)
const signed char RF_YELLOWFLAG_INVALID = -1;
const signed char RF_YELLOWFLAG_NONE = 0;
const signed char RF_YELLOWFLAG_PENDING = 1;
const signed char RF_YELLOWFLAG_PITS_CLOSED = 2;
const signed char RF_YELLOWFLAG_PIT_LEAD_LAP = 3;
const signed char RF_YELLOWFLAG_PITS_OPEN = 4;
const signed char RF_YELLOWFLAG_LAST_LAP = 5;
const signed char RF_YELLOWFLAG_RESUME = 6;
const signed char RF_YELLOWFLAG_RACE_HALT = 7;

// in control states
const signed char RF_CONTROL_NOBODY = -1;
const signed char RF_CONTROL_PLAYER = 0;
const signed char RF_CONTROL_AI = 1;
const signed char RF_CONTROL_REMOTE = 2;
const signed char RF_CONTROL_REPLAY = 3;

// Session types
const signed char RF_SESSION_TYPE_TESTDAY = 0;
const signed char RF_SESSION_TYPE_PRACTICE = 1;		// session types 1-4
const signed char RF_SESSION_TYPE_QUALIFICATION = 2;	// session types 5-8
const signed char RF_SESSION_TYPE_WARMUP = 3;			// session type 9
const signed char RF_SESSION_TYPE_RACE = 4;			// session types 10-13
static signed char RF_SESSION_TYPE[14];					// i.e. session_type = SESSION_TYPE[type_from_rFactor]

const signed char RF_IGNITION_OFF = 0;
const signed char RF_IGNITION_ON = 1;
const signed char RF_IGNITION_STARTER_ON = 2;

// Finish status
const signed char RF_FINISHER_NONE = 0;
const signed char RF_FINISHER_OK = 1;
const signed char RF_FINISHER_DNF = 2;
const signed char RF_FINISHER_DQ = 3;

const signed char RF_PIT_STATE_NONE = 0;
const signed char RF_PIT_STATE_REQUESTED = 1;
const signed char RF_PIT_STATE_ENTERING = 2;
const signed char RF_PIT_STATE_STOPPED = 3;
const signed char RF_PIT_STATE_EXITING = 4;




class ConnectorRF: public Connector
{
public:
	int rfVersion;
	SimData * sd = new SimData();

	ConnectorRF();

	bool Connect();
	int Check();
	std::wstring * SimName();
	bool Read();
	void Disconnect();


private:
	TelemetryData td;
	void sessionToStr();
};
