#include "menus.h"
#include<ctime>
using namespace std;

int PositiveInt()
{
    int n;
    while (!(cin >> n))
    {
        cout <<endl<< "Invalid input. Please enter an integer: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return n;
}

double PositiveDouble()
{
    double n;

    while (!(cin >> n) || !Validator::isPositiveNumber(n))
    {
        cout <<endl<< "Invalid input. Please enter a positive number: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    return n;
}


string toLowerCase(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
    }
    return str;
}

string getTodayDate()
{
    time_t now = time(0);          // gets current system time
    tm localTime;

    localtime_s(&localTime, &now); // converts time_t into local date/time safely

    char date[11];                 // DD-MM-YYYY = 10 chars + '\0'
    strftime(date, sizeof(date), "%d-%m-%Y", &localTime);

    return string(date);
}

bool isDateGreater(string a, string b)
{
    int dayA = stoi(a.substr(0, 2));
    int monthA = stoi(a.substr(3, 2));
    int yearA = stoi(a.substr(6, 4));

    int dayB = stoi(b.substr(0, 2));
    int monthB = stoi(b.substr(3, 2));
    int yearB = stoi(b.substr(6, 4));

    if (yearA > yearB)
        return true;

    if (yearA == yearB && monthA > monthB)
        return true;

    if (yearA == yearB &&
        monthA == monthB &&
        dayA > dayB)
        return true;

    return false;
}


time_t convertDateStringToTime(string Date)
{
    int day = stoi(Date.substr(0, 2));
    int month = stoi(Date.substr(3, 2));
    int year = stoi(Date.substr(6, 4));

    tm dateTime = {};
    dateTime.tm_mday = day;
    dateTime.tm_mon = month - 1;      // tm month is 0-11
    dateTime.tm_year = year - 1900;   // tm year starts from 1900 // c++ understands 126 as 2026
    dateTime.tm_hour = 0;
    dateTime.tm_min = 0;
    dateTime.tm_sec = 0;

    return mktime(&dateTime);
}

void HandleBookAppointment(patient& p, Storage <Doctor>& doctor, Storage<Appointment>& appointment, FileHandler& h, Storage <Bill>& bill, Storage<patient>& Patient)
{
    string Specialization;

    cout << endl << "Enter Specialization to search: ";
    cin >> Specialization;
    

    Specialization=toLowerCase(Specialization);

    bool found = 0;
    for (int i = 0; i < doctor.getSize(); i++)
    {

        if (toLowerCase(doctor[i].getSpecialization()) == Specialization)
        {
            found = 1;
            cout << endl;
            doctor[i].displayInfo();
        }
    }
    if (found != 1)
    {
        cout <<endl<< "No Doctors Found for that Specialization";
        return;
    }

    int doctorID;
    cout << endl << "Enter Doctor ID: ";
      doctorID=PositiveInt();
    if (Validator::isValidID(doctorID, doctor) == 0)
    {
        cout << endl << "Doctor Not Found.";
        return;
    }
    string Date;
    int attempt = 0;
    while (attempt < 3)
    {
        cout << endl << "Enter Date (DD-MM-YYYY): ";
        cin >> Date;
        if (Validator::isDateValid(Date))
        {
            break;
        }
        cout << endl << "Invalid Date.Please Enter Date Again: ";
        attempt++;
    }
    if (attempt == 3)
    {
        return;
    }
    attempt++;



    string TimeSlot;
    do {
        cout << endl << "Available slots: 09:00 10:00 11:00 12:00 13:00 14:00 15:00 16:00" << endl;
        cout << endl << "Enter Time Slot: ";
        cin >> TimeSlot;

        if (!Validator::isTimeSlotValid(TimeSlot)) {
            try {
                throw InvalidInputException("Invalid TimeSlot! Choose from 09:00 to 16:00");
            }
            catch (InvalidInputException& e) {
                cout << endl << e.what() << endl;
            }
        }
    } while (!Validator::isTimeSlotValid(TimeSlot));


    Appointment a(0, 0, doctorID, Date, TimeSlot, "pending");

    for (int i = 0; i < appointment.getSize(); i++)
    {
        try
        {
            if (a == appointment[i])
            {
                throw SlotUnavailableException();
            }
        }
        catch (SlotUnavailableException& s)
        {
            cout << s.what();
            return;
        }
    }

    try
    {
        for (int i = 0; i < doctor.getSize(); i++)
        {
            if (doctorID == doctor[i].getID())
            {
                if (p.getBalance() < doctor[i].getFee())
                {
                    throw InsufficientFundsException();
                }
            }
        }
    }
    catch (InsufficientFundsException& I)
    {
        cout <<endl<< I.what();
        return;
    }

    double fee=0;
    for (int i = 0; i < doctor.getSize(); i++)
    {
        if (doctorID == doctor[i].getID())
        {
            p -= doctor[i].getFee();
            fee = doctor[i].getFee();
        }
    }
    
    int MaxAID=0;

    for (int i = 0; i < appointment.getSize(); i++)
    {
        if (appointment[i].getID() >= MaxAID)
        {
            MaxAID = appointment[i].getID();
        }
    }
    //int ID,int PatientID,int DoctorID ,string Date,string Timeslot,string Status
    Appointment New(MaxAID + 1, p.getID(), doctorID, Date, TimeSlot, "pending");
    appointment.Add(New);
    h.appendAppointment(New);

    int MaxBID=0;

    for (int i = 0; i < bill.getSize(); i++)
    {
        if (bill[i].getID() >= MaxBID)
        {
            MaxBID = bill[i].getID();
        }
    }
    //int ID, int Pid, int Aid, double Amount, string Status, string Date
    Bill b(MaxBID + 1, p.getID(), New.getID(), fee, "unpaid", Date);
    bill.Add(b);
    h.appendBill(b);

    for (int i = 0; i < Patient.getSize(); i++)
    {
        if (p.getID() == Patient[i].getID())
        {
            Patient[i].setBalance(p.getBalance());
        }
    }

    h.updatePatients(Patient);
    
    cout << endl << "Appointment Booked Successfully." << endl << "Appointment ID: " << New.getID();

}

void HandleCancelAppointment(patient& p, Storage<Appointment>& appointment, FileHandler& h, Storage<patient>& Patient, Storage <Doctor>& doctor, Storage <Bill>& bill)
{
    bool found = 0;
   
    for (int i = 0; i < appointment.getSize(); i++)
    {
        if (p.getID() == appointment[i].getPatientID())
        {
           

            if (appointment[i].getStatus() == "pending")
            {
                found = true;
                cout <<endl<< appointment[i].getID() << "| ";
                for (int j = 0; j < doctor.getSize();j++)
                {
                    if (appointment[i].getDoctorID() == doctor[j].getID())
                    {
                        cout << doctor[j].getName()<<"| ";
                        break;
                    }
                }
                cout << appointment[i].getDate() << "| " << appointment[i].getTimeslot();

            }
        }
    }

    if (found == false)
    {
        cout << endl << "You Have no Pending Appointments.";
        return;
    }

    int AID;
    cout << endl << "Enter Appointment ID to Cancel: ";
    AID = PositiveInt();

    int IndexOfAppointment = -1;
    for (int i = 0; i < appointment.getSize(); i++) {
        if (appointment[i].getID() == AID &&
            appointment[i].getPatientID() == p.getID() &&
            appointment[i].getStatus() == "pending") {
            IndexOfAppointment = i;
            break;
        }
    }
    if (IndexOfAppointment == -1) {
        cout << "Invalid Appointment ID." << endl;
        return;
    }

    try
    {
        if (Validator::isValidID(AID, appointment) == 0)
        {
            throw InvalidInputException("Invalid Input. This Appointment ID doesnot exist.");
        }
    }
    catch (InvalidInputException& I)
    {
        cout <<endl<< I.what();
        return;
    }
    

    if (appointment[IndexOfAppointment].getID() != AID || appointment[IndexOfAppointment].getStatus()!="pending")
    {
        cout <<endl<< "Invalid Appointment ID.";
        return;
    }

    appointment[IndexOfAppointment].setStatus("cancelled");
    h.updateAppointments(appointment);

    double fee=0;
    for (int i = 0; i < doctor.getSize(); i++)
    {
        if (appointment[IndexOfAppointment].getDoctorID() == doctor[i].getID())
        {
            p += doctor[i].getFee();
            fee = doctor[i].getFee();
        }
    }

    for (int i = 0; i < Patient.getSize(); i++) {
        if (Patient[i].getID() == p.getID()) {
            Patient[i].setBalance(p.getBalance());
            break;
        }
    }

    h.updatePatients(Patient);
    for (int i = 0; i < bill.getSize(); i++)
    {
        if (AID == bill[i].getAppointmentID())
        {
            bill[i].setStatus("cancelled");
        }
    }
    h.updateBills(bill);
    cout << endl << "Appointment Cancelled. PKR " << fee;



}

void HandleViewMyAppointment(patient& p, Storage<Appointment>& appointment, Storage <Doctor>& doctor)
{
    bool found = 0;
    int count = 0;
    int arr[100];
    for (int i = 0; i < appointment.getSize(); i++)
    {
        if (appointment[i].getPatientID() == p.getID())
        {
            found = true;
            arr[count] = i;
            count++;
            
            
        }
    }
    
    for (int i=0;i < count-1; i++)
    {
        for (int j = i+1; j < count; j++)
        {
            if (isDateGreater(appointment[arr[i]].getDate(), appointment[arr[j]].getDate()))
            {
                swap(arr[i], arr[j]);
            }
        }
    }

    for (int i = 0; i < count; i++)
    {
       
            cout << endl <<"Appointment ID: "<< appointment[arr[i]].getID() << "| ";
            for (int j = 0; j < doctor.getSize(); j++)
            {
                if (appointment[arr[i]].getDoctorID() == doctor[j].getID())
                {
                    cout <<"Doctor: "<< doctor[j].getName() << "| " <<"Doctor Specialization: "<< doctor[j].getSpecialization();  //check not sure if its gonna be arr[i] or j

                    break;
                }
            }
            cout << "| " <<"Date: "<<appointment[arr[i]].getDate() << "| " <<"Time Slot: " << appointment[arr[i]].getTimeslot() << "| " <<"Status: " << appointment[arr[i]].getStatus();

        
    }
    if (found == 0)
    {
        cout << endl << "No appointments Found ";
    }
    
}

void HandleViewMyMedicalRecords(patient& p, Storage<Appointment>& appointment, Storage <Doctor>& doctor, Storage <prescription>& Prescription)
{
    bool found = 0;
    int count = 0;
    int arr[100];
    for (int i = 0; i < Prescription.getSize(); i++)
    {
        if (Prescription[i].getPatientID() == p.getID())
        {
            found = true;
            arr[count] = i;
            count++;


        }
    }
    if (found == 0)
    {
        cout << endl << "No Medical Records Found ";
        return;
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (isDateGreater(Prescription[arr[i]].getDate(), Prescription[arr[j]].getDate())==0)
            {
                swap(arr[i], arr[j]);
            }
        }
    }

    for (int i = 0; i < count; i++)
    {

        cout << endl <<"Date: " << Prescription[arr[i]].getDate() << "| ";
        for (int j = 0; j < doctor.getSize(); j++)
        {
            if (Prescription[arr[i]].getDoctorID() == doctor[j].getID())
            {
                cout <<"Doctor: "<<doctor[j].getName(); //check not sure if its gonna be arr[i] or j

                break;
            }
        }
        cout << "| " <<"Medicines: "<< Prescription[arr[i]].getMedicine() << "| " <<"Notes: " << Prescription[arr[i]].getNotes();


    }
   

    
}

