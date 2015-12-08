#pragma once

#include "stdafx.h"

#define MAX_ELEMENTS 1000 // Size of lookup table for SimDataElementsManager

// Track and provide lookup access to a list of SimDataElements
class SimDataElementsManager
{
public:
	SimDataElement * lookup(int id);
	void registerElement(SimDataElement *);
	void reset() { counter = 0; };
	std::wstring * jsonElements();
private:
	int counter = 0;
	SimDataElement * elements[MAX_ELEMENTS];
};

