#include "Validator.h"
#include "Storage.h"
#include<iostream>
#include<string.h>

using namespace std;
bool Validator::isPasswordValid(string password)
{
	if (password.length() < 6)
	{
		return false;
	}
	else
		return true;
}

bool Validator::isPositiveNumber(double n)
{
	return n > 0;
}
bool Validator::isMenuChoiceValid(int choice,int min,int max)
{
	if (choice >= min && choice <= max)
	{
		return true;
	}
	else return false;
}

bool Validator::isTimeSlotValid(string Timeslot)
{
	if (Timeslot == "09:00" || Timeslot == "10:00" || Timeslot == "11:00" || Timeslot == "12:00" || Timeslot == "13:00" || Timeslot == "14:00" || Timeslot == "15:00" || Timeslot == "16:00")
	{
		return true;
	}
	else
		return false;
}

bool Validator::isContactNumberValid(string Contact)
{
	if (Contact.length() != 11)
	{
		return false;
	}
	else
	{ 
		for (int i = 0; i < 11; i++)
		{
			if (Contact[i] < '0' || Contact[i] > '9')
			{
				return false;
			}
		}
	}
	return true;
}

bool Validator::isDateValid(string Date)
{
	if (Date.length() != 10)
	{
		return false;
	}
	else
	{
		if (Date[0] < '0' || Date[0]>'3')
		{
			return false;
		}
		if (Date[1] < '0' || Date[1]>'9'|| (Date[0]=='0' && Date[1] == '0')|| (Date[0] == '3' && Date[1] > '1'))
		{
			return false;
		}
		if (Date[2] != '-')
		{
			return false;
		}
		if (Date[3] < '0' || Date[3]>'1')
		{
			return false;
		}
		if (Date[4] < '0' || Date[4]>'9' || (Date[3] == '0' && Date[4] == '0')|| (Date[3] == '1' && Date[4] > '2'))
		{
			return false;
		}
		if (Date[5] != '-')
		{
			return false;
		}
		if (Date[6] != '2' || Date[7] != '0' || Date[8] != '2')
		{
			return false;
		}
		if (Date[9]<'6' || Date[9]>'7')
		{
			return false;
		}
	}
	return true;
}