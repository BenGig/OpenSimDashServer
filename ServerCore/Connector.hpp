#pragma once

#include "stdafx.h"

class Connector
{
public:
	std::wstring sim;
	virtual bool Connect() { return false; };	/* Connect (and read) to a sim data provider */
	virtual int Check() { return 0; };			/* Try connection, return version info or 0 if not available */
	virtual bool Read();						/* Transfer data from sim internals to SimData elements */
	virtual void Disconnect() { return; };

	Connector();

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


