#pragma once
#include<fstream>
#include "Storage.h"
#include "prescription.h"
#include "Appointment.h"
#include "Bill.h"
#include "Admin.h"
#include "patient.h"
#include "Doctor.h"
#include<iostream>
#include<string>
using namespace std;
class FileHandler
{
public:
	void loadPatients(Storage<patient>& storage);
	void loadDoctors(Storage<Doctor>& storage);
	void loadAdmin(Storage<Admin>& storage);
	void loadAppointments(Storage<Appointment>& storage);
	void loadBills(Storage<Bill>& storage);
	void loadPrescriptions(Storage<prescription>& storage);

	void appendPatient(patient& p);
	void appendDoctor(Doctor& d);
	void appendAppointment(Appointment& a);
	void appendBill(Bill& b);
	void appendPrescription(prescription& p);
	void appendAdmin(Admin& a);

	void updatePatients(Storage<patient>&);
	void updateDoctors(Storage<Doctor>&);
	void updateAppointments(Storage<Appointment>&);
    void updateBills(Storage<Bill>&);
	void updatePrescriptions(Storage<prescription>&);
	void dischargePatient(int PID, Storage<patient>& patients,
		Storage<Appointment>& appointment,
		Storage<Bill>& bill,
		Storage<prescription>& Prescription);
	void ViewSecurityLog();
	void WriteToSecurityLog(string role, int enteredID);
};


