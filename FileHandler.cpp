#include<fstream>
#include "FileHandler.h"
#include "Storage.h"
#include "prescription.h"
#include"HospitalException.h"
#include "Bill.h"
#include "Admin.h"
#include "patient.h"
#include "Doctor.h"
#include "ctime"
#include<iostream>
#include<string>
using namespace std;

int findComma(int Sindex, string line) {
	for (int i = Sindex; i < line.length(); i++) {
		if (line[i] == ',') {
			return i;  
		}
	}
	return line.length(); 
}
string findSubString(int Sindex, int Lindex, string line) {
	string sub = "";
	for (int i = Sindex; i < Lindex; i++) 
	{  
		sub += line[i];
	}
	return sub;
}



//1,Ahmed Ali,25,M,03001234567,pass123,5000.00
//string Password, string Name, string Contact, int ID, int Age, string Gender, double Balance
void FileHandler:: loadPatients(Storage<patient>& storage)
{
	ifstream file("data/patients.txt");
	if (!file.is_open()) {
		throw FileNotFoundException("patients.txt");
	}

	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;

		string fields[7];
		int ind = 0;
		for (int i = 0; i < 7; i++) {
			int commaPos = findComma(ind, line);
			fields[i] = findSubString(ind, commaPos, line);
			ind = commaPos + 1;
		}
		patient p;
		p.setID(stoi(fields[0]));
		p.setName(fields[1]);
		p.setAge(stoi(fields[2]));
		p.setGender(fields[3]);
		p.setContact(fields[4]);
		p.setPassword(fields[5]);
		p.setBalance(stod(fields[6]));
		storage.Add(p);
	}
		file.close();
}


//1, Dr.Ali Hassan, Cardiology, 03211234567, doc123, 1500.00
void FileHandler::loadDoctors(Storage<Doctor>& storage)
{
	ifstream file("data/doctors.txt");
	if (!file.is_open()) {
		throw FileNotFoundException("doctors.txt");
	}

	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;

		string fields[6];
		int ind = 0;
		for (int i = 0; i < 6; i++) {
			int commaPos = findComma(ind, line);
			fields[i] = findSubString(ind, commaPos, line);
			ind = commaPos + 1;
		}

		Doctor d;
		d.setID(stoi(fields[0]));
		d.setName(fields[1]);
		d.setSpecialization(fields[2]);
		d.setContact(fields[3]);
		d.setPassword(fields[4]);
		d.setFee(stod(fields[5]));
		storage.Add(d);
	}
	file.close();
}
//1, Admin, admin123,03334026526
void FileHandler::loadAdmin(Storage<Admin>& storage)
{
	ifstream file("data/admin.txt");
	if (!file.is_open()) {
		throw FileNotFoundException("admin.txt");
	}

	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;

		string fields[4];
		int ind = 0;
		for (int i = 0; i < 4; i++) {
			int commaPos = findComma(ind, line);
			fields[i] = findSubString(ind, commaPos, line);
			ind = commaPos + 1;
		}

		Admin a;
		a.setID(stoi(fields[0]));
		a.setName(fields[1]);
		a.setPassword(fields[2]);
		a.setContact(fields[3]);
		storage.Add(a);

	}
	file.close();
}

//1,1,1,15-04-2026,09:00,completed
//int ID,int PatientID,int DoctorID ,string Date,string Timeslot,string Status
void FileHandler::loadAppointments(Storage<Appointment>&storage)
{
		ifstream file("data/appointments.txt");
		if (!file.is_open()) {
			throw FileNotFoundException("appointments.txt");
		}

		string line;
		while (getline(file, line)) {
			if (line.empty()) continue;

			string fields[6];
			int ind = 0;
			for (int i = 0; i < 6; i++) {
				int commaPos = findComma(ind, line);
				fields[i] = findSubString(ind, commaPos, line);
				ind = commaPos + 1;
			}

			Appointment A;
			A.setID(stoi(fields[0]));
			A.setPatientID(stoi(fields[1]));
			A.setDoctorID(stoi(fields[2]));
			A.setDate(fields[3]);
			A.setTimeslot(fields[4]);
			A.setStatus(fields[5]);

			storage.Add(A);

		}
		file.close();
	}
