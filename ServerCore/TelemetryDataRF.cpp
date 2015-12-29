#include "stdafx.h"

HANDLE hMapFile;
LPCTSTR pBuf = NULL;
UnifiedRfData data;

bool TelemetryData::connect()
{
	if (pBuf == NULL)
	{
		hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			transferBufferName);               // name of mapping object

		if (hMapFile == NULL)
		{
			// _tprintf(TEXT("Could not open file mapping object (%d).\n"), GetLastError());
			return false;
		}

		pBuf = (LPTSTR)MapViewOfFile(hMapFile, // handle to map object
			FILE_MAP_READ,  // read permission
			0,
			0,
			transferBufferSize);

		if (pBuf == NULL)
		{
			// _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());

			CloseHandle(hMapFile);

			return false;
		}

	}
	memcpy(&rFactorVersion, pBuf, sizeof(pBuf));
	return true;
}

TelemetryData::~TelemetryData()
{
	disconnect();
}


bool TelemetryData::read()
{
	if (pBuf != NULL)
	{
		try
		{
			memcpy(&data, pBuf, sizeof(data));
		}
		catch(...) { 
			disconnect();
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

void TelemetryData::disconnect()
{
	UnmapViewOfFile(pBuf); pBuf = NULL;
	CloseHandle(hMapFile);
	rFactorVersion = 0;
}

