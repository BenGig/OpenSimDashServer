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

// Formatter from floating point time to string
std::wstring timeToString(double time, bool omitLeadingZeros, bool omitMillis)
{
	if (time < 0)
		time = 0;
#pragma warning(push)
#pragma warning(disable:4244)
	long hours = time / 3600;
	double num_hours = time - (3600 * hours);
	int minutes = num_hours / 60;
	double seconds_f = num_hours - (minutes * 60);
	int seconds;
	if (omitMillis)
		seconds = round(seconds_f);
	else
		seconds = trunc(seconds_f);
#pragma warning(pop)

	wchar_t buf[30];
	if ((hours > 0 && minutes > 0) || ! omitLeadingZeros)
		swprintf(buf, sizeof(buf), L"%d:%02d:%02d", hours, minutes, seconds);
	else if (hours == 0 && minutes > 0)
		swprintf(buf, sizeof(buf), L"%2d:%02d", minutes, seconds);
	else
		swprintf(buf, sizeof(buf), L"%d", seconds);
	std::wstring str = std::wstring(buf);
	if (!omitMillis)
	{
		std::wstring millis = std::to_wstring(seconds_f - trunc(seconds_f));
		str.append(millis.erase(0, 1));
		str.erase(str.find_first_of(L".", 0)+4, str.length());
	}
	return str;
}

SimDataTime::SimDataTime(std::wstring name, double f)
{
	label = name;
	flt = f;
	str = new std::wstring(L"");
	omitLeadingZeros = true;
	omitMillis = false;
}
SimDataTime::SimDataTime(std::wstring name, double f, bool omitZeros)
{
	label = name;
	flt = f;
	str = new std::wstring(L"");
	omitLeadingZeros = omitZeros;
	omitMillis = false;
}

SimDataTime::SimDataTime(std::wstring name, double f, bool omitZeros, bool omitMilliseconds)
{
	label = name;
	flt = f;
	str = new std::wstring(L"");
	omitLeadingZeros = omitZeros;
	omitMillis = omitMilliseconds;
}

std::wstring SimDataTime::toString()
{
	if (str->length() > 0)
		return * str;
	else
		return timeToString(flt, omitLeadingZeros, omitMillis);
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

