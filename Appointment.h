#pragma once
#include<iostream>
#include<string>
using namespace std;
class Appointment
{
protected:
	int id;
	int patientID;
	int doctorID;
	string date;
	string timeslot;
	string status;
public:
	Appointment();
	Appointment(int ID,int PatientID,int DoctorID ,string Date,string Timeslot,string Status );
	int getID() const;
	int getPatientID() const;
	int getDoctorID() const;
	string getDate() const;
	string getTimeslot() const;
	string getStatus() const;
	void setID(int ID);
	void setPatientID(int Pid);
	void setDoctorID(int Did);
	void setDate(string Date);
	void setTimeslot(string Timeslot);
	void setStatus(string Status);
	bool operator ==(const Appointment& obj) const;
	friend ostream& operator<<(ostream& out, const Appointment& obj);
};

