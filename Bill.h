#pragma once
#include<iostream>
#include<string>
using namespace std;
class Bill
{
protected:
	int id;
	int patientID;
	int appointmentID;
	double amount;
	string status;
	string date;
public:
	Bill();
	Bill(int ID,int Pid,int Aid,double Amount,string Status,string Date);
	int getID() const;
	int getAppointmentID() const;
	int getPatientID() const;
	string getDate() const;
	string getStatus() const;
	double getAmount() const;

	void setID(int ID);
	void setAppointmentID(int Aid) ;
	void setPatientID(int Pid) ;
	void setDate(string Date) ;
	void setStatus(string Status) ;
	void setAmount(double Amount) ;
	friend ostream& operator<<(ostream& out, const Bill& obj);
	

};