//1,1,1,1500.00,paid,15-04-2026
//int ID,int Pid,int Aid,double Amount,string Status,string Date
	void FileHandler::loadBills(Storage<Bill>& storage)
	{
		ifstream file("data/bills.txt");
		if (!file.is_open()) {
			throw FileNotFoundException("bills.txt");
		}

		string line;
		while (getline(file, line)) {
			if (line.empty()) continue;

			string fields[6];
			int ind = 0;
			for (int i = 0; i < 6; i++) {
				int commaPos = findComma(ind, line);
				fields[i] = findSubString(ind, commaPos, line);
				ind = commaPos + 1;
			}

			Bill b;
			b.setID(stoi(fields[0]));
			b.setPatientID(stoi(fields[1]));
			b.setAppointmentID(stoi(fields[2]));
			b.setAmount(stod(fields[3]));
			b.setStatus(fields[4]);
			b.setDate(fields[5]);

			storage.Add(b);

		}
		file.close();
	}
	//1,1,1,1,15-04-2026,Paracetamol 500mg,Take after meals
	//int ID, int Aid, int Did, int Pid, string Date, string Medicine, string Notes
	void FileHandler::loadPrescriptions(Storage<prescription>& storage)
	{

		ifstream file("data/prescriptions.txt");
		if (!file.is_open()) {
			throw FileNotFoundException("prescriptions.txt");
		}

		string line;
		while (getline(file, line)) {
			if (line.empty()) continue;

			string fields[7];
			int ind = 0;
			for (int i = 0; i < 7; i++) {
				int commaPos = findComma(ind, line);
				fields[i] = findSubString(ind, commaPos, line);
				ind = commaPos + 1;
			}

			prescription p;
			p.setID(stoi(fields[0]));
			p.setAppointmentID(stoi(fields[1]));
			p.setPatientID(stoi(fields[2]));
			p.setDoctorID(stoi(fields[3]));
			p.setDate(fields[4]);
			p.setMedicine(fields[5]);
			p.setNotes(fields[6]);
			storage.Add(p);

		}
		file.close();
	}

	//1,Ahmed Ali,25,M,03001234567,pass123,5000.00
//string Password, string Name, string Contact, int ID, int Age, string Gender, double Balance
	void FileHandler::appendPatient(patient& p)
	{
		ofstream file("data/patients.txt",ios::app);
		if (!file.is_open()) {
			throw FileNotFoundException("patients.txt");
		}
		file << p.getID() << "," << p.getName() << "," << p.getAge() << "," << p.getGender() <<"," <<p.getContact()<<"," << p.getPassword() << "," << p.getBalance() << endl;

		file.close();

	}
	//1, Dr.Ali Hassan, Cardiology, 03211234567, doc123, 1500.00
	//string Password, string Name, string Contact,int ID,string Specialization,double Fee
	void FileHandler::appendDoctor(Doctor& d)
	{
		ofstream file("data/doctors.txt", ios::app);
		if (!file.is_open()) {
			throw FileNotFoundException("doctors.txt");
		}
		file << d.getID() << "," << d.getName() << "," << d.getSpecialization() << "," << d.getContact() << "," << d.getPassword() << "," << d.getFee() << endl;

		file.close();
	}

	//1,1,1,15-04-2026,09:00,completed
//int ID,int PatientID,int DoctorID ,string Date,string Timeslot,string Status
	void FileHandler::appendAppointment(Appointment& a)
	{
		ofstream file("data/appointments.txt", ios::app);
		if (!file.is_open()) {
			throw FileNotFoundException("appointments.txt");
		}
		file << a.getID() << "," << a.getPatientID() << "," << a.getDoctorID() << "," << a.getDate() << "," << a.getTimeslot() << "," << a.getStatus() << endl;

		file.close();
	}

	//1,1,1,1500.00,paid,15-04-2026
