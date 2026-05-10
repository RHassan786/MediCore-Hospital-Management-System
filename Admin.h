#pragma once
#include"person.h"
#include<iostream>
#include<string>
using namespace std;
class Admin: public person
{
protected:
	int id;
public:
	Admin();
	Admin(string Password, string Name, string Contact, int ID);
	int getID() const;
	void setID(int ID);
	virtual void displayInfo() override;
};

