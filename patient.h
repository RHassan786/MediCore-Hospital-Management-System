#pragma once
#include"person.h"
#include<iostream>
#include<string>
using namespace std;
class patient: public person
{
protected:
	int id;
	int age;
	string gender;
	double balance;
public:
	patient();
	patient(string Password, string Name, string Contact, int ID, int Age, string Gender, double Balance);
	int getID() const;
	int getAge() const;
	string getGender() const ;
	double getBalance() const ;
	void setID(int ID);
	void setAge(int Age);
	void setGender(string Gender);
	void setBalance(double Balance);
	virtual void displayInfo() override;
    patient& operator += (double Amount);
	patient& operator -= (double Amount);
	bool operator == (const patient& obj) const;
	friend ostream& operator <<(ostream &out,const patient& obj);

};

