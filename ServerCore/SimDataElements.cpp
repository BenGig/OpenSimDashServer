#pragma once

#include "stdafx.h"

SimDataElementsManager sdem;

void SimDataElement::Register()
{
	sdem.Register(this);
}

std::wstring SimDataElement::Json()
{
	return std::wstring(L"\"") + label + std::wstring(L"\":");
}

// Bool type

SimDataBool::SimDataBool()
{
	label = std::wstring(L"unnamed");
	bl = false;
}
SimDataBool::SimDataBool(std::wstring name, bool isTrue)
{
	label = name;
	bl = isTrue;
}

std::wstring SimDataBool::String()
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

std::wstring SimDataBool::Json()
{
	std::wstring buf = SimDataElement::Json();

	if (bl)
	{
		buf.append(String());
	}
	else
	{
		buf.append(String());
	}
	return buf;
}

std::wstring SimDataNumber::Json()
{
	return
		SimDataElement::Json() +
		String();
}

// Integer type (long)

SimDataInteger::SimDataInteger()
{
	lint = 0;
	label = std::wstring(L"unnamed");
}
SimDataInteger::SimDataInteger(std::wstring name, long l)
{
	lint = l;
	label = name;
}

std::wstring SimDataInteger::String()
{
	return std::wstring(std::to_wstring(lint));
}
// Floating point type data

SimDataFloating::SimDataFloating()
{
	label = L"unnamed";
	flt = 0;
}


SimDataFloating::SimDataFloating(std::wstring name, double f)
{
	label = name;
	flt = f;
}

std::wstring SimDataFloating::String()
{
	return std::wstring(std::to_wstring(flt));
}



// Time, a floating number with particular output

SimDataTime::SimDataTime(std::wstring name, double f)
{
	label = name;
	flt = f;
}

std::wstring SimDataTime::String()
{
	if (flt == -1.0)
		return std::wstring(L"N/A");
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

std::wstring SimDataTime::Json()
{
	return SimDataElement::Json() + std::wstring(L"\"") + String() + std::wstring(L"\"");
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

std::wstring SimDataString::String()
{
	return str;
}

std::wstring SimDataString::Json()
{
	return
		SimDataElement::Json() +
		std::wstring(L"\"") +
		String() +
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

std::wstring SimDataVector::Json()
{
	return
		SimDataElement::Json() +
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

double SimDataVector::Scalar(SimDataVector v)
{
	return x*v.x + y*v.y + z*v.z;
}

double SimDataVector::Length()
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