void HandleViewMyBills(patient& p, Storage <Bill>& bill)
{
    double TotalUnpaidAmount=0;
    bool found = 0;
    for (int i = 0; i<bill.getSize(); i++)
    {
        if (bill[i].getPatientID() == p.getID())
        {
            found = true;
            cout <<endl<< bill[i];
            if (bill[i].getStatus() == "unpaid")
            {
                TotalUnpaidAmount = TotalUnpaidAmount + bill[i].getAmount();
            }
        }
    }
    if (found == false)
    {
        cout << endl << "No Bills Found.";
    }
    cout << endl << "Total Outstanding Unpaid Amount= " << TotalUnpaidAmount;
    
}

void HandlePayBill(patient& p, Storage <Bill>& bill, FileHandler& h, Storage<patient>& Patient)
{
    bool found = 0;
   
    for (int i = 0; i < bill.getSize(); i++)
    {
        if (bill[i].getStatus()=="unpaid"&& bill[i].getPatientID() ==p.getID() )
        {
            found = 1;
            cout <<endl<<bill[i];
            
        }
    }

    if (found == 0)
    {
        cout <<endl<<"No Unpaid Bills.";
        return;
    }
    
    int BillID;
    cout <<endl<< "Enter Bill ID to pay: ";
    BillID = PositiveInt();

    int BillIndex=-1;

    for (int i = 0; i < bill.getSize(); i++)
    {
        if (BillID == bill[i].getID())
        {
            BillIndex = i;
            if (bill[i].getPatientID() != p.getID()|| bill[i].getStatus()!="unpaid")
            {
                cout <<endl<< "Invalid Bill ID.";
                return;
            }
        }
    }
   
    if (BillIndex == -1)
    {
        cout << endl << "Bill Not Found.";
        return;
    }
    try
    {
        if (p.getBalance() < bill[BillIndex].getAmount())
        {
            throw InsufficientFundsException();
        }
    }
    catch (InsufficientFundsException& I)
    {
        cout <<endl<< I.what();
        return;
    }

    p -= bill[BillIndex].getAmount();
    for (int i = 0; i < Patient.getSize(); i++) {
        if (Patient[i].getID() == p.getID()) {
            Patient[i].setBalance(p.getBalance());
            break;
        }
    }

  

    bill[BillIndex].setStatus("paid");
    h.updateBills(bill);
    cout << endl << "Bill Paid Successfully. Remaining Balance: Pkr " << p.getBalance();
    h.updatePatients(Patient);
    


}

