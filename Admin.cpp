#include "Admin.h"
#include"person.h"
#include<iostream>
#include<string>
using namespace std;
Admin::Admin():person("", "", ""), id(0)
{

}
Admin::Admin(string Password, string Name, string Contact, int ID): person(Password,Name,Contact),id(ID)
{

}
int Admin::getID() const
{
	return id;
}
void Admin::setID(int ID)
{
	id = ID;
}
void Admin::displayInfo() 
{
	cout << "Details of Admin: ";
	cout << endl << "Admin ID: " << id;
	cout << endl << "Admin Name: " << name;
	cout << endl << "Admin Password: " << password;
	cout << endl << "Admin Contact: " << contact;
}