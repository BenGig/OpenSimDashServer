#pragma once

#include "stdafx.h"

bool isReadyRFactor();

class TelemetryDataRF {

public:
	UnifiedRfData data;
	long rFactorVersion;

	~TelemetryDataRF();

	bool connect();
	bool read();
	void disconnect();
};