void HandleTopUpBalance(patient& p, FileHandler& h, Storage<patient>& Patient)
{
    double amount;
    int attempt = 0;
    
    do
    {
        cout << "Enter Amount to Add (PKR): ";
        amount = PositiveDouble();
        try
        {
            if (Validator::isPositiveNumber(amount) == 0)
            {
                throw InvalidInputException("Amount should be greater than 0!");

            }
            break;

        }
        catch (InvalidInputException& I)
        {
            cout <<endl<< I.what();
            attempt++;
        }

    } while (attempt < 3);
    
    if (attempt >= 3)
    {
        return;
    }

   
    p += amount;

    for (int i = 0; i < Patient.getSize(); i++) {
        if (Patient[i].getID() == p.getID()) {
            Patient[i].setBalance(p.getBalance());
            break;
        }
    }
   
    
    h.updatePatients(Patient);
}


void HandleViewTodaysAppointment(Doctor & d,Storage <Appointment> & appointment, Storage<patient>& Patient)
{
    string TodayDate;
    TodayDate = getTodayDate();
    bool found = 0;
    int count = 0;
    int arr[100];


    for (int i = 0; i < appointment.getSize(); i++)
    {
        if (appointment[i].getDoctorID() == d.getID() && appointment[i].getDate() == TodayDate)
        {
            found = 1;
            arr[count] = i;
            count++;
            
        }
    }

    for (int i = 0; i < count-1; i++)
    {
        for (int j = i+1; j < count; j++)
        {
            if (appointment[arr[i]].getTimeslot() > appointment[arr[j]].getTimeslot())
            {
                swap(arr[i], arr[j]);
            }
        }
    }


  

    for (int i = 0; i < count; i++)
    {
        cout << endl << "Appointment ID: " << appointment[arr[i]].getID();
        for (int j = 0; j < Patient.getSize(); j++)
        {
            if (appointment[arr[i]].getPatientID() == Patient[j].getID())
            {

                cout << "| Patient Name: " << Patient[j].getName();
                break;
            }
        }
        cout << "| Time Slot: " << appointment[arr[i]].getTimeslot();
        cout << "| Status : " << appointment[arr[i]].getStatus();
    }

    if (found == 0)
    {
        cout <<endl<< "No Appointments Scheduled for Today.";
    }
    
}

void HandleMarkAppointmentComplete(Doctor& d,  FileHandler& h, Storage <Appointment> &appointment)
{

    bool isPending = 1;
    for (int i = 0; i < appointment.getSize(); i++)
    {
        if (appointment[i].getDoctorID() == d.getID() && appointment[i].getStatus()=="pending"&& appointment[i].getDate() == getTodayDate())
        {
            cout << appointment[i];
            isPending = 0;
        }
    }

    if (isPending == 1)
    {
        cout <<endl<<"No Pending Appointments Today";
        return;
    }
    int AID;
    cout <<endl<< "Enter Appointment ID: ";
    AID = PositiveInt();
    int Index = -1;
    for (int i = 0; i < appointment.getSize(); i++)
    {
        if (AID == appointment[i].getID())
        {
            Index = i;
        }
    }
    if (Index == -1)
    {
        cout <<endl<<"Invalid.ID does not exist";
        return;
    }
    if (appointment[Index].getDoctorID() != d.getID()|| appointment[Index].getStatus()!="pending"|| appointment[Index].getDate()!=getTodayDate())
    {
        cout <<endl<<"This Appointment ID either does not belong to this Doctor or does not has status as pending or is not dated Today";
        return;
    }
    appointment[Index].setStatus("completed");
    h.updateAppointments(appointment);
    cout <<endl<< "Appointment Marked as Completed.";

}

