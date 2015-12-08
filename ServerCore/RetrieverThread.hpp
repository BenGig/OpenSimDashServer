#pragma once
#include "stdafx.h"

const int SIM_RF = 1;
const int SIM_AC = 2;

const DWORD interval = 20; // milliseconds

static HANDLE hRunMutex;				// to signal end of execution
static HANDLE hReadingMutex;			// Locked during data transfer, get this before fetching data from connector


/*
Controls the thread which transforms individual sim data into server food.
*/
class ConnectorScheduler
{
public:
	ConnectorScheduler(int sim);
	~ConnectorScheduler();

	void stop();

	Connector * connector;

private:
	struct threadData
	{
		int sim;
		Connector * connector;
	};
	threadData * td;
};