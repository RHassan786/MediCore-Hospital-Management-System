#include "person.h"
#include<iostream>
using namespace std;

person::person() :password(""), name(""), contact("")
{

}

person::person(string Password, string Name, string Contact):password(Password),name(Name),contact(Contact)
{
}

string person::getName() const
{
	return name;
}
string person::getPassword() const
{
	return password;
}
string person::getContact() const
{
	return contact;
}
void  person::setName(string Name)
{
	name = Name;

}
void person::setPassword(string Password)
{
	password = Password;
}
void person::setContact(string Contact)
{
	contact = Contact;
}

person::~person()
{

}