#pragma once

#include "stdafx.h"

struct SMElement
{
	HANDLE hMapFile;
	unsigned char* mapFileBuffer;
};

struct ACdata {
	SPageFilePhysics* pfPhysics;
	SPageFileGraphic* pfGraphics;
	SPageFileStatic* pfStatic;
};

class TelemetryDataAC {

public:
	ACdata data;

	~TelemetryDataAC();

	bool connect();
	bool read();
	void disconnect();

private:
	SMElement m_graphics;
	SMElement m_physics;
	SMElement m_static;
};
