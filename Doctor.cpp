#include "Doctor.h"
#include"person.h"
#include<iostream>
#include<string>
using namespace std;

Doctor::Doctor() :person("", "", ""), id(0), specialization(""), fee(0)
{
}

Doctor::Doctor(string Password, string Name, string Contact, int ID, string Specialization, double Fee):
person(Password,Name,Contact),id(ID),specialization(Specialization),fee(Fee)
{
}

int Doctor::getID() const
{
	return id;
}

string Doctor::getSpecialization() const
{
	return specialization;
}
double Doctor::getFee() const
{
	return fee;
}
void Doctor::setID(int ID)
{
	id = ID;
}
void Doctor::setSpecialization(string Specialization)
{
	specialization = Specialization;
}
void Doctor::setFee(double Fee)
{
	fee = Fee;
}

void Doctor::displayInfo()
{
	cout << *this;
}

bool Doctor::operator == (const Doctor& obj) const
{
	if (id == obj.id)
	{
		return true;
	}
	else
		return false;
}
ostream& operator <<(ostream& out, const Doctor& obj)
{
	out << "Details of the Doctor: ";
	out << endl << "Doctor ID: " << obj.getID();
	out << "| Doctor Name: " << obj.getName();
	out << "| Doctor Specialization: " << obj.getSpecialization();
	out << "| Doctor Contact: " << obj.getContact();
	out << "| Doctor Fee : " << obj.getFee();
	return out;
}
