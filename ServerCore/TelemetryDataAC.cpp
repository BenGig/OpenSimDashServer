#include "stdafx.h"
#include "TelemetryDataAC.hpp"

/*
struct SMElement
{
	HANDLE hMapFile;
	unsigned char* mapFileBuffer;
};
*/

void dismiss(SMElement element)
{
	UnmapViewOfFile(element.mapFileBuffer);
	CloseHandle(element.hMapFile);
}


// Data access object
TelemetryDataAC::~TelemetryDataAC()
{
	disconnect();
}

bool TelemetryDataAC::connect()
{
	TCHAR szNamePhysics[] = TEXT("Local\\acpmf_physics");
	m_physics.hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szNamePhysics);
	if (!m_physics.hMapFile)
		return false;
	m_physics.mapFileBuffer = (unsigned char*)MapViewOfFile(m_physics.hMapFile, FILE_MAP_READ, 0, 0, sizeof(SPageFilePhysics));
	if (!m_physics.mapFileBuffer)
		return false;

	TCHAR szNameGraphics[] = TEXT("Local\\acpmf_graphics");
	m_graphics.hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szNameGraphics);
	if (!m_graphics.hMapFile)
		return false;
	m_graphics.mapFileBuffer = (unsigned char*)MapViewOfFile(m_graphics.hMapFile, FILE_MAP_READ, 0, 0, sizeof(SPageFileGraphic));
	if (!m_graphics.mapFileBuffer)
		return false;

	TCHAR szNameStatic[] = TEXT("Local\\acpmf_static");
	m_static.hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szNameStatic);
	if (!m_static.hMapFile)
		return false;
	m_static.mapFileBuffer = (unsigned char*)MapViewOfFile(m_static.hMapFile, FILE_MAP_READ, 0, 0, sizeof(SPageFileStatic));
	if (!m_static.mapFileBuffer)
		return false;

	return true;
}

bool TelemetryDataAC::read()
{
	// just return true if mappings are ok
	if (m_physics.mapFileBuffer && m_graphics.hMapFile && m_static.hMapFile)
	{
		data.pfPhysics = (SPageFilePhysics*)m_physics.mapFileBuffer;
		data.pfGraphics = (SPageFileGraphic*)m_graphics.mapFileBuffer;
		data.pfStatic = (SPageFileStatic*)m_static.mapFileBuffer;
		return true;
	}
	
	else
		return false;
}

void TelemetryDataAC::disconnect()
{
	UnmapViewOfFile(m_graphics.mapFileBuffer);
	CloseHandle(m_graphics.hMapFile);
	UnmapViewOfFile(m_physics.mapFileBuffer);
	CloseHandle(m_physics.hMapFile);
	UnmapViewOfFile(m_static.mapFileBuffer);
	CloseHandle(m_static.hMapFile);
}
