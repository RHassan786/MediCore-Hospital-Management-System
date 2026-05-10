#include "Bill.h"
#include "HospitalException.h"
#include<iostream>
#include<string>
using namespace std;

Bill::Bill():id(0),patientID(0),appointmentID(0),amount(0),status("unpaid"),date("")
{
}
Bill::Bill(int ID, int Pid, int Aid, double Amount, string Status, string Date):
	id(ID),patientID(Pid),appointmentID(Aid),amount(Amount),date(Date)
{
	setStatus(Status);
}

int Bill::getID() const
{
	return id;
}
int Bill::getAppointmentID() const
{
	return appointmentID;
}
int Bill::getPatientID() const
{
	return patientID;
}
string Bill::getDate() const
{ 
return date;
}
string Bill::getStatus() const
{
	return status;
}
double Bill::getAmount() const
{
	return amount;
}
void Bill::setID(int ID) 
{
	id = ID;
}
void Bill::setAppointmentID(int Aid) 
{
	appointmentID = Aid;
}
void Bill::setPatientID(int Pid) 
{
	patientID = Pid;
}
void Bill::setDate(string Date) 
{
	date = Date;
}
void Bill::setStatus(string Status) 
{
	if (Status == "paid" || Status == "unpaid" || Status == "cancelled")
	{
		status = Status;
	}
	else
		try
		{
			throw InvalidInputException("Invalid Input. Status can only be paid, unpaid or cancelled");
	    }
		catch (InvalidInputException& I)
		{
			cout << I.what();
		}
	
}
void Bill::setAmount(double Amount) 
{
	amount = Amount;
}
ostream& operator<<(ostream& out, const Bill& obj)
{
	out << "Bill Details: ";
	out << endl << "Bill ID: " << obj.id;
	out << "| " << "Patient ID: " << obj.patientID;
	out << "| " << "Appointment ID:" << obj.appointmentID;
	out << "| " << "Amount: " << obj.amount;
	out << "| " << "Status: " << obj.status;
	out << "| " << "Date: " << obj.date;
	return out;
	
}