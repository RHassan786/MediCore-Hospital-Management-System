//#include "menus.h"
//
//int main()
//{
//    FileHandler h;
//
//    Storage<patient> Patient;
//    Storage<Doctor> doctor;
//    Storage<Admin> admin;
//    Storage<Appointment> appointment;
//    Storage<Bill> bill;
//    Storage<prescription> Prescription;
//
//    try
//    {
//        h.loadPatients(Patient);
//        h.loadDoctors(doctor);
//        h.loadAdmin(admin);
//        h.loadAppointments(appointment);
//        h.loadBills(bill);
//        h.loadPrescriptions(Prescription);
//
//        showLoginMenu(Patient, doctor, admin, appointment, bill, Prescription, h);
//    }
//    catch (FileNotFoundException& e)
//    {
//        cout << e.what() << endl;
//    }
//
//    return 0;
//}
#include <iostream>
#include "FileHandler.h"
#include "Storage.h"
#include "GUI.h"
using namespace std;

int main() {
    Storage<patient>      patients;
    Storage<Doctor>       doctors;
    Storage<Admin>        admins;
    Storage<Appointment>  appointments;
    Storage<Bill>         bills;
    Storage<prescription> prescriptions;
    FileHandler fh;

    try {
        fh.loadPatients(patients);
        fh.loadDoctors(doctors);
        fh.loadAdmin(admins);
        fh.loadAppointments(appointments);
        fh.loadBills(bills);
        fh.loadPrescriptions(prescriptions);
    }
    catch (FileNotFoundException& e) {
        cout << e.what() << endl;
        return 1;
    }

    GUI gui(patients, doctors, admins, appointments, bills, prescriptions, fh);
    gui.run();

    return 0;
}