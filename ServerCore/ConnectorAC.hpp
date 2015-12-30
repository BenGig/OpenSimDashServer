#pragma once

#include "stdafx.h"

class ConnectorAC : public Connector
{
public:
	std::wstring acVersion;

	ConnectorAC() {};
	~ConnectorAC() {};

	bool connect();
	int check();
	std::wstring * simName();
	bool read();
	void disconnect();


private:
	TelemetryDataAC td;
	void sessionToStr();
};