void HandleMarkAppointmentNoShow(Doctor& d,  FileHandler& h, Storage <Appointment> &appointment, Storage <Bill>& bill)
{
    bool isPending = 1;
    for (int i = 0; i < appointment.getSize(); i++)
    {
        if (appointment[i].getDoctorID() == d.getID() && appointment[i].getStatus() == "pending" && appointment[i].getDate() == getTodayDate())
        {
            cout << appointment[i];
            isPending = 0;

        }
    }
    if (isPending == 1)
    {
        cout <<endl<< "No Pending Appointments Today";
        return;
    }

    int AID;
    cout << endl << "Enter Appointment ID: ";
    AID = PositiveInt();
    int Index = -1;
    for (int i = 0; i < appointment.getSize(); i++)
    {
        if (AID == appointment[i].getID())
        {
            Index = i;
        }
    }
    if (Index == -1)
    {
        cout << endl << "Invalid.ID does not exist";
        return;
    }
    if (appointment[Index].getDoctorID() != d.getID() || appointment[Index].getStatus() != "pending" || appointment[Index].getDate() != getTodayDate())
    {
        cout << endl << "This Appointment ID either does not belong to this Doctor or does not has status as pending or is not dated Today";
        return;
    }

   
    for (int i = 0; i < bill.getSize(); i++)
    {
        if (appointment[Index].getID()==bill[i].getAppointmentID())
        {
            bill[i].setStatus("cancelled");
            
        }
    }
   
    
    appointment[Index].setStatus("noshow");
    h.updateAppointments(appointment);
    cout << "Appointment Marked as no-show.";
    h.updateBills(bill);

}

void HandleWritePrescription(Doctor& d, FileHandler& h, Storage <Appointment> &appointment, Storage <prescription>& Prescription) 
{
    int AID;
    cout << endl << "Enter Appointment ID: ";
    AID = PositiveInt();
    int Index = -1;
    for (int i = 0; i < appointment.getSize(); i++)
    {
        if (AID == appointment[i].getID())
        {
            Index = i;
        }
    }
    if (Index == -1)
    {
        cout << endl << "Invalid.ID does not exist";
        return;
    }
    if (appointment[Index].getDoctorID() != d.getID() || appointment[Index].getStatus() != "completed" )
    {
        cout << endl << "This Appointment ID either does not belong to this Doctor or does not has status as completed";
        return;
    }

    for (int i = 0; i < Prescription.getSize(); i++)
    {
        if (AID == Prescription[i].getAppointmentID())
        {
            cout <<endl<< "Prescription already written for this appointment.";
            return;
        }
    }
    
    string medicines;
    cout << endl << "Enter Medicines: ";
    cin.ignore();
    getline(cin, medicines);
    string notes;
    cout << endl << "Enter Notes: ";
    getline(cin, notes);

    if (medicines.length() > 499) medicines = medicines.substr(0, 499);
    if (notes.length() > 299) notes = notes.substr(0, 299);
   

    int pID=appointment[Index].getPatientID();
    
    int MaxPrescriptionID = 0;

    for (int i = 0; i < Prescription.getSize(); i++)
    {
        if (Prescription[i].getID() >= MaxPrescriptionID)
        {
            MaxPrescriptionID = Prescription[i].getID();
        }
    }

    prescription p(MaxPrescriptionID + 1, AID, d.getID(), pID, appointment[Index].getDate(), medicines, notes);
    Prescription.Add(p);
    h.appendPrescription(p);
    cout << endl << "Prescription Saved.";
     
}

void HandleViewPatientMedicalHistory(Doctor& d,  Storage <Appointment> &appointment, Storage<patient>& Patient, Storage <prescription>& Prescription)
{
    int PID;
    cout << "Enter Patient ID: ";
    PID = PositiveInt();
    try
    {
        if (!Validator::isValidID(PID, Patient))
        {
            throw InvalidInputException("Patient ID does not Exist.");
        }

    }
    catch (InvalidInputException& I)
    {
        cout << endl << I.what();
        return;
    }

    int count = 0;
    for(int i = 0; i < Patient.getSize(); i++)
    {
        if (PID == Patient[i].getID())
        {
            for (int j = 0; j < appointment.getSize(); j++)
            {
                if (Patient[i].getID() == appointment[j].getPatientID() && appointment[j].getStatus() == "completed"&& appointment[j].getDoctorID() == d.getID())
                {
                    count++;
                }
            }
        }
    }

    if (count < 1)
    {
        cout <<endl<< "Access Denied.";
        return;
    }

    
   

    bool found = 0;
    int Count = 0;
    int arr[100];
    for (int i = 0; i < Patient.getSize(); i++)
    {
        if (PID == Patient[i].getID())
        {
            for (int j = 0; j < Prescription.getSize(); j++)
            {
                if (Patient[i].getID() == Prescription[j].getPatientID() && Prescription[j].getDoctorID() == d.getID())
                {
                    found = true;
                    arr[Count] =j;
                    Count++;
                }
            }
        }
    }
    if (found == 0)
    {
        cout << endl << "No Medical Records Found ";
        return;
    }

    for (int i = 0; i < Count - 1; i++)
    {
        for (int j = i + 1; j < Count; j++)
        {
            if (!isDateGreater(Prescription[arr[i]].getDate(), Prescription[arr[j]].getDate()))
            {
                swap(arr[i], arr[j]);
            }
        }
    }
    cout <<endl<< "Displaying Patient's Medical History: ";

    for (int i = 0; i < Count; i++)
    {
        cout <<endl<< Prescription[arr[i]];
    }

}

