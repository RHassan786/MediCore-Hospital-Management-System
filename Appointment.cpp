#include "Appointment.h"

#include<iostream>
#include<string>
#include "HospitalException.h"
using namespace std;
Appointment::Appointment() :id(0), patientID(0), doctorID(0), date(""), timeslot(""), status("pending")
{
}

Appointment::Appointment(int ID, int PatientID, int DoctorID, string Date, string Timeslot, string Status):
	id(ID), patientID(PatientID), doctorID(DoctorID), date(Date), timeslot(Timeslot)
{
	setStatus(Status);
}
int Appointment::getID() const
{
	return id;
}
int Appointment::getPatientID() const
{
	return patientID;
}
int Appointment::getDoctorID() const
{
	return doctorID;
}
string Appointment::getDate() const
{
	return date;
}
string Appointment::getTimeslot() const
{
	return timeslot;
}
string Appointment::getStatus() const
{
	return status;
}
void Appointment::setID(int ID)
{
	id = ID;
}
void Appointment::setPatientID(int Pid)
{
	patientID = Pid;
}
void Appointment::setDoctorID(int Did)
{
	doctorID = Did;
}
void Appointment::setDate(string Date)
{
	date = Date;
}
void Appointment::setTimeslot(string Timeslot)
{
	timeslot = Timeslot;
}
void Appointment::setStatus(string Status)
{
	if (Status == "cancelled" || Status == "pending" || Status == "completed" || Status == "noshow")
	{
		status = Status;
	}
	//else add exception here
	else
	{
		try
		{
			throw InvalidInputException("Invalid Status.");
		}
		catch (InvalidInputException& I)
		{
			cout << endl << I.what();
		}
	}

}
bool Appointment::operator ==(const Appointment& obj) const
{
	if (doctorID == obj.doctorID && date == obj.date && timeslot == obj.timeslot && status != "cancelled" && obj.status != "cancelled")
	{
		return true;
	}
	else
		return false;
}

ostream& operator<<(ostream& out, const Appointment& obj)
{
	out << "Appointment Details: ";
	out << endl << "Appointment ID:" << obj.id;
	out << "| Patient ID: " << obj.patientID;
	out << "| Doctor ID: " << obj.doctorID;
	out << "| Date: " << obj.date;
	out << "| Timeslot: " << obj.timeslot;
	out << "| Status: " << obj.status;
	return out;
}