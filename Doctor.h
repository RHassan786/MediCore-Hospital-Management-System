#pragma once
#include"person.h"
#include<iostream>
#include<string>
using namespace std;
class Doctor:public person
{
protected:
	int id;
	string specialization;
	double fee;
public:
	Doctor();
	Doctor(string Password, string Name, string Contact,int ID,string Specialization,double Fee);
	int getID() const;
	string getSpecialization() const;
	double getFee() const;
	void setID(int ID);
	void setSpecialization(string Specialization);
	void setFee(double Fee);
	virtual void displayInfo() override;
	bool operator == (const Doctor& obj) const;
	friend ostream& operator <<(ostream& out, const Doctor& obj);


};