void HandleAddPatient(Storage <patient>& Patient, FileHandler& h)
{
    string Name;
    int age;
    string gender;
    string contact;
    string password;
    double balance;
    cout << endl << "Enter Patient's Name: ";
    cin.ignore();
    getline(cin, Name);
    do {
        cout << endl << "Enter Patient's Age: ";
        age = PositiveInt();
        try
        {
            if (!Validator::isPositiveNumber(age))
            {
                throw InvalidInputException("Age must be greater than 0! Try Again");
            }
        }
        catch (InvalidInputException& I)
        {
            cout << I.what();
        }
    } while (!Validator::isPositiveNumber(age));

    cout << endl << "Enter Gender";
    cin >> gender;
    do {
        cout << "Enter Contact Number (Should be 11 digits long): ";
        cin >> contact;
        try
        {
            if (!Validator::isContactNumberValid(contact))
            {
                throw InvalidInputException("Contact Number Not Valid! Try Again");
            }
        }
        catch (InvalidInputException& I)
        {
            cout << I.what();
        }
    } while (!Validator::isContactNumberValid(contact));


    do {
        cout << "Enter Password (Minimum 6 characters): ";
        cin >> password;
        try
        {
            if (!Validator::isPasswordValid(password))
            {
                throw InvalidInputException("Password Not Valid. Must be minimum 6 characters ! Try Again");
            }
        }
        catch (InvalidInputException& I)
        {
            cout << I.what();
        }
    } while (!Validator::isPasswordValid(password));

    do {
        cout << "Enter Initial Balance: ";
        balance = PositiveDouble();
        try
        {
            if (!Validator::isPositiveNumber(balance))
            {
                throw InvalidInputException("Balance must be greater than 0! Try Again");
            }
        }
        catch (InvalidInputException& I)
        {
            cout << I.what();
        }
    } while (!Validator::isPositiveNumber(balance));


    int MaxPatientID = 0;

    for (int i = 0; i < Patient.getSize(); i++)
    {
        if (Patient[i].getID() >= MaxPatientID)
        {
            MaxPatientID = Patient[i].getID();
        }
    }

    patient p(password, Name, contact, MaxPatientID + 1, age, gender, balance);
    Patient.Add(p);
    h.appendPatient(p);
    cout << "Patient Added Successfully. ID: " << p.getID();

}

void HandleAddDoctor(Storage <Doctor>& doctor, FileHandler& h)
{
    string Name;
    string Specialization;
    string contact;
    string password;
    double fee;
    cout <<endl<< "Enter Doctor's Name: ";
    cin.ignore();
    getline(cin, Name);
    cout << endl << "Enter Specialization";
        getline(cin, Specialization);
        do{
            cout << "Enter Contact Number (Should be 11 digits long): ";
            cin >> contact;
            try
            {
                if (!Validator::isContactNumberValid(contact))
                {
                    throw InvalidInputException("Contact Number Not Valid! Try Again");
                }
            }
            catch(InvalidInputException & I)
            {
                cout << I.what();
            }
        } while (!Validator::isContactNumberValid(contact));
        
        
        do {
            cout << "Enter Password (Minimum 6 characters): ";
            cin >> password;
            try
            {
                if (!Validator::isPasswordValid(password))
                {
                    throw InvalidInputException("Password Not Valid. Must be minimum 6 characters ! Try Again");
                }
            }
            catch (InvalidInputException& I)
            {
                cout << I.what();
            }
        } while (!Validator::isPasswordValid(password));

        do {
            cout << "Enter Consultation Fee: ";
            fee = PositiveDouble();
            try
            {
                if (!Validator::isPositiveNumber(fee))
                {
                    throw InvalidInputException("Fee must be greater than 0! Try Again");
                }
            }
            catch (InvalidInputException& I)
            {
                cout << I.what();
            }
        } while (!Validator::isPositiveNumber(fee));


        int MaxDoctorID = 0;

        for (int i = 0; i < doctor.getSize(); i++)
        {
            if (doctor[i].getID() >= MaxDoctorID)
            {
                MaxDoctorID = doctor[i].getID();
            }
        }

        Doctor d(password, Name, contact,MaxDoctorID+1 , Specialization, fee);
        doctor.Add(d);
        h.appendDoctor(d);
        cout << "Doctor Added Successfully. ID: " << d.getID();

}

void HandleRemoveDoctor(Storage <Doctor>& doctor, FileHandler& h, Storage <Appointment>& appointment)
{
    cout << endl << "Displaying All Doctors: ";
    for (int i = 0; i < doctor.getSize(); i++)
    {
        cout <<endl<< doctor[i];
    }

    int DID;
    cout <<endl<< "Enter Doctor ID to remove: ";
    DID = PositiveInt();

    bool found = 0;
    int Index;

    for (int i = 0; i < doctor.getSize(); i++)
    {
        if (DID == doctor[i].getID())
        {
            Index = i;
            found = true;
            for (int j = 0; j < appointment.getSize(); j++)
            {
                if (doctor[i].getID() == appointment[j].getDoctorID() && appointment[j].getStatus() == "pending")
                {
                    cout << endl << "Cannot remove doctor with pending appointments. Cancel or Resign them first";
                    return;
                }
            }
        }
    }

    if (found == 0)
    {
        cout <<endl<< "No Doctor Found with That ID";
        return;
    }
    doctor.RemoveByID(DID);
    h.updateDoctors(doctor);
    cout <<endl<< "Doctor Removed";


}

