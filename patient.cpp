#include "patient.h"
#include"person.h"
#include<iostream>
#include<string>
using namespace std;
patient::patient():person("", "", ""), id(0), age(0), gender(""), balance(0) {}

patient::patient(string Password, string Name, string Contact, int ID, int Age, string Gender, double Balance):
person(Password,Name,Contact),id(ID),age(Age),gender(Gender),balance(Balance)
{
}

int patient::getID() const 
{
	return id;
}
int patient::getAge() const
{
	return age;
}
string patient::getGender() const
{
	return gender;
}
double patient::getBalance() const
{
	return balance;
}
void patient::setID(int ID)
{
	id = ID;
}
void patient::setAge(int Age)
{
	age = Age;
}
void patient::setGender(string Gender)
{
	gender = Gender;
}
void patient::setBalance(double Balance)
{
	balance = Balance;
}

patient& patient::operator += (double Amount)
{
	balance += Amount;
	return *this;
}
patient& patient::operator -= (double Amount)
{
	balance -= Amount;
	return *this;
}

bool patient::operator == (const patient& obj) const
{
	if (id == obj.id)
	{
		return true;
	}
	else
		return false;
}

void patient::displayInfo()
{
	//id,name,age,gender,password,balance,contact
	cout << *this;
}

ostream& operator <<(ostream& out, const patient& obj)
{
	out << "Details of the Patient: ";
	out << endl << "Patient ID:" << obj.getID();
	out << "| Patient Name: " << obj.getName();
	out << "| Patient Age: " << obj.getAge();
	out << "| Patient gender: " << obj.getGender();
	out << "| Patient balance: " << obj.getBalance();
	out << "| Patient contact: " << obj.getContact();
	return out;
}