//int ID,int Pid,int Aid,double Amount,string Status,string Date
	void FileHandler::appendBill(Bill& b)
	{
		ofstream file("data/bills.txt", ios::app);
		if (!file.is_open()) {
			throw FileNotFoundException("bills.txt");
		}
		file << b.getID() << "," << b.getPatientID() << "," << b.getAppointmentID() << "," << b.getAmount() << "," << b.getStatus() << "," << b.getDate() << endl;

		file.close();
	}
	//1,1,1,1,15-04-2026,Paracetamol 500mg,Take after meals
	//int ID, int Aid, int Did, int Pid, string Date, string Medicine, string Notes
	void FileHandler::appendPrescription(prescription& p)
	{
		ofstream file("data/prescriptions.txt", ios::app);
		if (!file.is_open()) {
			throw FileNotFoundException("prescriptions.txt");
		}
		file << p.getID() << "," << p.getAppointmentID() << "," << p.getPatientID() << "," << p.getDoctorID() << "," << p.getDate() << "," << p.getMedicine() << "," << p.getNotes() << endl;

		file.close();
	}
	//1, Admin, admin123,03334026526
	void FileHandler::appendAdmin(Admin& a)
	{
		ofstream file("data/admin.txt", ios::app);
		if (!file.is_open()) {
			throw FileNotFoundException("admin.txt");
		}
		file << a.getID() << "," << a.getName() << "," << a.getPassword() << "," << a.getContact() << endl;

		file.close();
	}
	
	
	void FileHandler::updatePatients(Storage<patient>& storage) {
		ofstream file("data/patients.txt");
		if (!file.is_open()) {
			throw FileNotFoundException("patients.txt");
		}
		for (int i = 0; i < storage.getSize(); i++) {
			file << storage[i].getID() << ","
				<< storage[i].getName() << ","
				<< storage[i].getAge() << ","
				<< storage[i].getGender() << ","
				<< storage[i].getContact() << ","
				<< storage[i].getPassword() << ","
				<< storage[i].getBalance() << "\n";
		}
		file.close();
	}

	//1, Dr.Ali Hassan, Cardiology, 03211234567, doc123, 1500.00
	//string Password, string Name, string Contact,int ID,string Specialization,double Fee
	void FileHandler::updateDoctors(Storage<Doctor>& storage)
	{
		ofstream file("data/doctors.txt");
		if (!file.is_open()) {
			throw FileNotFoundException("doctors.txt");
		}
		for (int i = 0; i < storage.getSize(); i++) {
			file << storage[i].getID() << ","
				<< storage[i].getName() << ","
				<< storage[i].getSpecialization() << ","
				<< storage[i].getContact() << ","
				<< storage[i].getPassword() << ","
				<< storage[i].getFee() << "\n";
		}
		file.close();
	}
	//1,1,1,15-04-2026,09:00,completed
//int ID,int PatientID,int DoctorID ,string Date,string Timeslot,string Status
	void FileHandler::updateAppointments(Storage<Appointment>& storage)
	{
		ofstream file("data/appointments.txt");
		if (!file.is_open()) {
			throw FileNotFoundException("appointments.txt");
		}
		for (int i = 0; i < storage.getSize(); i++) {
			file << storage[i].getID() << ","
				<< storage[i].getPatientID() << ","
				<< storage[i].getDoctorID() << ","
				<< storage[i].getDate() << ","
				<< storage[i].getTimeslot() << ","
				<< storage[i].getStatus() << "\n";
		}
		file.close();
	}

	//1,1,1,1500.00,paid,15-04-2026
