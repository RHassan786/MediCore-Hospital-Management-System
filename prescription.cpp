#include "prescription.h"
#include<iostream>
#include<string>
using namespace std;

prescription::prescription():id(0),appointmentID(0),doctorID(0),patientID(0),date(""),medicine(""),notes("")
{

}
prescription::prescription(int ID, int Aid, int Did, int Pid, string Date, string Medicine, string Notes):
	id(ID),appointmentID(Aid),doctorID(Did),patientID(Pid),date(Date),medicine(Medicine),notes(Notes)
{

}
int prescription::getID() const
{
	return id;
}
int prescription::getPatientID() const
{
	return patientID;
}
int prescription::getDoctorID() const
{
	return doctorID;
}
int prescription::getAppointmentID() const
{
	return appointmentID;
}
string prescription::getDate() const
{
	return date;
}
string prescription::getMedicine()const
{
	return medicine;
}
string prescription::getNotes()const
{
	return notes;
}
void prescription::setID(int ID)
{
	id = ID;
}
void prescription::setPatientID(int Pid)
{
	patientID = Pid;
}
void prescription::setDoctorID(int Did)
{
	doctorID = Did;
}
void prescription::setAppointmentID(int Aid)
{
	appointmentID = Aid;
}
void prescription::setDate(string Date)
{
	date = Date;
}
void prescription::setMedicine(string Medicine)
{
	medicine = Medicine;
}
void prescription::setNotes(string Notes)
{
	notes = Notes;
}
ostream& operator<<(ostream& out, const prescription& obj)
{
	out << "Prescription Details: ";
	out << endl << "Prescription ID: " << obj.id;
	out << endl << "Appointment ID: " << obj.appointmentID;
	out << endl << "Patient ID: " << obj.patientID;
	out << endl << "DoctorID: " << obj.doctorID;
	out << endl << "Date: " << obj.date;
	out << endl << "Medicine: " << obj.medicine;
	out << endl << "Doctor Notes: " << obj.notes;
	return out;
}