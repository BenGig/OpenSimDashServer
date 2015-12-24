#pragma once

#include "stdafx.h"

#define STRLEN 64	// current maximum length of sim provided strings

std::wstring timeToString(double, bool, bool);

class SimDataElement
{
public:
	SimDataElement(std::wstring name);
	SimDataElement();

	std::wstring label;
	
	void registerMe();
	bool isRegistered;

	int Id() { return id; };
	void setId(int i) { id = i; };
	virtual std::wstring toString() { return L""; };
	virtual std::wstring json();
private:
	int id;
};

class SimDataNumber : public SimDataElement
{
public:
	SimDataNumber() {};
	SimDataNumber(std::wstring name);

	std::wstring json();
};

class SimDataBool : public SimDataElement
{
public:
	SimDataBool();
	SimDataBool(std::wstring label, bool);

	bool bl;

	std::wstring toString();
	std::wstring json();
};

class SimDataInteger : public SimDataNumber
{
public:
	SimDataInteger();
	SimDataInteger(std::wstring, long);

	long lint;

	std::wstring toString();
};

// Floating point values
class SimDataFloating : public SimDataNumber
{
public:
	SimDataFloating();
	SimDataFloating(std::wstring label, double);

	double flt;

	std::wstring toString();
};

// Floating points with a maximum value (i.e. RPM, fuel)
class SimDataFloatingLimited : public SimDataFloating
{
public:
	SimDataFloatingLimited();
	SimDataFloatingLimited(std::wstring label, double value, double limit);

	double flt, max;

	std::wstring toString();
};

// Time, a floating number with particular output
class SimDataTime : public SimDataFloating
{
public:
	SimDataTime() {};
	SimDataTime(std::wstring label, double);
	SimDataTime(std::wstring label, double, bool omitLeadingZeros);
	SimDataTime(std::wstring label, double, bool omitLeadingZeros, bool omitMillis);

	double flt;
	bool omitLeadingZeros;
	bool omitMillis;
	std::wstring toString();
	std::wstring json();
};


// String elements
class SimDataString : public SimDataElement
{
public:
	SimDataString();
	SimDataString(std::wstring, std::wstring);

	std::wstring str;

	std::wstring toString();
	std::wstring json();
};

// Vector: contains 3 floating numbers
class SimDataVector : public SimDataElement
{
public:
	double x, y, z;

	SimDataVector() {label = std::wstring(L"unnamed");};
	SimDataVector(std::wstring name);
	SimDataVector(std::wstring name, double x, double y, double z);

	std::wstring json();

	SimDataVector Add(SimDataVector, std::wstring resultName); // Vector addition
	SimDataVector Mul(double factor, std::wstring resultName); // Multiplication with scalar
	SimDataVector Cross(SimDataVector, std::wstring resultName); // Cross product
	double scalar(SimDataVector v); // Scalar product; dot product
	double length(); 
};

