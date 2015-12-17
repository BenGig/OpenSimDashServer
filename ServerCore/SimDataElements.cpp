#pragma once

#include "stdafx.h"

SimDataElementsManager sdem;

SimDataElement::SimDataElement()
{
	label = std::wstring(L"unnamed");
}

SimDataElement::SimDataElement(std::wstring name)
{
	label = name;
}

void SimDataElement::registerMe()
{
	sdem.registerElement(this);
}

std::wstring SimDataElement::json()
{
	return std::wstring(L"\"") + label + std::wstring(L"\":");
}

// Bool type

SimDataBool::SimDataBool()
	: SimDataElement()
{
	bl = false;
}
SimDataBool::SimDataBool(std::wstring name, bool isTrue)
	: SimDataElement(name)
{
	bl = isTrue;
}

std::wstring SimDataBool::toString()
{
	if (bl)
	{
		return std::wstring(L"true");
	}
	else
	{
		return std::wstring(L"false");
	}
}

std::wstring SimDataBool::json()
{
	std::wstring buf = SimDataElement::json();

	if (bl)
	{
		buf.append(toString());
	}
	else
	{
		buf.append(toString());
	}
	return buf;
}

// Numbers

SimDataNumber::SimDataNumber(std::wstring name)
	: SimDataElement(name)
{
}

std::wstring SimDataNumber::json()
{
	return
		SimDataElement::json() +
		toString();
}

// Integer type (long)

SimDataInteger::SimDataInteger()
{
	lint = 0;
}
SimDataInteger::SimDataInteger(std::wstring name, long l)
	: SimDataNumber(name)
{
	lint = l;
}

std::wstring SimDataInteger::toString()
{
	return std::wstring(std::to_wstring(lint));
}
// Floating point type data

SimDataFloating::SimDataFloating()
	: SimDataNumber()
{
}


SimDataFloating::SimDataFloating(std::wstring name, double f)
	: SimDataNumber(name)
{
	flt = f;
}

std::wstring SimDataFloating::toString()
{
	return std::wstring(std::to_wstring(flt));
}

SimDataFloatingLimited::SimDataFloatingLimited()
	: SimDataFloating()
{
}
SimDataFloatingLimited::SimDataFloatingLimited(std::wstring name, double f, double limit)
	: SimDataFloating(name, f)
{
	max = limit;
}

std::wstring SimDataFloatingLimited::toString()
{
	return L"{\"cur\":" +
		std::to_wstring(flt) +
		L",\"max\":" +
		std::to_wstring(max) +
		L"}";
}

// Time, a floating number with particular output

SimDataTime::SimDataTime(std::wstring name, double f)
{
	label = name;
	flt = f;
}

std::wstring SimDataTime::toString()
{
	if (flt < .0)
		return std::wstring(L"0");
#pragma warning(push)
#pragma warning(disable:4244)
	long hours = flt / 3600;
	double no_hours = flt - (3600 * hours);
	int minutes = no_hours / 60;
#pragma warning(pop)
	double seconds_f = no_hours - (minutes * 60);

	if (hours > 0)
		if (minutes < 10)
			return std::wstring(std::to_wstring(hours) + L":0" + std::to_wstring(minutes) + L"" + std::to_wstring(seconds_f));
		else
			return std::wstring( std::to_wstring(hours) + L":" + std::to_wstring(minutes) + L"" + std::to_wstring(seconds_f));
	else
		if (minutes > 0)
			if (seconds_f < 10)
				return std::wstring(std::to_wstring(minutes) + L":0" + std::to_wstring(seconds_f));
			else
				return std::wstring(std::to_wstring(minutes) + L":" + std::to_wstring(seconds_f));
		else
			return std::wstring(std::to_wstring(seconds_f));
}

std::wstring SimDataTime::json()
{
	return SimDataElement::json() + std::wstring(L"\"") + toString() + std::wstring(L"\",\"") +
		std::wstring(label) + std::wstring(L"Raw\":") + std::to_wstring(flt);
}


// Strings

SimDataString::SimDataString()
{
	label = std::wstring(L"unnamed");
	str = std::wstring(L"");
}

SimDataString::SimDataString(std::wstring name, std::wstring s)
{
	label = name;
	str = s;
}

std::wstring SimDataString::toString()
{
	return str;
}

std::wstring SimDataString::json()
{
	return
		SimDataElement::json() +
		std::wstring(L"\"") +
		toString() +
		std::wstring(L"\"");
}

// Vector (x, y, z coordinates)

SimDataVector::SimDataVector(std::wstring name)
{
	label = name;
	x = 0; y = 0; z = 0;
}

SimDataVector::SimDataVector(std::wstring name, double xi, double yi, double zi)
{
	label = name;
	x = xi;
	y = yi;
	z = zi;
}

std::wstring SimDataVector::json()
{
	return
		SimDataElement::json() +
		std::wstring(L"[{\"x\":") + std::to_wstring(x) +
		std::wstring(L"},{\"y\":") + std::to_wstring(y) +
		std::wstring(L"},{\"z\":") + std::to_wstring(z) +
		std::wstring(L"}]");
}

SimDataVector SimDataVector::Add(SimDataVector v, std::wstring resultName)
{
	return SimDataVector(resultName,
		x + v.x,
		y + v.y,
		z + v.z);
}

// Must be disposed by "delete"
SimDataVector SimDataVector::Cross(SimDataVector v, std::wstring resultName)
{
	return SimDataVector(resultName,
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x);
}

SimDataVector SimDataVector::Mul(double factor, std::wstring resultName)
{
	return SimDataVector(resultName,
		x * factor,
		y * factor,
		z * factor);
}

double SimDataVector::scalar(SimDataVector v)
{
	return x*v.x + y*v.y + z*v.z;
}

double SimDataVector::length()
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

