#include "stdafx.h"

HANDLE hMapFile = nullptr;
LPCTSTR pBuf = nullptr;
UnifiedRfData data;

bool TelemetryDataRF::connect()
{
	if (pBuf == nullptr)
	{
		hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			transferBufferName);               // name of mapping object

		if (hMapFile == nullptr)
		{
			// _tprintf(TEXT("Could not open file mapping object (%d).\n"), GetLastError());
			return false;
		}

		pBuf = (LPTSTR)MapViewOfFile(hMapFile, // handle to map object
			FILE_MAP_READ,  // read permission
			0,
			0,
			transferBufferSize);

		if (pBuf == nullptr)
		{
			// _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());

			CloseHandle(hMapFile);

			return false;
		}

	}
	memcpy(&rFactorVersion, pBuf, sizeof(pBuf));
	return true;
}

TelemetryDataRF::~TelemetryDataRF()
{
	disconnect();
}


bool TelemetryDataRF::read()
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

void TelemetryDataRF::disconnect()
{
	UnmapViewOfFile(pBuf); pBuf = NULL;
	CloseHandle(hMapFile);
	rFactorVersion = 0;
}

bool isReadyRFactor()
{
	HANDLE hMapFile;

	hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, transferBufferName);
	if (!hMapFile)
		return false;
	else
		CloseHandle(hMapFile);
	return true;
}