void HandleViewAllPatients(Storage<patient>& Patient, Storage <Bill>& bill)
{
    cout << endl << "========== PATIENT DIRECTORY ==========";
    for (int i = 0; i < Patient.getSize(); i++)
    {
        int count = 0;
        cout << endl << "---------------------------------------";
        cout << endl << "Record #" << (i + 1);
        cout << endl << Patient[i];
        for (int j = 0; j < bill.getSize(); j++)
        {
            if (Patient[i].getID() == bill[j].getPatientID() && bill[j].getStatus() == "unpaid")
            {
                count++;
            }
        }
        cout << endl << "Count of Unpaid Bills: " << count;
    }
    cout << endl << "=======================================";
}

void HandleViewAllDoctors(Storage <Doctor>& doctor)
{
    cout << endl << "=========== DOCTOR DIRECTORY ===========" << endl;
    for (int i = 0; i < doctor.getSize(); i++)
    {
        cout << "---------------------------------------";
        cout << endl << doctor[i];
        cout << endl;
    }
    cout << "=======================================";
}

void HandleViewAllAppointments(Storage <Appointment>& appointment)
{
    bool found = 0;
    int count = 0;
    int arr[100];


    for (int i = 0; i < appointment.getSize(); i++)
    {
        
        arr[count] = i;
        count++;
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (!isDateGreater(appointment[arr[i]].getDate(),appointment[arr[j]].getDate()))
            {
                swap(arr[i], arr[j]);
            }
        }
    }

    cout << endl << "========= APPOINTMENT DIRECTORY =========";
    for (int i = 0; i < count; i++)
    {
        cout << endl << "---------------------------------------";
        cout <<endl<< appointment[arr[i]];
    }
    cout << endl << "=========================================";


}

void HandleViewUnpaidBills(Storage <Bill>& bill, Storage<patient>& Patient)
{
    bool found = 0;
    cout << endl << "============= UNPAID BILLS =============";
    for (int i = 0; i < bill.getSize(); i++)
    {
        if (bill[i].getStatus() == "unpaid")
        {
            found = 1;
            cout << endl << "---------------------------------------";
            cout << endl << "Bill ID: " << bill[i].getID();
            for (int j = 0; j < Patient.getSize(); j++)
            {
                if (bill[i].getPatientID() == Patient[j].getID())
                {
                    cout << "| Patient Name: " << Patient[j].getName();
                    break;
                }
            }
            cout << "| Amount (PKR): " << bill[i].getAmount();
            time_t today = time(0);
            time_t billDate = convertDateStringToTime(bill[i].getDate());

            double seconds = difftime(today, billDate);
            double days = seconds / (60 * 60 * 24);
            if (days > 7)
            {
                cout << "|Date: " << bill[i].getDate() << " [OVERDUE]";
            }
            else
                cout << "|Date: " << bill[i].getDate();
        }
    }
    if (found == 0)
    {
        cout << "No Unpaid Bills Found";
        return;
    }
    cout << endl << "========================================";

}

void HandleDischargePatient(Storage<patient>& Patient, Storage <Bill>& bill, FileHandler& h, Storage <Appointment>& appointment, Storage<prescription> &Prescription)
{
    int PID;
    cout << "Enter Patient ID: ";
    PID = PositiveInt();

    if (!Validator::isValidID(PID, Patient)) {
        cout << "Patient not found." << endl;
        return;
    }
    for (int i = 0; i < bill.getSize(); i++)
    {
        if (PID == bill[i].getPatientID()&& bill[i].getStatus()=="unpaid")
        {
            cout << endl << "Cannot Discharge Patient with unpaid Bills";
            return;
        }
    }
    for (int i = 0; i < appointment.getSize(); i++)
    {
        if (PID == appointment[i].getPatientID() && appointment[i].getStatus() == "pending")
        {
            cout << endl << "Cannot Discharge Patient with pending Appointments";
            return;
        }
    }

    //
    h.dischargePatient(PID, Patient, appointment, bill, Prescription);
    Patient.RemoveByID(PID);
    

    for (int i = 0; i < appointment.getSize(); i++)
    {
        if (PID == appointment[i].getPatientID())
        {
            appointment.RemoveByID(appointment[i].getID());
            i--;
        }
    }

    for (int i = 0; i < bill.getSize(); i++)
    {
        if (PID == bill[i].getPatientID())
        {
            bill.RemoveByID(bill[i].getID());
            i--;
        }
    }
        

    for (int i = 0; i < Prescription.getSize(); i++)
    {
        if (PID == Prescription[i].getPatientID())
        {
            Prescription.RemoveByID(Prescription[i].getID());
            i--;
        }
    }
        
    h.updatePatients(Patient);
    h.updateAppointments(appointment);
    h.updateBills(bill);
    h.updatePrescriptions(Prescription);
    cout <<endl<< "Patient discharged and archived successfully";
   
}

void HandleViewSecurityLog(FileHandler& h)
{
    h.ViewSecurityLog();
}