//int ID,int Pid,int Aid,double Amount,string Status,string Date
	void FileHandler::updateBills(Storage<Bill>& storage)
	{
		ofstream file("data/bills.txt");
		if (!file.is_open()) {
			throw FileNotFoundException("bills.txt");
		}
		for (int i = 0; i < storage.getSize(); i++) {
			file << storage[i].getID() << ","
				<< storage[i].getPatientID() << ","
				<< storage[i].getAppointmentID() << ","
				<< storage[i].getAmount() << ","
				<< storage[i].getStatus() << ","
				<< storage[i].getDate() << "\n";
		}
		file.close();
	}

	
	void FileHandler::updatePrescriptions(Storage<prescription>& storage)
	{
		ofstream file("data/prescriptions.txt");
		if (!file.is_open()) {
			throw FileNotFoundException("prescriptions.txt");
		}
		for (int i = 0; i < storage.getSize(); i++) {
			file << storage[i].getID() << "," << storage[i].getAppointmentID() << "," << storage[i].getDoctorID() << "," << storage[i].getPatientID() << "," << storage[i].getDate() << "," << storage[i].getMedicine() << "," << storage[i].getNotes() << endl;
		}
		file.close();
	}

	void FileHandler::dischargePatient(int PID, Storage<patient>& patients,
		Storage<Appointment>& appointment,
		Storage<Bill>& bill,
		Storage<prescription>& Prescription)
	{
		ofstream dischargedFile("data/discharged.txt", ios::app);
		for (int i = 0; i < patients.getSize(); i++) {
			if (patients[i].getID() == PID) {
				dischargedFile << patients[i].getID() << ","
					<< patients[i].getName() << ","
					<< patients[i].getAge() << ","
					<< patients[i].getGender() << ","
					<< patients[i].getContact() << ","
					<< patients[i].getPassword() << ","
					<< patients[i].getBalance() << "\n";
				break;
			}
		}

		//1,1,1,15-04-2026,09:00,completed
//int ID,int PatientID,int DoctorID ,string Date,string Timeslot,string Status
		for (int i = 0; i < appointment.getSize(); i++)
		{
			if (PID == appointment[i].getPatientID())
			{
				dischargedFile << appointment[i].getID() << ","
					<< appointment[i].getPatientID() << ","
					<< appointment[i].getDoctorID() << ","
					<< appointment[i].getDate() << ","
					<< appointment[i].getTimeslot() << ","
					<< appointment[i].getStatus() 
					<< "\n";
				
			}
		}

		for (int i = 0; i < bill.getSize(); i++) {
			if (bill[i].getPatientID() == PID) {
				dischargedFile << bill[i].getID() << ","
					<< bill[i].getPatientID() << ","
					<< bill[i].getAppointmentID() << ","
					<< bill[i].getAmount() << ","
					<< bill[i].getStatus() << ","
					<< bill[i].getDate() << "\n";
			}
		}

		for (int i = 0; i < Prescription.getSize(); i++) {
			if (Prescription[i].getPatientID() == PID)
			{
				dischargedFile << Prescription[i].getID() << ","
					<< Prescription[i].getAppointmentID() << ","
					<< Prescription[i].getPatientID() << ","
					<< Prescription[i].getDoctorID() << ","
					<< Prescription[i].getDate() << ","
					<< Prescription[i].getMedicine() << ","
					<< Prescription[i].getNotes() << "\n";
			}
		}

		dischargedFile.close();

	}

	void FileHandler::ViewSecurityLog()
	{
		ifstream file("data/security_log.txt");
		string line;
		bool empty = true;
		while (getline(file, line)) {
			if (!line.empty()) {
				cout << line << endl;
				empty = false;
			}
		}
		if (empty) cout << "No security events logged." << endl;
		file.close();
	}

	void FileHandler::WriteToSecurityLog(string role, int enteredID)
	{
		ofstream file("data/security_log.txt", ios::app);
		if (!file.is_open()) return;  // if file cant open just skip silently

		// get current timestamp
		time_t now = time(0);
		tm localTime;

		localtime_s(&localTime, &now);
		char buffer[25];
		strftime(buffer, 25, "%d-%m-%Y %H:%M:%S", &localTime);

		// write one line to the log
		file << buffer << ","        // timestamp
			<< role << ","          // Patient, Doctor or Admin
			<< enteredID << ","     // the ID they tried
			<< "FAILED" << "\n";    // always FAILED since we only log failures

		file.close();
	}
	

	//int main() {
	//	FileHandler fh;
	//	Storage<patient> patients;
	//	Storage<Doctor> doctors;
	//	Storage<Appointment> appointments;
	//	Storage<Bill> bills;

	//	try {
	//		// load first
	//		fh.loadPatients(patients);
	//		fh.loadDoctors(doctors);
	//		fh.loadAppointments(appointments);
	//		fh.loadBills(bills);

	//		// test updatePatients — change patient 1 balance
	//		patient* p = patients.findByID(1);
	//		if (p != nullptr) {
	//			p->setBalance(9999.00);
	//			fh.updatePatients(patients);
	//			cout << "Patient balance updated" << endl;
	//		}

	//		// test updateDoctors — change doctor 1 fee
	//		Doctor* d = doctors.findByID(1);
	//		if (d != nullptr) {
	//			d->setFee(2500.00);
	//			fh.updateDoctors(doctors);
	//			cout << "Doctor fee updated" << endl;
	//		}

	//		// test updateAppointments — change appointment 1 status
	//		Appointment* a = appointments.findByID(1);
	//		if (a != nullptr) {
	//			a->setStatus("completed");
	//			fh.updateAppointments(appointments);
	//			cout << "Appointment status updated" << endl;
	//		}

	//		// test updateBills — change bill 1 status
	//		Bill* b = bills.findByID(1);
	//		if (b != nullptr) {
	//			b->setStatus("paid");
	//			fh.updateBills(bills);
	//			cout << "Bill status updated" << endl;
	//		}

	//		cout << "\nAll updates done. Check your .txt files to verify." << endl;
	//	}
	//	catch (FileNotFoundException& e) {
	//		cout << e.what() << endl;
	//	}

	//	return 0;
	//}