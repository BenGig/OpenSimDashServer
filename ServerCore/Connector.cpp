#include "stdafx.h"

void convertFrom8bit(char * str, std::wstring * wstr) {
	wchar_t wbuf[64]; // max length rFactor provides
	MultiByteToWideChar(CP_ACP, 0, str, -1, wbuf, 64);
	wstr->assign(wbuf);
}

bool Connector::read()
{
	return false;
}

Connector::Connector()
{
	slowInterval = std::chrono::seconds(1);
	quickInterval = std::chrono::milliseconds(33);
	lastSlowUpdate = std::chrono::steady_clock::now() - slowInterval;
	lastQuickUpdate = std::chrono::steady_clock::now() - quickInterval;
//	sd = new SimData();
}

Connector::~Connector()
{
	sdem.reset();
//	delete sd;
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

