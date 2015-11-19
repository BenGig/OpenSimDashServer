#include "stdafx.h"

bool Connector::Read()
{
	return false;
}

Connector::Connector()
{
	slowInterval = std::chrono::seconds(1);
	quickInterval = std::chrono::milliseconds(33);
	lastSlowUpdate = std::chrono::steady_clock::now() - slowInterval;
	lastQuickUpdate = std::chrono::steady_clock::now() - quickInterval;
}

bool Connector::slowReady()
{
	if (std::chrono::steady_clock::now() >= lastSlowUpdate + slowInterval)
		return true;
	else
		return false;
}

bool Connector::quickReady()
{
	if (std::chrono::steady_clock::now() >= lastQuickUpdate + quickInterval)
		return true;
	else
		return false;
}

void Connector::slowFetched()
{
	lastSlowUpdate = std::chrono::steady_clock::now();
}

void Connector::quickFetched()
{
	lastQuickUpdate = std::chrono::steady_clock::now();
}

