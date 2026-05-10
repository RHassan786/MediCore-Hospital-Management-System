# MediCore Hospital Management System

MediCore Hospital Management System is a C++ object-oriented hospital management project with an SFML graphical user interface. It supports patient, doctor, and admin roles, stores records in text files, and provides appointment, billing, prescription, doctor, patient, and admin management features.

## GitHub Repository Link

Replace this line with your public GitHub repository link after uploading the project:

`https://github.com/RHassan786/MediCore-Hospital-Management-System.git`

## Features

- Role-based login for Patient, Doctor, and Admin.
- Patient functions:
  - Book appointments.
  - Cancel pending appointments.
  - View appointments.
  - View medical records.
  - View and pay bills.
  - Add balance.
- Doctor functions:
  - View today's appointments.
  - Mark appointments as completed.
  - Mark appointments as no-show.
  - Write prescriptions.
  - View patient medical history.
- Admin functions:
  - Add doctors.
  - Add patients.
  - Remove doctors.
  - View all patients.
  - View all doctors.
  - View all appointments.
  - View unpaid bills.
  - Discharge patients.
  - View security login log.
  - Generate daily summary report.

## Technologies Used

- C++
- Object-Oriented Programming
- SFML Graphics Library
- Text-file based storage
- Visual Studio

## Project Structure

- `Main.cpp` starts the program and loads all data files.
- `GUI.h` and `GUI.cpp` contain the SFML graphical interface.
- `menus.h` and `menus.cpp` contain the main role actions and business logic.
- `FileHandler.h` and `FileHandler.cpp` handle loading, saving, and updating text files.
- `Storage.h` stores project records.
- `patient`, `Doctor`, `Admin`, `Appointment`, `Bill`, and `prescription` classes are separated into their own `.h` and `.cpp` files.
- `Data/` contains the text files used by the program.

## Data Files

The program reads and writes data from the `Data` folder:

- `patients.txt`
- `doctors.txt`
- `admin.txt`
- `appointments.txt`
- `bills.txt`
- `prescriptions.txt`
- `discharged.txt`
- `security_log.txt`

The files must remain in the `Data` folder so the program can load the records correctly.

## How to Compile and Run

1. Install Visual Studio with C++ desktop development support.
2. Install SFML and place it at:

   `C:\SFML`

3. Make sure these folders exist:

   `C:\SFML\include`

   `C:\SFML\lib`

4. Open the project in Visual Studio using:

   `Medicore Hospital Management System.vcxproj`

5. Select the `Debug` configuration.
6. Select the same platform used by the SFML libraries, preferably `x64` if SFML is installed for x64.
7. Build the project.
8. Run the project from Visual Studio.

The SFML DLL files must be available beside the executable or in the debug output folder. The project already contains debug output folders with SFML DLLs, but if the program fails to start because of a missing SFML DLL, copy these files from `C:\SFML\bin` into the folder where the `.exe` is generated:

- `sfml-graphics-d-2.dll`
- `sfml-window-d-2.dll`
- `sfml-system-d-2.dll`

## Login Details

Sample admin accounts are stored in `Data/admin.txt`.

Example:

- Admin ID: `1`
- Password: `admin123`

Other patient and doctor login records are stored in `Data/patients.txt` and `Data/doctors.txt`.

## Submission Notes

Submit one `.zip` file named exactly according to your roll number, for example:

`22L-1234.zip`

The zip file should include:

- All `.h` files.
- All `.cpp` files.
- `README.md`
- `Data` folder.
- Visual Studio project files.
- Required font and resource files.

Do not submit only the `.exe` file because the evaluator needs the source code.
