#pragma once

#include "stdafx.h"

#define STRLEN 64	// current maximum length of sim provided strings

class SimDataElement
{
public:
	//SimDataElement() : label(std::wstring(L"unnamed")) { Register(); };
//	SimDataElement();

	std::wstring label;
	
	void Register();
	bool isRegistered;

	int Id() { return id; };
	void SetId(int i) { id = i; };
	virtual std::wstring String() { return L""; };
	virtual std::wstring Json();
private:
	int id;
};

class SimDataNumber : public SimDataElement
{
public:
	std::wstring Json();
};

class SimDataBool : public SimDataElement
{
public:
	SimDataBool();
	SimDataBool(std::wstring label, bool);

	bool bl;

	std::wstring String();
	std::wstring Json();
};

class SimDataInteger : public SimDataNumber
{
public:
	SimDataInteger();
	SimDataInteger(std::wstring, long);

	long lint;

	std::wstring String();
};

// Floating point values
class SimDataFloating : public SimDataNumber
{
public:
	SimDataFloating();
	SimDataFloating(std::wstring label, double);

	double flt;

	std::wstring String();
};


// Time, a floating number with particular output
class SimDataTime : public SimDataFloating
{
public:
	SimDataTime() {};
	SimDataTime(std::wstring label, double);

	double flt;
	std::wstring String();
	std::wstring Json();
};


// String elements
class SimDataString : public SimDataElement
{
public:
	SimDataString();
	SimDataString(std::wstring, std::wstring);

	std::wstring str;

	std::wstring String();
	std::wstring Json();
};

// Vector: contains 3 floating numbers
class SimDataVector : public SimDataElement
{
public:
	double x, y, z;

	SimDataVector() {label = std::wstring(L"unnamed");};
	SimDataVector(std::wstring name);
	SimDataVector(std::wstring name, double x, double y, double z);

	std::wstring Json();

	SimDataVector Add(SimDataVector, std::wstring resultName); // Vector addition
	SimDataVector Mul(double factor, std::wstring resultName); // Multiplication with scalar
	SimDataVector Cross(SimDataVector, std::wstring resultName); // Cross product
	double Scalar(SimDataVector v); // Scalar product; dot product
	double Length(); 
};

