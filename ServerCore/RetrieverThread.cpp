#include "stdafx.h"

struct threadData
{
	int sim;
	Connector * connector;
};

void SchedulerLaunch(void *pParam)
{
	threadData* td = (threadData*)pParam;
	ConnectorRF* crf = NULL;
	ConnectorAC* cac = NULL;

	switch (td->sim)
	{
	case SIM_RF:
		crf = (ConnectorRF*)td->connector;
//		if (crf->connect())
		if (true)
		{
			// Connect fetched data first, so we can sleep
			Sleep(interval);
			//WaitForSingleObject(hReadingMutex, INFINITE);

			int i = 0;
			// transfer data and check for stop signal
			while (crf->read() && WaitForSingleObject(hRunMutex, 0L) == WAIT_TIMEOUT)
			{
				i = i++;
				if (i==50)
				{
					// TODO: remove loop
					i = 0;
				}
				ReleaseMutex(hReadingMutex);
				Sleep(interval);
				WaitForSingleObject(hReadingMutex, INFINITE);
			}
		}
	case SIM_AC:
		cac = (ConnectorAC*)td->connector;
		if (true)
		{
			// Connect fetched data first, so we can sleep
			Sleep(interval);
			//WaitForSingleObject(hReadingMutex, INFINITE);

			int i = 0;
			// transfer data and check for stop signal
			while (cac->read() && WaitForSingleObject(hRunMutex, 0L) == WAIT_TIMEOUT)
			{
				i = i++;
				if (i == 50)
				{
					// TODO: remove loop
					i = 0;
				}
				ReleaseMutex(hReadingMutex);
				Sleep(interval);
				WaitForSingleObject(hReadingMutex, INFINITE);
			}
		}
	default:
		crf = NULL;
		cac = NULL;
		break;
	}

	ReleaseMutex(hRunMutex);
	ReleaseMutex(hReadingMutex);
	return;
}

ConnectorScheduler::ConnectorScheduler(int sim)
{
	ConnectorRF* crf = NULL; 
	ConnectorAC* cac = NULL;


	td = new threadData;
	td->sim = sim;
	switch (sim)
	{
	case SIM_RF:
		crf = new ConnectorRF();
		if (crf != NULL && crf->connect())
		{
			td->connector = (Connector*)crf;	// Pointer for thread
			connector = (Connector*)crf;		// for main program
			hRunMutex = CreateMutex(NULL, TRUE, NULL);      // set 
			hReadingMutex = CreateMutex(NULL, FALSE, NULL); // unset 
			_beginthread(SchedulerLaunch, 0, td);
			return;
		}
		delete crf; connector = NULL;
		break;
	case SIM_AC:
		cac = new ConnectorAC();
		if (cac != NULL && cac->connect())
		{
			td->connector = (Connector*)cac;	// Pointer for thread
			connector = (Connector*)cac;		// for main program
			hRunMutex = CreateMutex(NULL, TRUE, NULL);      // set 
			hReadingMutex = CreateMutex(NULL, FALSE, NULL); // unset 
			_beginthread(SchedulerLaunch, 0, td);
			return;
		}
		delete cac; connector = NULL;
		break;

	default:
		break;
	}
	delete td;
	sdem.reset();
}

ConnectorScheduler::~ConnectorScheduler()
{
	stop();
}

void ConnectorScheduler::stop()
{
	if (connector)
	{
		ReleaseMutex(hRunMutex);
		WaitForSingleObject(hReadingMutex, INFINITE);

		CloseHandle(hRunMutex);
		CloseHandle(hReadingMutex);
		//Sleep(100);
		delete (ConnectorRF*)connector; connector = nullptr;
		delete td;
		sdem.reset();
	}
}
