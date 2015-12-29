#pragma once

#include "stdafx.h"

class TelemetryDataRF {

public:
	UnifiedRfData data;
	long rFactorVersion;

	~TelemetryDataRF();

	bool connect();
	bool read();
	void disconnect();
};
