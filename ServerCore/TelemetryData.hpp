#pragma once

#include "stdafx.h"

class TelemetryData {

public:
	UnifiedRfData data;
	long rFactorVersion;

	~TelemetryData();

	bool connect();
	bool read();
	void disconnect();
};