void HandleGenerateDailyReport(Storage<patient>& Patient, Storage <Doctor>& doctor, Storage <Bill>& bill, Storage <Appointment>& appointment)
{
    string TodayDate = getTodayDate();
    int TotalA= 0;
    int PendingA = 0;
    int CompletedA = 0;
    int NoShowA = 0;
    int cancelledA = 0;
    double RevenueCollected = 0;

    for (int i = 0; i < appointment.getSize(); i++)
    {
        if (appointment[i].getDate() == TodayDate)
        {
            TotalA++;
            if (appointment[i].getStatus() == "pending")
            {
                PendingA++;
            }
            else if (appointment[i].getStatus() == "completed")
            {
                CompletedA++;
            }
            else if (appointment[i].getStatus() == "noshow")
            {
                NoShowA++;
            }
            else if (appointment[i].getStatus() == "cancelled")
            {
                cancelledA++;
            }
        }
    }

    for (int i = 0; i < bill.getSize(); i++)
    {
    
        if (bill[i].getStatus() == "paid"&& bill[i].getDate() == TodayDate )
        {
            RevenueCollected+=bill[i].getAmount();
        }
    }

    cout << endl << "Total Appointments Today: " << TotalA;
    cout << "(Pending: " << PendingA << " Completed: " << CompletedA << " No-Show: " << NoShowA << " Cancelled: " << cancelledA<<" ) ";
    cout << endl << "Revenue generated today (paid bills) : " << RevenueCollected;

    int arr[100];
    int count = 0;
    bool found = 0;
    for (int i = 0; i < bill.getSize(); i++)
    {
        if (bill[i].getStatus() == "unpaid")
        {
            found = 1;
            for (int j = 0; j < Patient.getSize(); j++)
            {
                if (bill[i].getPatientID() == Patient[j].getID())
                {
                    arr[count] = j;
                    count++;
                    break;
                }
            }
        }
    }

    cout << endl << "Patients with outstanding unpaid bills: ";
    for (int i = 0; i < count; i++)
    {
        bool found = 0;
        double TotalOwed = 0;
        for (int j = 0; j < bill.getSize(); j++)
        {
            if (Patient[arr[i]].getID() == bill[j].getPatientID()&& bill[j].getStatus() == "unpaid")
            {
                found = 1;
                TotalOwed += bill[j].getAmount();
            }
        }
        if (found == 1)
        {
            cout <<endl<< Patient[arr[i]].getName() << "| " << TotalOwed;
        }
        
    }

    cout << endl << "Doctor wise summary for today: ";
    for (int i = 0; i < doctor.getSize(); i++)
    {
        bool found = 0;
        int completed=0;
        int pending=0;
        int noshow = 0;
        for (int j = 0; j < appointment.getSize(); j++)
        {
            if (doctor[i].getID() == appointment[j].getDoctorID()&& appointment[j].getDate() == TodayDate)
            {
                found = 1;
                if (appointment[j].getStatus() == "completed")
                {
                    completed++;
                }
                else if (appointment[j].getStatus() == "pending")
                {
                    pending++;
                }
                else if (appointment[j].getStatus() == "noshow")
                {
                    noshow++;
                }
            }
        }
        if (found == 1)
        {
            cout << endl << doctor[i].getName() << "| Completed: " << completed << "| Pending: " << pending << "| No-show: " << noshow;
        }
    }
}

