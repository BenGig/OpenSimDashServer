#pragma once

#include "stdafx.h"

// convert 8 bit strings to wstring
void convertFrom8bit(char * str, std::wstring * wstr);

class Connector
{
public:
	virtual std::wstring * simName() { return NULL; };
	virtual bool connect() { return false; };	/* Connect (and read) to a sim data provider */
	virtual int check() { return 0; };			/* Try connection, return version info or 0 if not available */
	virtual bool read();						/* Transfer data from sim internals to SimData elements */
	virtual void disconnect() { return; };

	Connector();
	~Connector();

	SimData * sd;

protected:
	bool slowReady();
	bool quickReady();
	void slowFetched();
	void quickFetched();
	std::chrono::seconds slowInterval;							// once per second
	std::chrono::milliseconds quickInterval;	// 33.3 times per second

private:
	std::chrono::steady_clock::time_point lastSlowUpdate;
	std::chrono::steady_clock::time_point lastQuickUpdate;

};


