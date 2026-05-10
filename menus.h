#pragma once
#include <iostream>
using namespace std;
#include<string>
#include"patient.h"
#include"Doctor.h"
#include"Bill.h"
#include"Appointment.h"
#include"Validator.h"
#include"HospitalException.h"
#include"person.h"
#include"prescription.h"
#include"Storage.h"
#include"FileHandler.h"

int PositiveInt();
double PositiveDouble();
string toLowerCase(string str);
string getTodayDate();


void HandleBookAppointment(patient& p, Storage <Doctor>& doctor, Storage<Appointment>& appointment, FileHandler& h, Storage <Bill>& bill, Storage<patient>& Patient);
void HandleCancelAppointment(patient& p, Storage<Appointment>& appointment, FileHandler& h, Storage<patient>& Patient, Storage <Doctor>& doctor, Storage <Bill>& bill);
void HandleViewMyAppointment(patient& p, Storage<Appointment>& appointment, Storage <Doctor>& doctor);
void HandleViewMyMedicalRecords(patient& p, Storage<Appointment>& appointment, Storage <Doctor>& doctor, Storage <prescription>& Prescription);
void HandleViewMyBills(patient& p, Storage <Bill>& bill);
void HandlePayBill(patient& p, Storage <Bill>& bill, FileHandler& h, Storage<patient>& Patient);
void HandleTopUpBalance(patient& p, FileHandler& h, Storage<patient>& Patient);

void HandleViewTodaysAppointment(Doctor& d, Storage <Appointment>& appointment, Storage<patient>& Patient);
void HandleMarkAppointmentComplete(Doctor& d, FileHandler& h, Storage <Appointment>& appointment);
void HandleMarkAppointmentNoShow(Doctor& d, FileHandler& h, Storage <Appointment>& appointment, Storage <Bill>& bill);
void HandleWritePrescription(Doctor& d, FileHandler& h, Storage <Appointment>& appointment, Storage <prescription>& Prescription);
void HandleViewPatientMedicalHistory(Doctor& d, Storage <Appointment>& appointment, Storage<patient>& Patient, Storage <prescription>& Prescription);

void HandleAddPatient(Storage <patient>& Patient, FileHandler& h);
void HandleAddDoctor(Storage <Doctor>& doctor, FileHandler& h);
void HandleRemoveDoctor(Storage <Doctor>& doctor, FileHandler& h, Storage <Appointment>& appointment);
void HandleViewAllPatients(Storage<patient>& Patient, Storage <Bill>& bill);
void HandleViewAllDoctors(Storage <Doctor>& doctor);
void HandleViewAllAppointments(Storage <Appointment>& appointment);
void HandleViewUnpaidBills(Storage <Bill>& bill, Storage<patient>& Patient);
void HandleDischargePatient(Storage<patient>& Patient, Storage <Bill>& bill, FileHandler& h, Storage <Appointment>& appointment, Storage<prescription> & Prescription);
void HandleViewSecurityLog(FileHandler& h);
void HandleGenerateDailyReport(Storage<patient>& Patient, Storage <Doctor>& doctor, Storage <Bill>& bill, Storage <Appointment>& appointment);

void showPatientMenu(patient& p, Storage<Doctor>& doctor, Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription, Storage<patient>& Patient, FileHandler& h);
void showDoctorMenu(Doctor& d, Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription, Storage<patient>& Patient, FileHandler& h);
void showAdminMenu(Storage<Doctor>& doctor, Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription, Storage<patient>& Patient, FileHandler& h);

void LogInPatient(Storage<patient>& Patient, Storage<Doctor>& doctor, Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription,
    FileHandler& h);
void LogInDoctor(Storage<patient>& Patient, Storage<Doctor>& doctor,
    Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription,
    FileHandler& h);
void LogInAdmin(Storage<patient>& Patient, Storage<Doctor>& doctor,
    Storage<Admin>& admin, Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription,
    FileHandler& h);
void showLoginMenu(Storage<patient>& Patient, Storage<Doctor>& doctor,
    Storage<Admin>& admin, Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription,
    FileHandler& h);

