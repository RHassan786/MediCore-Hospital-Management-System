#pragma once
#include<iostream>
#include<string>
using namespace std;
class HospitalException {
public:
	virtual string what()
	{
		return "General Hospital Error!";
	}

	virtual ~HospitalException()
	{
	}
};

class FileNotFoundException : public HospitalException {
private:
	string message;
public:
	FileNotFoundException(string filename) : message("Error: Cannot open file: " + filename) {}
	string what() override {
		return message;
	}
};

class InsufficientFundsException :public HospitalException
{
public:

	string what() override
	{
		return "Insufficient Funds!";
	}
};

class InvalidInputException :public HospitalException
{
	string message;
public:
	InvalidInputException(string msg) : message(msg)
	{

	}
	string what()override
	{
		return message;
	}
};

class SlotUnavailableException :public HospitalException
{
public:

	string what() override
	{
		return "Slot Unavailable!";
	}
};
