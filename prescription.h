#pragma once
#include<iostream>
#include<string>
using namespace std;

class prescription
{
protected:
	int id;
	int appointmentID;
	int doctorID;
	int patientID;
	string date;
	string medicine;
	string notes;

public:
	prescription();
	prescription(int ID, int Aid, int Did, int Pid, string Date, string Medicine, string Notes);
	int getID() const;
	int getPatientID() const;
	int getDoctorID() const;
	int getAppointmentID() const;
	string getDate() const;
	string getMedicine()const;
	string getNotes()const;
	void setID(int ID);
	void setPatientID(int Pid);
	void setDoctorID(int Did);
	void setAppointmentID(int Aid);
	void setDate(string Date);
	void setMedicine(string Medicine);
	void setNotes(string Notes);
	friend ostream& operator<<(ostream& out, const prescription& obj);
};

