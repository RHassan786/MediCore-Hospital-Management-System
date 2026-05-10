#pragma once
#include<string>
using namespace std;
class person
{
protected:
	
	string password;
	string name;
    string contact;
public:
	person();
	person(string Password, string Name , string Contact);
	string getName() const;
	string getPassword() const;
	string getContact() const;
     void  setName(string Name);
	 void setPassword(string Password);
	 void setContact(string Contact);
	virtual void displayInfo() = 0;
	/*virtual void displayMenu() = 0;*/

	virtual ~person();
};

