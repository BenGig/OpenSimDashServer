#pragma once

#include "stdafx.h"

#define MAX_ELEMENTS 1000 // Size of lookup table for SimDataElementsManager

// Track and provide lookup access to a list of SimDataElements
class SimDataElementsManager
{
public:
	SimDataElement * Lookup(int id);
	void Register(SimDataElement *);
	void Reset() { counter = 0; };
	std::wstring * JsonElements();
private:
	int counter = 0;
	SimDataElement * elements[MAX_ELEMENTS];
};