int LoginInvalidInt()
{
    int n;
    while (!(cin >> n))
    {
        cout << "Invalid input. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return n;
}

void LogInPatient(Storage<patient>& Patient, Storage<Doctor>& doctor, Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription,
    FileHandler& h)
{
    int attempts = 0;
    int id;
    string password;

    while (attempts < 3)
    {
        cout <<endl<< "Enter ID: ";
        id = PositiveInt();

        cout <<endl<< "Enter Password: ";
        cin >> password;

        patient* p = Patient.findByID(id);

        if (p == nullptr)
        {
            cout <<endl<< "Invalid! This Patient ID does not exist";
            attempts++;
        }
        else if (p->getPassword() != password)
        {
            cout <<endl<< "Invalid Password!";
            attempts++;
        }
        else
        {
            showPatientMenu(*p, doctor, appointment, bill, Prescription, Patient, h);
            return;
        }
    }

    cout <<endl<< "Account locked. Contact Admin.";
    h.WriteToSecurityLog("Patient", id);

}

void showPatientMenu(patient&p, Storage<Doctor>& doctor, Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription, Storage<patient>& Patient, FileHandler& h)
{
    int choice;
    cout <<endl<< "Welcome, " << p.getName();
    
    do {
        cout << endl << "Balance: PKR " << p.getBalance();
        cout << endl << "============================" << endl;
        cout << "1. Book Appointment" << endl;
        cout << "2. Cancel Appointment" << endl;
        cout << "3. View My Appointments" << endl;
        cout << "4. View My Medical Records" << endl;
        cout << "5. View My Bills" << endl;
        cout << "6. Pay Bill" << endl;
        cout << "7. Top Up Balance" << endl;
        cout << "8. Logout" << endl;
        cout << "Enter choice: ";
        choice=PositiveInt();

        if (!Validator::isMenuChoiceValid(choice, 1, 8))
        {
            cout <<endl<< "Invalid choice!";
            continue;
        }

        switch (choice) {
        case 1:
            HandleBookAppointment(p,doctor,appointment,h,bill,Patient);
            break;
        case 2:
            HandleCancelAppointment(p, appointment, h, Patient, doctor, bill);
            break;
        case 3:
            HandleViewMyAppointment(p, appointment, doctor);
            break;
        case 4:
            HandleViewMyMedicalRecords(p, appointment, doctor, Prescription);
            break;
        case 5:
            HandleViewMyBills(p, bill);
            break;
        case 6:
            HandlePayBill(p, bill, h, Patient);
            break;
        case 7:
            HandleTopUpBalance(p, h, Patient);
            break;
        case 8:
            cout <<endl<< "Logging out..." << endl;
            break;
        default:
            cout <<endl<< "Invalid choice." << endl;
            break;
        }

    } while (choice != 8);

}

void showDoctorMenu(Doctor& d,Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription, Storage<patient>& Patient, FileHandler& h)
{
    int choice;
    cout << endl << "Welcome, " << d.getName();
    cout << "   |   Specialization: " << d.getSpecialization();

    do {
       
        cout << endl << "============================" << endl;
        cout << "1. View Today's Appointments" << endl;
        cout << "2. Mark Appointment Complete" << endl;
        cout << "3. Mark Appointment No-Show" << endl;
        cout << "4. Write Prescription" << endl;
        cout << "5. View Patient Medical History" << endl;
        cout << "6. Logout" << endl;
        cout << "Enter choice: ";
        choice = PositiveInt();

        if (!Validator::isMenuChoiceValid(choice, 1, 6))
        {
            cout <<endl<< "Invalid choice!";
            continue;
        }

        switch (choice) {
        case 1:
            HandleViewTodaysAppointment(d, appointment, Patient);
            break;
        case 2:
            HandleMarkAppointmentComplete(d,h,appointment);
            break;
        case 3:
            HandleMarkAppointmentNoShow(d,h,appointment,bill);
            break;
        case 4:
            HandleWritePrescription(d,h,appointment,Prescription);
            break;
        case 5:
            HandleViewPatientMedicalHistory(d,appointment,Patient,Prescription);
            break;
        case 6:
            cout <<endl<< "Logging out..." << endl;
            break;
        default:
            cout <<endl<<"Invalid choice." << endl;
            break;
        }

    } while (choice != 6);

}

void showAdminMenu( Storage<Doctor>& doctor, Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription, Storage<patient>& Patient, FileHandler& h)
{
    int choice;
    cout << endl << "Admin Panel - MediCore ";

    do {
       
        cout << endl << "============================" << endl;
        cout << "1. Add Doctor" << endl;
        cout << "2. Add Patient" << endl;
        cout << "3. Remove Doctor" << endl;
        cout << "4.View All Patients" << endl;
        cout << "5. View All Doctors" << endl;
        cout << "6. View All Appointments" << endl;
        cout << "7. View Unpaid Bills" << endl;
        cout << "8. Discharge Patient" << endl;
        cout << "9. View Security Log" << endl;
        cout << "10. Generate Daily Report" << endl;
        cout << "11. Logout" << endl;
        cout << "Enter choice: ";
        choice = PositiveInt();

        if (!Validator::isMenuChoiceValid(choice, 1, 11))
        {
            cout <<endl<< "Invalid choice!";
            continue;
        }

        switch (choice) {
        case 1:
            HandleAddDoctor(doctor,h);
            break;
        case 2:
            HandleAddPatient(Patient,h);
            break;
        case 3:
            HandleRemoveDoctor(doctor,h,appointment);
            break;
        case 4:
            HandleViewAllPatients(Patient,bill);
            break;
        case 5:
            HandleViewAllDoctors(doctor);
            break;
        case 6:
            HandleViewAllAppointments(appointment);
            break;
        case 7:
            HandleViewUnpaidBills(bill,Patient);
            break;
        case 8:
            HandleDischargePatient(Patient,bill,h,appointment,Prescription);
            break;
        case 9:
            HandleViewSecurityLog(h);
            break;
        case 10:
            HandleGenerateDailyReport(Patient, doctor, bill, appointment);
            break;
        case 11:
            cout <<endl<< "Logging out..." << endl;
            break;
        default:
            cout <<endl<< "Invalid choice." << endl;
            break;
        }

    } while (choice != 11);

}

void showLoginMenu(Storage<patient>& Patient, Storage<Doctor>& doctor,
    Storage<Admin>& admin, Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription,
    FileHandler& h)
{
    int choice;
    do {
        cout << endl << "Welcome to MediCore Hospital Management System";
        cout << endl << "===================================================";
        cout << "Login as:" << endl;
        cout << "1. Patient" << endl;
        cout << "2. Doctor" << endl;
        cout << "3. Admin" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";
        choice = PositiveInt();
        if (!Validator::isMenuChoiceValid(choice, 1, 4))
        {
            cout << "Invalid Choice!Try Again.";
            continue;
        }
        switch (choice) {
        case 1:
            LogInPatient(Patient, doctor,appointment,bill,Prescription,h);
            break;
        case 2:
            LogInDoctor(Patient, doctor, appointment, bill, Prescription, h);
            break;
        case 3:
            LogInAdmin(Patient, doctor, admin, appointment, bill, Prescription, h);
            break;
        case 4:
            cout <<endl<< "GoodBye!" << endl;
            break;
        default:
            cout <<endl<< "Invalid choice." << endl;
            break;
        }

        } while (choice != 4);

    }


void LogInDoctor(Storage<patient>& Patient, Storage<Doctor>& doctor,
     Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription,
    FileHandler& h)
{
    int attempts = 0;
    int id;
    string password;

    while (attempts < 3)
    {
        cout <<endl<< "Enter ID: ";
        id = PositiveInt();

        cout <<endl<< "Enter Password: ";
        cin >> password;

        Doctor* d = doctor.findByID(id);

        if (d == nullptr)
        {
            cout <<endl<< "Invalid! This Doctor ID does not exist";
            attempts++;
        }
        else if (d->getPassword() != password)
        {
            cout <<endl<< "Invalid Password!";
            attempts++;
        }
        else
        {
            showDoctorMenu(*d,appointment,bill,Prescription,Patient,h);
            return;
        }
    }

    cout << "Account locked. Contact Admin.";
    h.WriteToSecurityLog("Doctor", id);

}


void LogInAdmin(Storage<patient>& Patient, Storage<Doctor>& doctor,
    Storage<Admin>& admin, Storage<Appointment>& appointment,
    Storage<Bill>& bill, Storage<prescription>& Prescription,
    FileHandler& h)
{
    int attempts = 0;
    int id;
    string password;

    while (attempts < 3)
    {
        cout <<endl<< "Enter ID: ";
        id = PositiveInt();

        cout <<endl<< "Enter Password: ";
        cin >> password;

        Admin* a = admin.findByID(id);

        if (a== nullptr)
        {
            cout <<endl<< "Invalid! This Admin ID does not exist";
            attempts++;
        }
        else if (a->getPassword() != password)
        {
            cout <<endl<< "Invalid Password!";
            attempts++;
        }
        else
        {
            showAdminMenu(doctor,appointment,bill,Prescription,Patient,h);
            return;
        }
    }

    cout << "Account locked. Contact Admin.";
    h.WriteToSecurityLog("Admin", id);

}
