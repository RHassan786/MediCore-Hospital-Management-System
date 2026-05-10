#pragma once
#include "patient.h"
#include "Doctor.h"
#include "Storage.h"
#include<iostream>
#include<string>
using namespace std;
class Validator
{
public:
	static bool isPasswordValid(string password);
	static bool isPositiveNumber(double n);
	static bool isMenuChoiceValid(int n,int min,int max);
	static bool isTimeSlotValid(string Timeslot);
	static bool isContactNumberValid(string Contact);
	template<class T>
	static bool isValidID(int id, Storage<T>& storage) 
	{
		return storage.findByID(id) != nullptr;
	}
	static bool isDateValid(string Date);


	
};

