#include "GUI.h"
#include <exception>
#include <algorithm>
#include <cctype>

using namespace std;

namespace {
    const float APP_W = 1280.f;
    const float APP_H = 800.f;
    const float TOPBAR_H = 86.f;
    const float SIDEBAR_W = 330.f;
    const float CONTENT_X = SIDEBAR_W + 16.f;
    const float CONTENT_Y = TOPBAR_H + 14.f;
    const float CONTENT_W = APP_W - CONTENT_X - 14.f;
    const float CONTENT_H = 470.f;
    const float ACTION_Y = CONTENT_Y + CONTENT_H + 12.f;
    const float ACTION_H = APP_H - ACTION_Y - 10.f;
}

GUI::GUI(Storage<patient>& p, Storage<Doctor>& d, Storage<Admin>& a,
    Storage<Appointment>& ap, Storage<Bill>& b,
    Storage<prescription>& pr, FileHandler& fileHandler)
    : window(sf::VideoMode((unsigned int)APP_W, (unsigned int)APP_H), "MediCore Hospital Management System",
        sf::Style::Close | sf::Style::Titlebar),
    currentScreen(Screen::LOGIN_MENU) {

    patients = &p;
    doctors = &d;
    admins = &a;
    appointments = &ap;
    bills = &b;
    prescriptions = &pr;
    fh = &fileHandler;

    window.setFramerateLimit(60);

    if (!font.loadFromFile("Roboto_Condensed-Regular.ttf")) {
        font.loadFromFile("arial.ttf");
    }

    setupInput(idBox, 460.f, 320.f, 360.f, 46.f, "Enter ID");
    setupInput(passwordBox, 460.f, 396.f, 360.f, 46.f, "Enter Password", true);
    setupInput(actionInputBox, CONTENT_X + 18.f, ACTION_Y + 80.f, CONTENT_W - 190.f, 44.f, "Enter required input...");
}

void GUI::setupButton(Button& btn, float x, float y, float w, float h, const string& label, unsigned int fontSize) {
    btn.shape.setPosition(x, y);
    btn.shape.setSize({ w, h });
    btn.shape.setFillColor(sf::Color(20, 184, 166));
    btn.shape.setOutlineThickness(0.f);

    btn.text.setFont(font);
    btn.text.setString(label);
    btn.text.setCharacterSize(fontSize);
    btn.text.setFillColor(WHITE_COLOR);

    sf::FloatRect tb = btn.text.getLocalBounds();
    btn.text.setPosition(x + (w - tb.width) / 2.f - tb.left, y + (h - tb.height) / 2.f - tb.top);
}

void GUI::setupInput(InputBox& box, float x, float y, float w, float h, const string& ph, bool isPass) {
    box.shape.setPosition(x, y);
    box.shape.setSize({ w, h });
    box.shape.setFillColor(sf::Color(22, 50, 58));
    box.shape.setOutlineThickness(2.f);
    box.shape.setOutlineColor(sf::Color(67, 138, 151));
    box.isPassword = isPass;

    box.text.setFont(font);
    box.text.setCharacterSize(18);
    box.text.setFillColor(WHITE_COLOR);
    box.text.setPosition(x + 12.f, y + 11.f);

    box.placeholder.setFont(font);
    box.placeholder.setString(ph);
    box.placeholder.setCharacterSize(16);
    box.placeholder.setFillColor(sf::Color(145, 197, 204));
    box.placeholder.setPosition(x + 12.f, y + 12.f);
}

sf::Text GUI::makeText(const string& str, float x, float y, unsigned int size, sf::Color color) {
    sf::Text t;
    t.setFont(font);
    t.setString(str);
    t.setCharacterSize(size);
    t.setFillColor(color);
    t.setPosition(x, y);
    return t;
}

void GUI::showMessage(const string& msg, sf::Color color) {
    message = msg;
    messageColor = color;
    messageClock.restart();
}

void GUI::drawTitle(const string& title, float) {
    for (int i = 0; i < (int)TOPBAR_H; ++i) {
        sf::RectangleShape line;
        line.setPosition(0.f, (float)i);
        line.setSize({ APP_W, 1.f });
        line.setFillColor(sf::Color(8 + i / 8, 25 + i / 3, 30 + i / 3));
        window.draw(line);
    }

    sf::CircleShape badge(24.f);
    badge.setPosition(18.f, 18.f);
    badge.setFillColor(sf::Color(45, 212, 191));
    window.draw(badge);
    window.draw(makeText("M", 33.f, 22.f, 28, sf::Color(6, 40, 41)));

    window.draw(makeText(title, 74.f, 19.f, 28, WHITE_COLOR));
    window.draw(makeText("MediCore Hospital Management System", 76.f, 54.f, 14, sf::Color(156, 220, 212)));

    sf::RectangleShape barLine;
    barLine.setPosition(0.f, TOPBAR_H - 1.f);
    barLine.setSize({ APP_W, 1.f });
    barLine.setFillColor(sf::Color(70, 157, 156));
    window.draw(barLine);
}

void GUI::drawPanel(float x, float y, float w, float h) {
    sf::RectangleShape shadow;
    shadow.setPosition(x + 3.f, y + 4.f);
    shadow.setSize({ w, h });
    shadow.setFillColor(sf::Color(0, 0, 0, 70));
    window.draw(shadow);

    sf::RectangleShape panel;
    panel.setPosition(x, y);
    panel.setSize({ w, h });
    panel.setFillColor(sf::Color(17, 36, 45));
    panel.setOutlineThickness(1.5f);
    panel.setOutlineColor(sf::Color(59, 130, 138));
    window.draw(panel);
}

bool GUI::actionRequiresInput(PendingAction action) {
    return action != PendingAction::NONE;
}

string GUI::getActionPrompt(PendingAction action) {
    switch (action) {
    case PendingAction::PATIENT_BOOK_APPOINTMENT: return "Format: specialization|doctorId|DD-MM-YYYY|HH:MM";
    case PendingAction::PATIENT_CANCEL_APPOINTMENT: return "Format: appointmentId";
    case PendingAction::PATIENT_PAY_BILL: return "Format: billId";
    case PendingAction::PATIENT_TOP_UP: return "Format: amount";
    case PendingAction::DOCTOR_MARK_COMPLETE: return "Format: appointmentId";
    case PendingAction::DOCTOR_MARK_NO_SHOW: return "Format: appointmentId";
    case PendingAction::DOCTOR_WRITE_PRESCRIPTION: return "Format: appointmentId|medicines|notes";
    case PendingAction::DOCTOR_VIEW_HISTORY: return "Format: patientId";
    case PendingAction::ADMIN_ADD_PATIENT: return "Format: name|age|gender|contact(11 digits)|password|balance";
    case PendingAction::ADMIN_ADD_DOCTOR: return "Format: name|specialization|contact(11 digits)|password|fee";
    case PendingAction::ADMIN_REMOVE_DOCTOR: return "Format: doctorId";
    case PendingAction::ADMIN_DISCHARGE_PATIENT: return "Format: patientId";
    default: return "";
    }
}

vector<pair<string, string>> GUI::getActionFields(PendingAction action) {
    switch (action) {
    case PendingAction::PATIENT_BOOK_APPOINTMENT:
        return { {"Specialization", "Example: cardio"}, {"Doctor ID", "Numeric ID"}, {"Date", "DD-MM-YYYY"}, {"Time Slot", "HH:MM"} };
    case PendingAction::PATIENT_CANCEL_APPOINTMENT:
        return { {"Appointment ID", "Numeric ID"} };
    case PendingAction::PATIENT_PAY_BILL:
        return { {"Bill ID", "Numeric ID"} };
    case PendingAction::PATIENT_TOP_UP:
        return { {"Amount", "e.g. 5000"} };
    case PendingAction::DOCTOR_MARK_COMPLETE:
        return { {"Appointment ID", "Numeric ID"} };
    case PendingAction::DOCTOR_MARK_NO_SHOW:
        return { {"Appointment ID", "Numeric ID"} };
    case PendingAction::DOCTOR_WRITE_PRESCRIPTION:
        return { {"Appointment ID", "Numeric ID"}, {"Medicines", "Comma-separated if needed"}, {"Notes", "Short clinical note"} };
    case PendingAction::DOCTOR_VIEW_HISTORY:
        return { {"Patient ID", "Numeric ID"} };
    case PendingAction::ADMIN_ADD_PATIENT:
        return { {"Patient Name", "Full name"}, {"Age", "Positive number"}, {"Gender", "e.g. Male"}, {"Contact Number", "11 digits"}, {"Password", "Minimum 6 chars"}, {"Initial Balance", "e.g. 5000"} };
    case PendingAction::ADMIN_ADD_DOCTOR:
        return { {"Doctor Name", "Full name"}, {"Specialization", "e.g. Cardiologist"}, {"Contact Number", "11 digits"}, {"Password", "Minimum 6 chars"}, {"Consultation Fee", "e.g. 2500"} };
    case PendingAction::ADMIN_REMOVE_DOCTOR:
        return { {"Doctor ID", "Numeric ID"} };
    case PendingAction::ADMIN_DISCHARGE_PATIENT:
        return { {"Patient ID", "Numeric ID"} };
    default:
        return {};
    }
}

vector<string> GUI::splitInput(const string& input, char delimiter) {
    vector<string> parts;
    string item;
    istringstream ss(input);
    while (getline(ss, item, delimiter)) parts.push_back(item);
    return parts;
}

string GUI::buildScriptFromPendingValues() {
    ostringstream ss;
    for (size_t i = 0; i < pendingValues.size(); ++i) {
        ss << pendingValues[i] << "\n";
    }
    return ss.str();
}

bool GUI::isPositiveIntString(const string& s) {
    if (s.empty()) return false;
    for (size_t i = 0; i < s.size(); ++i) {
        if (!isdigit((unsigned char)s[i])) return false;
    }
    try {
        long long v = stoll(s);
        return v > 0 && v <= 2147483647LL;
    }
    catch (...) {
        return false;
    }
}

bool GUI::isPositiveDoubleString(const string& s) {
    if (s.empty()) return false;
    bool dotSeen = false;
    int digitCount = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '.') {
            if (dotSeen) return false;
            dotSeen = true;
        }
        else if (isdigit((unsigned char)c)) {
            digitCount++;
        }
        else {
            return false;
        }
    }
    if (digitCount == 0) return false;
    try {
        return stod(s) > 0.0;
    }
    catch (...) {
        return false;
    }
}

bool GUI::validateFieldForAction(PendingAction action, int fieldIndex, const string& value, string& errorMsg) {
    auto isDigitsOnly = [&](const string& s) -> bool {
        if (s.empty()) return false;
        for (size_t i = 0; i < s.size(); ++i) {
            if (!isdigit((unsigned char)s[i])) return false;
        }
        return true;
        };

    auto requirePositiveInt = [&](const string& fieldName) -> bool {
        if (!isPositiveIntString(value)) {
            errorMsg = fieldName + " must be a positive integer.";
            return false;
        }
        return true;
        };

    auto requirePositiveDouble = [&](const string& fieldName) -> bool {
        if (!isPositiveDoubleString(value)) {
            errorMsg = fieldName + " must be a positive number.";
            return false;
        }
        return true;
        };

    if (action == PendingAction::PATIENT_BOOK_APPOINTMENT) {
        if (fieldIndex == 0) {
            if (value.empty()) {
                errorMsg = "Specialization cannot be empty.";
                return false;
            }
            return true;
        }
        if (fieldIndex == 1) return requirePositiveInt("Doctor ID");
        if (fieldIndex == 2) {
            if (!Validator::isDateValid(value)) {
                errorMsg = "Date must be in valid DD-MM-YYYY format.";
                return false;
            }
            return true;
        }
        if (fieldIndex == 3) {
            if (!Validator::isTimeSlotValid(value)) {
                errorMsg = "Time slot must be between 09:00 and 16:00.";
                return false;
            }
            return true;
        }
        return true;
    }
    if (action == PendingAction::PATIENT_CANCEL_APPOINTMENT) {
        return requirePositiveInt("Appointment ID");
    }
    if (action == PendingAction::PATIENT_PAY_BILL) {
        return requirePositiveInt("Bill ID");
    }
    if (action == PendingAction::PATIENT_TOP_UP) {
        return requirePositiveDouble("Amount");
    }
    if (action == PendingAction::DOCTOR_MARK_COMPLETE || action == PendingAction::DOCTOR_MARK_NO_SHOW) {
        return requirePositiveInt("Appointment ID");
    }
    if (action == PendingAction::DOCTOR_WRITE_PRESCRIPTION) {
        if (fieldIndex == 0) return requirePositiveInt("Appointment ID");
        if (value.empty()) {
            errorMsg = "This field cannot be empty.";
            return false;
        }
        return true;
    }
    if (action == PendingAction::DOCTOR_VIEW_HISTORY) {
        return requirePositiveInt("Patient ID");
    }
    if (action == PendingAction::ADMIN_ADD_PATIENT) {
        if (fieldIndex == 0) {
            if (value.empty()) {
                errorMsg = "Patient Name cannot be empty.";
                return false;
            }
            return true;
        }
        if (fieldIndex == 1) return requirePositiveInt("Age");
        if (fieldIndex == 2) {
            if (value.empty()) {
                errorMsg = "Gender cannot be empty.";
                return false;
            }
            return true;
        }
        if (fieldIndex == 3) {
            if (!Validator::isContactNumberValid(value)) {
                errorMsg = "Contact Number must be exactly 11 digits.";
                return false;
            }
            return true;
        }
        if (fieldIndex == 4) {
            if (!Validator::isPasswordValid(value)) {
                errorMsg = "Password must be at least 6 characters.";
                return false;
            }
            return true;
        }
        if (fieldIndex == 5) return requirePositiveDouble("Initial Balance");
        return true;
    }
    if (action == PendingAction::ADMIN_ADD_DOCTOR) {
        if (fieldIndex == 0) {
            if (value.empty()) {
                errorMsg = "Doctor Name cannot be empty.";
                return false;
            }
            return true;
        }
        if (fieldIndex == 1) {
            if (value.empty()) {
                errorMsg = "Specialization cannot be empty.";
                return false;
            }
            return true;
        }
        if (fieldIndex == 2) {
            if (!Validator::isContactNumberValid(value)) {
                errorMsg = "Contact Number must be exactly 11 digits.";
                return false;
            }
            return true;
        }
        if (fieldIndex == 3) {
            if (!Validator::isPasswordValid(value)) {
                errorMsg = "Password must be at least 6 characters.";
                return false;
            }
            return true;
        }
        if (fieldIndex == 4) return requirePositiveDouble("Consultation Fee");
        return true;
    }
    if (action == PendingAction::ADMIN_REMOVE_DOCTOR) {
        return requirePositiveInt("Doctor ID");
    }
    if (action == PendingAction::ADMIN_DISCHARGE_PATIENT) {
        return requirePositiveInt("Patient ID");
    }
    return true;
}

string GUI::normalizeOutputText(const string& raw) {
    string s = raw;

    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '|') {
            s[i] = '\n';
        }
    }

    const string token = "Patient ID:";
    size_t pos = 0;
    while ((pos = s.find(token, pos)) != string::npos) {
        if (pos > 1 && s[pos - 1] != '\n') {
            s.insert(pos, "\n");
            pos += token.size() + 1;
        }
        else {
            pos += token.size();
        }
    }

    const string recToken = "Appointment ID:";
    pos = 0;
    while ((pos = s.find(recToken, pos)) != string::npos) {
        if (pos > 1 && s[pos - 1] != '\n') {
            s.insert(pos, "\n");
            pos += recToken.size() + 1;
        }
        else {
            pos += recToken.size();
        }
    }

    // Remove interactive console prompts from captured output.
    const vector<string> promptNoise = {
        "Enter Doctor's Name:",
        "Enter Patient's Name:",
        "Enter Specialization",
        "Enter Patient's Age:",
        "Enter Gender",
        "Enter Contact Number (Should be 11 digits long):",
        "Enter Password (Minimum 6 characters):",
        "Enter Consultation Fee:",
        "Enter Initial Balance:",
        "Enter ID:",
        "Enter Password:",
        "Enter Amount to Add (PKR):",
        "Enter Appointment ID:",
        "Enter Patient ID:",
        "Enter Bill ID to pay:",
        "Enter Date (DD-MM-YYYY):",
        "Enter Time Slot:",
        "Available slots: 09:00 10:00 11:00 12:00 13:00 14:00 15:00 16:00",
        "Enter Medicines:",
        "Enter Notes:",
        "Enter Specialization to search:",
        "Enter Doctor ID:",
        "Enter Doctor ID to remove:"
    };

    for (size_t i = 0; i < promptNoise.size(); ++i) {
        size_t p = 0;
        while ((p = s.find(promptNoise[i], p)) != string::npos) {
            s.erase(p, promptNoise[i].size());
        }
    }

    // Collapse excessive blank lines after prompt cleanup.
    while (s.find("\n\n\n") != string::npos) {
        s.replace(s.find("\n\n\n"), 3, "\n\n");
    }

    return s;
}

string GUI::wrapOutputText(const string& input, size_t maxCharsPerLine) {
    istringstream all(input);
    string line;
    ostringstream out;

    while (getline(all, line)) {
        if (line.size() <= maxCharsPerLine) {
            out << line << '\n';
            continue;
        }

        istringstream words(line);
        string word;
        string current;
        while (words >> word) {
            if (current.empty()) {
                current = word;
            }
            else if (current.size() + 1 + word.size() <= maxCharsPerLine) {
                current += " " + word;
            }
            else {
                out << current << '\n';
                current = word;
            }
        }
        if (!current.empty()) out << current << '\n';
    }

    return out.str();
}

string GUI::runConsoleAction(const string& scriptedInput, const function<void()>& actionFn) {
    streambuf* oldCin = cin.rdbuf();
    streambuf* oldCout = cout.rdbuf();
    istringstream fakeIn(scriptedInput);
    ostringstream fakeOut;
    cin.rdbuf(fakeIn.rdbuf());
    cout.rdbuf(fakeOut.rdbuf());

    try { actionFn(); }
    catch (HospitalException& e) { fakeOut << endl << e.what(); }
    catch (exception& e) { fakeOut << endl << "Error: " << e.what(); }
    catch (...) { fakeOut << endl << "An unexpected error occurred."; }

    cin.rdbuf(oldCin);
    cout.rdbuf(oldCout);
    string normalized = normalizeOutputText(fakeOut.str());
    return wrapOutputText(normalized, 95);
}

string GUI::buildActionPreview(PendingAction action) {
    ostringstream out;

    if (action == PendingAction::ADMIN_REMOVE_DOCTOR) {
        out << "Displaying all doctors before removal:\n";
        if (doctors->getSize() == 0) out << "No doctors found.\n";
        for (int i = 0; i < doctors->getSize(); ++i) {
            out << "\nDoctor ID: " << (*doctors)[i].getID()
                << "\nDoctor Name: " << (*doctors)[i].getName()
                << "\nDoctor Specialization: " << (*doctors)[i].getSpecialization()
                << "\nDoctor Contact: " << (*doctors)[i].getContact()
                << "\nDoctor Fee: " << (*doctors)[i].getFee() << "\n";
        }
    }
    else if (action == PendingAction::PATIENT_CANCEL_APPOINTMENT && currentPatient) {
        out << "Pending appointments available to cancel:\n";
        bool found = false;
        for (int i = 0; i < appointments->getSize(); ++i) {
            if ((*appointments)[i].getPatientID() == currentPatient->getID() &&
                (*appointments)[i].getStatus() == "pending") {
                found = true;
                out << "\nAppointment ID: " << (*appointments)[i].getID()
                    << "\nDoctor ID: " << (*appointments)[i].getDoctorID()
                    << "\nDate: " << (*appointments)[i].getDate()
                    << "\nTime Slot: " << (*appointments)[i].getTimeslot() << "\n";
            }
        }
        if (!found) out << "No pending appointments.\n";
    }
    else if (action == PendingAction::PATIENT_PAY_BILL && currentPatient) {
        out << "Unpaid bills available to pay:\n";
        bool found = false;
        for (int i = 0; i < bills->getSize(); ++i) {
            if ((*bills)[i].getPatientID() == currentPatient->getID() &&
                (*bills)[i].getStatus() == "unpaid") {
                found = true;
                out << "\nBill ID: " << (*bills)[i].getID()
                    << "\nAppointment ID: " << (*bills)[i].getAppointmentID()
                    << "\nAmount: PKR " << (*bills)[i].getAmount()
                    << "\nDate: " << (*bills)[i].getDate() << "\n";
            }
        }
        if (!found) out << "No unpaid bills.\n";
    }
    else if ((action == PendingAction::DOCTOR_MARK_COMPLETE ||
        action == PendingAction::DOCTOR_MARK_NO_SHOW) && currentDoctor) {
        out << "Today's pending appointments:\n";
        bool found = false;
        string today = getTodayDate();
        for (int i = 0; i < appointments->getSize(); ++i) {
            if ((*appointments)[i].getDoctorID() == currentDoctor->getID() &&
                (*appointments)[i].getDate() == today &&
                (*appointments)[i].getStatus() == "pending") {
                found = true;
                out << "\nAppointment ID: " << (*appointments)[i].getID()
                    << "\nPatient ID: " << (*appointments)[i].getPatientID()
                    << "\nTime Slot: " << (*appointments)[i].getTimeslot() << "\n";
            }
        }
        if (!found) out << "No pending appointments today.\n";
    }
    else if (action == PendingAction::DOCTOR_WRITE_PRESCRIPTION && currentDoctor) {
        out << "Completed appointments available for prescription:\n";
        bool found = false;
        for (int i = 0; i < appointments->getSize(); ++i) {
            if ((*appointments)[i].getDoctorID() == currentDoctor->getID() &&
                (*appointments)[i].getStatus() == "completed") {
                found = true;
                out << "\nAppointment ID: " << (*appointments)[i].getID()
                    << "\nPatient ID: " << (*appointments)[i].getPatientID()
                    << "\nDate: " << (*appointments)[i].getDate()
                    << "\nTime Slot: " << (*appointments)[i].getTimeslot() << "\n";
            }
        }
        if (!found) out << "No completed appointments found.\n";
    }
    else if (action == PendingAction::ADMIN_DISCHARGE_PATIENT) {
        out << "Registered patients:\n";
        if (patients->getSize() == 0) out << "No patients found.\n";
        for (int i = 0; i < patients->getSize(); ++i) {
            out << "\nPatient ID: " << (*patients)[i].getID()
                << "\nPatient Name: " << (*patients)[i].getName()
                << "\nContact: " << (*patients)[i].getContact()
                << "\nBalance: PKR " << (*patients)[i].getBalance() << "\n";
        }
    }
    else if (action == PendingAction::PATIENT_BOOK_APPOINTMENT && pendingValues.size() >= 1) {
        string specialization = toLowerCase(pendingValues[0]);
        out << "Doctors matching specialization: " << pendingValues[0] << "\n";
        bool found = false;
        for (int i = 0; i < doctors->getSize(); ++i) {
            if (toLowerCase((*doctors)[i].getSpecialization()) == specialization) {
                found = true;
                out << "\nDoctor ID: " << (*doctors)[i].getID()
                    << "\nDoctor Name: " << (*doctors)[i].getName()
                    << "\nSpecialization: " << (*doctors)[i].getSpecialization()
                    << "\nFee: PKR " << (*doctors)[i].getFee() << "\n";
            }
        }
        if (!found) out << "No doctors found for that specialization.\n";
    }

    string preview = out.str();
    if (preview.empty()) return "";
    return wrapOutputText(preview, 95);
}

void GUI::refreshActionPreview() {
    string preview = buildActionPreview(pendingAction);
    if (!preview.empty()) {
        displayContent = preview;
        scrollOffset = 0.f;
    }
}

void GUI::closePendingAction() {
    pendingAction = PendingAction::NONE;
    actionInputBox.value = "";
    actionInputBox.isActive = false;
    pendingFields.clear();
    pendingValues.clear();
    currentFieldIndex = 0;
}

void GUI::beginAction(PendingAction action, const string& title) {
    pendingAction = action;
    pendingFields = getActionFields(action);
    pendingValues.clear();
    currentFieldIndex = 0;
    actionInputBox.value = "";
    actionInputBox.isActive = true;
    displayContent = "Action: " + title + "\nPlease fill the fields below step by step.";
    refreshActionPreview();
    scrollOffset = 0.f;
}

void GUI::executePatientAction(int index) {
    if (index == 0) { beginAction(PendingAction::PATIENT_BOOK_APPOINTMENT, "Book New Appointment"); return; }
    if (index == 1) { beginAction(PendingAction::PATIENT_CANCEL_APPOINTMENT, "Cancel Pending Appointment"); return; }
    if (index == 2) { displayContent = runConsoleAction("", [&]() { HandleViewMyAppointment(*currentPatient, *appointments, *doctors); }); return; }
    if (index == 3) { displayContent = runConsoleAction("", [&]() { HandleViewMyMedicalRecords(*currentPatient, *appointments, *doctors, *prescriptions); }); return; }
    if (index == 4) { displayContent = runConsoleAction("", [&]() { HandleViewMyBills(*currentPatient, *bills); }); return; }
    if (index == 5) { beginAction(PendingAction::PATIENT_PAY_BILL, "Pay Unpaid Bill"); return; }
    if (index == 6) { beginAction(PendingAction::PATIENT_TOP_UP, "Add Wallet Balance"); return; }
    if (index == 7) { currentPatient = nullptr; currentScreen = Screen::LOGIN_MENU; closePendingAction(); }
}

void GUI::executeDoctorAction(int index) {
    if (index == 0) { displayContent = runConsoleAction("", [&]() { HandleViewTodaysAppointment(*currentDoctor, *appointments, *patients); }); return; }
    if (index == 1) { beginAction(PendingAction::DOCTOR_MARK_COMPLETE, "Mark Appointment Complete"); return; }
    if (index == 2) { beginAction(PendingAction::DOCTOR_MARK_NO_SHOW, "Mark Appointment No-Show"); return; }
    if (index == 3) { beginAction(PendingAction::DOCTOR_WRITE_PRESCRIPTION, "Write New Prescription"); return; }
    if (index == 4) { beginAction(PendingAction::DOCTOR_VIEW_HISTORY, "View Patient History"); return; }
    if (index == 5) { currentDoctor = nullptr; currentScreen = Screen::LOGIN_MENU; closePendingAction(); }
}

void GUI::executeAdminAction(int index) {
    if (index == 0) { beginAction(PendingAction::ADMIN_ADD_DOCTOR, "Add Doctor Profile"); return; }
    if (index == 1) { beginAction(PendingAction::ADMIN_ADD_PATIENT, "Add Patient Profile"); return; }
    if (index == 2) { beginAction(PendingAction::ADMIN_REMOVE_DOCTOR, "Remove Doctor Profile"); return; }
    if (index == 3) { displayContent = runConsoleAction("", [&]() { HandleViewAllPatients(*patients, *bills); }); return; }
    if (index == 4) { displayContent = runConsoleAction("", [&]() { HandleViewAllDoctors(*doctors); }); return; }
    if (index == 5) { displayContent = runConsoleAction("", [&]() { HandleViewAllAppointments(*appointments); }); return; }
    if (index == 6) { displayContent = runConsoleAction("", [&]() { HandleViewUnpaidBills(*bills, *patients); }); return; }
    if (index == 7) { beginAction(PendingAction::ADMIN_DISCHARGE_PATIENT, "Discharge Patient & Archive"); return; }
    if (index == 8) { displayContent = runConsoleAction("", [&]() { HandleViewSecurityLog(*fh); }); return; }
    if (index == 9) { displayContent = runConsoleAction("", [&]() { HandleGenerateDailyReport(*patients, *doctors, *bills, *appointments); }); return; }
    if (index == 10) { currentAdmin = nullptr; currentScreen = Screen::LOGIN_MENU; closePendingAction(); }
}

void GUI::tryExecutePendingAction() {
    if (pendingAction == PendingAction::NONE) return;

    string entry = actionInputBox.value;
    size_t left = entry.find_first_not_of(" \t\r\n");
    size_t right = entry.find_last_not_of(" \t\r\n");
    if (left == string::npos) entry = "";
    else entry = entry.substr(left, right - left + 1);
    if (entry.empty() || all_of(entry.begin(), entry.end(), [](unsigned char c) { return isspace(c); })) {
        showMessage("Please enter a value before continuing.", ERROR_COLOR);
        return;
    }

    string validationError;
    if (!validateFieldForAction(pendingAction, currentFieldIndex, entry, validationError)) {
        showMessage(validationError, ERROR_COLOR);
        return;
    }

    pendingValues.push_back(entry);
    actionInputBox.value = "";
    currentFieldIndex++;

    if (currentFieldIndex < (int)pendingFields.size()) {
        refreshActionPreview();
        showMessage("Captured. Enter next field.", SUCCESS_COLOR);
        return;
    }

    string script;

    if (pendingAction == PendingAction::PATIENT_BOOK_APPOINTMENT) {
        script = buildScriptFromPendingValues();
        displayContent = runConsoleAction(script, [&]() { HandleBookAppointment(*currentPatient, *doctors, *appointments, *fh, *bills, *patients); });
    }
    else if (pendingAction == PendingAction::PATIENT_CANCEL_APPOINTMENT) {
        script = buildScriptFromPendingValues();
        displayContent = runConsoleAction(script, [&]() { HandleCancelAppointment(*currentPatient, *appointments, *fh, *patients, *doctors, *bills); });
    }
    else if (pendingAction == PendingAction::PATIENT_PAY_BILL) {
        script = buildScriptFromPendingValues();
        displayContent = runConsoleAction(script, [&]() { HandlePayBill(*currentPatient, *bills, *fh, *patients); });
    }
    else if (pendingAction == PendingAction::PATIENT_TOP_UP) {
        script = buildScriptFromPendingValues();
        displayContent = runConsoleAction(script, [&]() { HandleTopUpBalance(*currentPatient, *fh, *patients); });
    }
    else if (pendingAction == PendingAction::DOCTOR_MARK_COMPLETE) {
        script = buildScriptFromPendingValues();
        displayContent = runConsoleAction(script, [&]() { HandleMarkAppointmentComplete(*currentDoctor, *fh, *appointments); });
    }
    else if (pendingAction == PendingAction::DOCTOR_MARK_NO_SHOW) {
        script = buildScriptFromPendingValues();
        displayContent = runConsoleAction(script, [&]() { HandleMarkAppointmentNoShow(*currentDoctor, *fh, *appointments, *bills); });
    }
    else if (pendingAction == PendingAction::DOCTOR_WRITE_PRESCRIPTION) {
        script = buildScriptFromPendingValues();
        displayContent = runConsoleAction(script, [&]() { HandleWritePrescription(*currentDoctor, *fh, *appointments, *prescriptions); });
    }
    else if (pendingAction == PendingAction::DOCTOR_VIEW_HISTORY) {
        script = buildScriptFromPendingValues();
        displayContent = runConsoleAction(script, [&]() { HandleViewPatientMedicalHistory(*currentDoctor, *appointments, *patients, *prescriptions); });
    }
    else if (pendingAction == PendingAction::ADMIN_ADD_PATIENT) {
        script = "\n" + buildScriptFromPendingValues();
        displayContent = runConsoleAction(script, [&]() { HandleAddPatient(*patients, *fh); });
    }
    else if (pendingAction == PendingAction::ADMIN_ADD_DOCTOR) {
        script = "\n" + buildScriptFromPendingValues();
        displayContent = runConsoleAction(script, [&]() { HandleAddDoctor(*doctors, *fh); });
    }
    else if (pendingAction == PendingAction::ADMIN_REMOVE_DOCTOR) {
        script = buildScriptFromPendingValues();
        displayContent = runConsoleAction(script, [&]() { HandleRemoveDoctor(*doctors, *fh, *appointments); });
    }
    else if (pendingAction == PendingAction::ADMIN_DISCHARGE_PATIENT) {
        script = buildScriptFromPendingValues();
        displayContent = runConsoleAction(script, [&]() { HandleDischargePatient(*patients, *bills, *fh, *appointments, *prescriptions); });
    }

    scrollOffset = 0.f;
    showMessage("Action completed.", SUCCESS_COLOR);
    closePendingAction();
}

void GUI::drawLoginMenu() {
    window.clear(sf::Color(9, 24, 30));
    drawTitle("MediCore Hospital Management System");

    drawPanel(350.f, 160.f, 580.f, 560.f);
    window.draw(makeText("Secure Role Login", 550.f, 200.f, 34, WHITE_COLOR));
    window.draw(makeText("Choose your portal and continue", 540.f, 242.f, 16, sf::Color(160, 186, 220)));

    Button patBtn, docBtn, adminBtn, exitBtn;
    setupButton(patBtn, 460.f, 295.f, 360.f, 68.f, "Patient Portal", 23);
    setupButton(docBtn, 460.f, 385.f, 360.f, 68.f, "Doctor Portal", 23);
    setupButton(adminBtn, 460.f, 475.f, 360.f, 68.f, "Admin Control Panel", 23);
    setupButton(exitBtn, 460.f, 575.f, 360.f, 54.f, "Exit Application", 18);
    exitBtn.shape.setFillColor(sf::Color(130, 55, 65));

    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    patBtn.isHovered = patBtn.contains(mouse);
    docBtn.isHovered = docBtn.contains(mouse);
    adminBtn.isHovered = adminBtn.contains(mouse);
    exitBtn.isHovered = exitBtn.contains(mouse);
    patBtn.draw(window); docBtn.draw(window); adminBtn.draw(window); exitBtn.draw(window);

    if (messageClock.getElapsedTime().asSeconds() < 3.f) {
        window.draw(makeText(message, 430.f, 655.f, 15, messageColor));
    }
    window.display();
}

void GUI::drawPatientLogin() {
    window.clear(sf::Color(9, 24, 30));
    drawTitle("Patient Login");
    drawPanel(360.f, 190.f, 560.f, 500.f);
    window.draw(makeText("Patient Authentication", 530.f, 240.f, 30, WHITE_COLOR));
    window.draw(makeText("Patient ID", 460.f, 292.f, 16, sf::Color(170, 225, 218)));
    window.draw(makeText("Password", 460.f, 368.f, 16, sf::Color(170, 225, 218)));
    idBox.draw(window); passwordBox.draw(window);

    Button loginBtn, backBtn;
    setupButton(loginBtn, 460.f, 478.f, 360.f, 52.f, "Sign In", 19);
    setupButton(backBtn, 460.f, 542.f, 360.f, 44.f, "Back to Roles", 17);
    backBtn.shape.setFillColor(sf::Color(44, 110, 112));
    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    loginBtn.isHovered = loginBtn.contains(mouse);
    backBtn.isHovered = backBtn.contains(mouse);
    loginBtn.draw(window); backBtn.draw(window);

    window.draw(makeText("Attempts remaining: " + to_string(3 - loginAttempts), 460.f, 603.f, 14, loginAttempts > 0 ? ERROR_COLOR : GRAY_COLOR));
    if (messageClock.getElapsedTime().asSeconds() < 3.f) window.draw(makeText(message, 430.f, 632.f, 14, messageColor));
    window.display();
}

void GUI::drawDoctorLogin() {
    window.clear(sf::Color(9, 24, 30));
    drawTitle("Doctor Login");
    drawPanel(360.f, 190.f, 560.f, 500.f);
    window.draw(makeText("Doctor Authentication", 535.f, 240.f, 30, WHITE_COLOR));
    window.draw(makeText("Doctor ID", 460.f, 292.f, 16, sf::Color(170, 225, 218)));
    window.draw(makeText("Password", 460.f, 368.f, 16, sf::Color(170, 225, 218)));
    idBox.draw(window); passwordBox.draw(window);

    Button loginBtn, backBtn;
    setupButton(loginBtn, 460.f, 478.f, 360.f, 52.f, "Sign In", 19);
    setupButton(backBtn, 460.f, 542.f, 360.f, 44.f, "Back to Roles", 17);
    backBtn.shape.setFillColor(sf::Color(44, 110, 112));
    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    loginBtn.isHovered = loginBtn.contains(mouse);
    backBtn.isHovered = backBtn.contains(mouse);
    loginBtn.draw(window); backBtn.draw(window);

    window.draw(makeText("Attempts remaining: " + to_string(3 - loginAttempts), 460.f, 603.f, 14, loginAttempts > 0 ? ERROR_COLOR : GRAY_COLOR));
    if (messageClock.getElapsedTime().asSeconds() < 3.f) window.draw(makeText(message, 430.f, 632.f, 14, messageColor));
    window.display();
}

void GUI::drawAdminLogin() {
    window.clear(sf::Color(9, 24, 30));
    drawTitle("Admin Login");
    drawPanel(360.f, 190.f, 560.f, 500.f);
    window.draw(makeText("Administrative Authentication", 500.f, 240.f, 30, WHITE_COLOR));
    window.draw(makeText("Admin ID", 460.f, 292.f, 16, sf::Color(170, 225, 218)));
    window.draw(makeText("Password", 460.f, 368.f, 16, sf::Color(170, 225, 218)));
    idBox.draw(window); passwordBox.draw(window);

    Button loginBtn, backBtn;
    setupButton(loginBtn, 460.f, 478.f, 360.f, 52.f, "Sign In", 19);
    setupButton(backBtn, 460.f, 542.f, 360.f, 44.f, "Back to Roles", 17);
    backBtn.shape.setFillColor(sf::Color(44, 110, 112));
    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    loginBtn.isHovered = loginBtn.contains(mouse);
    backBtn.isHovered = backBtn.contains(mouse);
    loginBtn.draw(window); backBtn.draw(window);

    window.draw(makeText("Attempts remaining: " + to_string(3 - loginAttempts), 460.f, 603.f, 14, loginAttempts > 0 ? ERROR_COLOR : GRAY_COLOR));
    if (messageClock.getElapsedTime().asSeconds() < 3.f) window.draw(makeText(message, 430.f, 632.f, 14, messageColor));
    window.display();
}

void GUI::drawPatientMenu() {
    window.clear(sf::Color(8, 21, 28));
    drawTitle("Patient Workspace - " + currentPatient->getName() + "  |  Balance: PKR " + to_string((int)currentPatient->getBalance()));

    drawPanel(12.f, TOPBAR_H + 10.f, SIDEBAR_W - 20.f, APP_H - TOPBAR_H - 20.f);
    drawPanel(CONTENT_X, CONTENT_Y, CONTENT_W, CONTENT_H);
    drawPanel(CONTENT_X, ACTION_Y, CONTENT_W, ACTION_H);

    window.draw(makeText("PATIENT ACTIONS", 30.f, TOPBAR_H + 24.f, 14, sf::Color(152, 226, 217)));
    window.draw(makeText("OUTPUT VIEWER", CONTENT_X + 18.f, CONTENT_Y + 12.f, 16, sf::Color(152, 226, 217)));
    window.draw(makeText("INPUT CONSOLE", CONTENT_X + 18.f, ACTION_Y + 10.f, 16, sf::Color(152, 226, 217)));

    vector<string> options = {
        "Book New Appointment", "Cancel Pending Appointment", "View My Appointments", "View Medical Records",
        "View Billing Summary", "Pay Unpaid Bill", "Add Wallet Balance", "Logout to Role Menu"
    };

    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    for (int i = 0; i < (int)options.size(); i++) {
        float by = TOPBAR_H + 50.f + i * 78.f;
        sf::RectangleShape btn;
        btn.setPosition(22.f, by);
        btn.setSize({ SIDEBAR_W - 40.f, 62.f });
        btn.setFillColor(sf::FloatRect(22.f, by, SIDEBAR_W - 40.f, 62.f).contains(mouse) ? sf::Color(34, 145, 145) : sf::Color(24, 61, 70));
        btn.setOutlineThickness(1.f);
        btn.setOutlineColor(sf::Color(69, 146, 148));
        window.draw(btn);
        window.draw(makeText(options[i], 36.f, by + 19.f, 15, WHITE_COLOR));
    }

    float lineY = CONTENT_Y + 46.f - scrollOffset;
    istringstream stream(displayContent);
    string line;
    while (getline(stream, line)) {
        if (lineY > CONTENT_Y + 38.f && lineY < CONTENT_Y + CONTENT_H - 16.f) {
            window.draw(makeText(line, CONTENT_X + 20.f, lineY, 15, sf::Color(231, 241, 255)));
        }
        lineY += 23.f;
    }

    if (pendingAction != PendingAction::NONE) {
        string progress = "Step " + to_string(currentFieldIndex + 1) + " of " + to_string((int)pendingFields.size());
        string label = pendingFields.empty() ? getActionPrompt(pendingAction) : (pendingFields[currentFieldIndex].first + "  (" + pendingFields[currentFieldIndex].second + ")");
        window.draw(makeText(progress, CONTENT_X + 20.f, ACTION_Y + 32.f, 13, sf::Color(169, 232, 224)));
        window.draw(makeText(label, CONTENT_X + 20.f, ACTION_Y + 50.f, 13, sf::Color(150, 208, 201)));
        actionInputBox.draw(window);
        Button runBtn, closeBtn;
        setupButton(runBtn, CONTENT_X + CONTENT_W - 158.f, ACTION_Y + 80.f, 70.f, 44.f, "Run", 16);
        setupButton(closeBtn, CONTENT_X + CONTENT_W - 80.f, ACTION_Y + 80.f, 66.f, 44.f, "Close", 15);
        closeBtn.shape.setFillColor(sf::Color(125, 64, 72));
        runBtn.isHovered = runBtn.contains(mouse);
        closeBtn.isHovered = closeBtn.contains(mouse);
        runBtn.draw(window); closeBtn.draw(window);
    }
    else {
        window.draw(makeText("Select an action from the left panel to begin.", CONTENT_X + 20.f, ACTION_Y + 56.f, 16, sf::Color(156, 185, 217)));
    }

    if (messageClock.getElapsedTime().asSeconds() < 3.f) {
        window.draw(makeText(message, CONTENT_X + 18.f, APP_H - 28.f, 14, messageColor));
    }
    window.display();
}

void GUI::drawDoctorMenu() {
    window.clear(sf::Color(8, 21, 28));
    drawTitle("Doctor Workspace - Dr. " + currentDoctor->getName() + "  |  " + currentDoctor->getSpecialization());

    drawPanel(12.f, TOPBAR_H + 10.f, SIDEBAR_W - 20.f, APP_H - TOPBAR_H - 20.f);
    drawPanel(CONTENT_X, CONTENT_Y, CONTENT_W, CONTENT_H);
    drawPanel(CONTENT_X, ACTION_Y, CONTENT_W, ACTION_H);

    window.draw(makeText("CLINICAL ACTIONS", 30.f, TOPBAR_H + 24.f, 14, sf::Color(152, 226, 217)));
    window.draw(makeText("CLINICAL OUTPUT", CONTENT_X + 18.f, CONTENT_Y + 12.f, 16, sf::Color(152, 226, 217)));
    window.draw(makeText("INPUT CONSOLE", CONTENT_X + 18.f, ACTION_Y + 10.f, 16, sf::Color(152, 226, 217)));

    vector<string> options = {
        "View Today's Schedule", "Mark Appointment Complete", "Mark Appointment No-Show",
        "Write New Prescription", "View Patient History", "Logout to Role Menu"
    };

    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    for (int i = 0; i < (int)options.size(); i++) {
        float by = TOPBAR_H + 50.f + i * 92.f;
        sf::RectangleShape btn;
        btn.setPosition(22.f, by);
        btn.setSize({ SIDEBAR_W - 40.f, 74.f });
        btn.setFillColor(sf::FloatRect(22.f, by, SIDEBAR_W - 40.f, 74.f).contains(mouse) ? sf::Color(34, 145, 145) : sf::Color(24, 61, 70));
        btn.setOutlineThickness(1.f);
        btn.setOutlineColor(sf::Color(69, 146, 148));
        window.draw(btn);
        window.draw(makeText(options[i], 36.f, by + 25.f, 15, WHITE_COLOR));
    }

    float lineY = CONTENT_Y + 46.f - scrollOffset;
    istringstream stream(displayContent);
    string line;
    while (getline(stream, line)) {
        if (lineY > CONTENT_Y + 38.f && lineY < CONTENT_Y + CONTENT_H - 16.f) {
            window.draw(makeText(line, CONTENT_X + 20.f, lineY, 15, sf::Color(231, 241, 255)));
        }
        lineY += 23.f;
    }

    if (pendingAction != PendingAction::NONE) {
        string progress = "Step " + to_string(currentFieldIndex + 1) + " of " + to_string((int)pendingFields.size());
        string label = pendingFields.empty() ? getActionPrompt(pendingAction) : (pendingFields[currentFieldIndex].first + "  (" + pendingFields[currentFieldIndex].second + ")");
        window.draw(makeText(progress, CONTENT_X + 20.f, ACTION_Y + 32.f, 13, sf::Color(169, 232, 224)));
        window.draw(makeText(label, CONTENT_X + 20.f, ACTION_Y + 50.f, 13, sf::Color(150, 208, 201)));
        actionInputBox.draw(window);
        Button runBtn, closeBtn;
        setupButton(runBtn, CONTENT_X + CONTENT_W - 158.f, ACTION_Y + 80.f, 70.f, 44.f, "Run", 16);
        setupButton(closeBtn, CONTENT_X + CONTENT_W - 80.f, ACTION_Y + 80.f, 66.f, 44.f, "Close", 15);
        closeBtn.shape.setFillColor(sf::Color(125, 64, 72));
        runBtn.isHovered = runBtn.contains(mouse);
        closeBtn.isHovered = closeBtn.contains(mouse);
        runBtn.draw(window); closeBtn.draw(window);
    }
    else {
        window.draw(makeText("Select a clinical action from the left panel.", CONTENT_X + 20.f, ACTION_Y + 56.f, 16, sf::Color(156, 185, 217)));
    }

    if (messageClock.getElapsedTime().asSeconds() < 3.f) {
        window.draw(makeText(message, CONTENT_X + 18.f, APP_H - 28.f, 14, messageColor));
    }
    window.display();
}

void GUI::drawAdminMenu() {
    window.clear(sf::Color(8, 21, 28));
    drawTitle("Administration Workspace");

    drawPanel(12.f, TOPBAR_H + 10.f, SIDEBAR_W - 20.f, APP_H - TOPBAR_H - 20.f);
    drawPanel(CONTENT_X, CONTENT_Y, CONTENT_W, CONTENT_H);
    drawPanel(CONTENT_X, ACTION_Y, CONTENT_W, ACTION_H);

    window.draw(makeText("ADMIN ACTIONS", 30.f, TOPBAR_H + 18.f, 13, sf::Color(152, 226, 217)));
    window.draw(makeText("REPORTS & RESULTS", CONTENT_X + 18.f, CONTENT_Y + 12.f, 16, sf::Color(152, 226, 217)));
    window.draw(makeText("INPUT CONSOLE", CONTENT_X + 18.f, ACTION_Y + 10.f, 16, sf::Color(152, 226, 217)));

    vector<string> options = {
        "Add Doctor Profile", "Add Patient Profile", "Remove Doctor Profile", "View All Registered Patients", "View All Doctors",
        "View All Appointments", "View Unpaid Bills", "Discharge Patient & Archive", "View Security Login Log",
        "Generate Daily Summary Report", "Logout to Role Menu"
    };

    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    for (int i = 0; i < (int)options.size(); i++) {
        float by = TOPBAR_H + 42.f + i * 54.f;
        sf::RectangleShape btn;
        btn.setPosition(22.f, by);
        btn.setSize({ SIDEBAR_W - 40.f, 45.f });
        btn.setFillColor(sf::FloatRect(22.f, by, SIDEBAR_W - 40.f, 45.f).contains(mouse) ? sf::Color(34, 145, 145) : sf::Color(24, 61, 70));
        btn.setOutlineThickness(1.f);
        btn.setOutlineColor(sf::Color(69, 146, 148));
        window.draw(btn);
        window.draw(makeText(options[i], 36.f, by + 14.f, 13, WHITE_COLOR));
    }

    float lineY = CONTENT_Y + 46.f - scrollOffset;
    istringstream stream(displayContent);
    string line;
    while (getline(stream, line)) {
        if (lineY > CONTENT_Y + 38.f && lineY < CONTENT_Y + CONTENT_H - 16.f) {
            window.draw(makeText(line, CONTENT_X + 20.f, lineY, 15, sf::Color(231, 241, 255)));
        }
        lineY += 23.f;
    }

    if (pendingAction != PendingAction::NONE) {
        string progress = "Step " + to_string(currentFieldIndex + 1) + " of " + to_string((int)pendingFields.size());
        string label = pendingFields.empty() ? getActionPrompt(pendingAction) : (pendingFields[currentFieldIndex].first + "  (" + pendingFields[currentFieldIndex].second + ")");
        window.draw(makeText(progress, CONTENT_X + 20.f, ACTION_Y + 32.f, 13, sf::Color(169, 232, 224)));
        window.draw(makeText(label, CONTENT_X + 20.f, ACTION_Y + 50.f, 13, sf::Color(150, 208, 201)));
        actionInputBox.draw(window);
        Button runBtn, closeBtn;
        setupButton(runBtn, CONTENT_X + CONTENT_W - 158.f, ACTION_Y + 80.f, 70.f, 44.f, "Run", 16);
        setupButton(closeBtn, CONTENT_X + CONTENT_W - 80.f, ACTION_Y + 80.f, 66.f, 44.f, "Close", 15);
        closeBtn.shape.setFillColor(sf::Color(125, 64, 72));
        runBtn.isHovered = runBtn.contains(mouse);
        closeBtn.isHovered = closeBtn.contains(mouse);
        runBtn.draw(window); closeBtn.draw(window);
    }
    else {
        window.draw(makeText("Select an administrative action from the left panel.", CONTENT_X + 20.f, ACTION_Y + 56.f, 16, sf::Color(156, 185, 217)));
    }

    if (messageClock.getElapsedTime().asSeconds() < 3.f) {
        window.draw(makeText(message, CONTENT_X + 18.f, APP_H - 28.f, 14, messageColor));
    }
    window.display();
}

void GUI::handleLoginMenuEvents(sf::Event& event) {
    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    if (event.type == sf::Event::MouseButtonPressed) {
        if (sf::FloatRect(460.f, 295.f, 360.f, 68.f).contains(mouse)) {
            currentScreen = Screen::PATIENT_LOGIN; loginAttempts = 0; idBox.value = ""; passwordBox.value = ""; activeInput = nullptr;
        }
        else if (sf::FloatRect(460.f, 385.f, 360.f, 68.f).contains(mouse)) {
            currentScreen = Screen::DOCTOR_LOGIN; loginAttempts = 0; idBox.value = ""; passwordBox.value = ""; activeInput = nullptr;
        }
        else if (sf::FloatRect(460.f, 475.f, 360.f, 68.f).contains(mouse)) {
            currentScreen = Screen::ADMIN_LOGIN; loginAttempts = 0; idBox.value = ""; passwordBox.value = ""; activeInput = nullptr;
        }
        else if (sf::FloatRect(460.f, 575.f, 360.f, 54.f).contains(mouse)) {
            window.close();
        }
    }
}

void GUI::handlePatientLoginEvents(sf::Event& event) {
    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    if (event.type == sf::Event::MouseButtonPressed) {
        idBox.isActive = idBox.contains(mouse);
        passwordBox.isActive = passwordBox.contains(mouse);
        activeInput = idBox.isActive ? &idBox : (passwordBox.isActive ? &passwordBox : nullptr);
        if (sf::FloatRect(460.f, 478.f, 360.f, 52.f).contains(mouse)) tryPatientLogin();
        if (sf::FloatRect(460.f, 542.f, 360.f, 44.f).contains(mouse)) {
            currentScreen = Screen::LOGIN_MENU; idBox.value = ""; passwordBox.value = ""; loginAttempts = 0; activeInput = nullptr;
        }
    }
    if (event.type == sf::Event::TextEntered && activeInput) {
        if (event.text.unicode == 8) { if (!activeInput->value.empty()) activeInput->value.pop_back(); }
        else if (event.text.unicode == 13) tryPatientLogin();
        else if (event.text.unicode >= 32 && event.text.unicode < 128) activeInput->value += static_cast<char>(event.text.unicode);
    }
}

void GUI::handleDoctorLoginEvents(sf::Event& event) {
    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    if (event.type == sf::Event::MouseButtonPressed) {
        idBox.isActive = idBox.contains(mouse);
        passwordBox.isActive = passwordBox.contains(mouse);
        activeInput = idBox.isActive ? &idBox : (passwordBox.isActive ? &passwordBox : nullptr);
        if (sf::FloatRect(460.f, 478.f, 360.f, 52.f).contains(mouse)) tryDoctorLogin();
        if (sf::FloatRect(460.f, 542.f, 360.f, 44.f).contains(mouse)) {
            currentScreen = Screen::LOGIN_MENU; idBox.value = ""; passwordBox.value = ""; loginAttempts = 0; activeInput = nullptr;
        }
    }
    if (event.type == sf::Event::TextEntered && activeInput) {
        if (event.text.unicode == 8) { if (!activeInput->value.empty()) activeInput->value.pop_back(); }
        else if (event.text.unicode == 13) tryDoctorLogin();
        else if (event.text.unicode >= 32 && event.text.unicode < 128) activeInput->value += static_cast<char>(event.text.unicode);
    }
}

void GUI::handleAdminLoginEvents(sf::Event& event) {
    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    if (event.type == sf::Event::MouseButtonPressed) {
        idBox.isActive = idBox.contains(mouse);
        passwordBox.isActive = passwordBox.contains(mouse);
        activeInput = idBox.isActive ? &idBox : (passwordBox.isActive ? &passwordBox : nullptr);
        if (sf::FloatRect(460.f, 478.f, 360.f, 52.f).contains(mouse)) tryAdminLogin();
        if (sf::FloatRect(460.f, 542.f, 360.f, 44.f).contains(mouse)) {
            currentScreen = Screen::LOGIN_MENU; idBox.value = ""; passwordBox.value = ""; loginAttempts = 0; activeInput = nullptr;
        }
    }
    if (event.type == sf::Event::TextEntered && activeInput) {
        if (event.text.unicode == 8) { if (!activeInput->value.empty()) activeInput->value.pop_back(); }
        else if (event.text.unicode == 13) tryAdminLogin();
        else if (event.text.unicode >= 32 && event.text.unicode < 128) activeInput->value += static_cast<char>(event.text.unicode);
    }
}

void GUI::handlePatientMenuEvents(sf::Event& event) {
    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    if (event.type == sf::Event::MouseButtonPressed) {
        if (pendingAction != PendingAction::NONE) {
            actionInputBox.isActive = actionInputBox.contains(mouse);
            if (sf::FloatRect(CONTENT_X + CONTENT_W - 158.f, ACTION_Y + 80.f, 70.f, 44.f).contains(mouse)) { tryExecutePendingAction(); return; }
            if (sf::FloatRect(CONTENT_X + CONTENT_W - 80.f, ACTION_Y + 80.f, 66.f, 44.f).contains(mouse)) { closePendingAction(); return; }
        }
        for (int i = 0; i < 8; i++) {
            float by = TOPBAR_H + 50.f + i * 78.f;
            if (sf::FloatRect(22.f, by, SIDEBAR_W - 40.f, 62.f).contains(mouse)) { executePatientAction(i); return; }
        }
    }
    if (event.type == sf::Event::TextEntered && pendingAction != PendingAction::NONE && actionInputBox.isActive) {
        if (event.text.unicode == 8) { if (!actionInputBox.value.empty()) actionInputBox.value.pop_back(); }
        else if (event.text.unicode == 13) tryExecutePendingAction();
        else if (event.text.unicode >= 32 && event.text.unicode < 128) actionInputBox.value += static_cast<char>(event.text.unicode);
    }
    if (event.type == sf::Event::MouseWheelScrolled) {
        scrollOffset -= event.mouseWheelScroll.delta * 20.f;
        if (scrollOffset < 0.f) scrollOffset = 0.f;
    }
}

void GUI::handleDoctorMenuEvents(sf::Event& event) {
    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    if (event.type == sf::Event::MouseButtonPressed) {
        if (pendingAction != PendingAction::NONE) {
            actionInputBox.isActive = actionInputBox.contains(mouse);
            if (sf::FloatRect(CONTENT_X + CONTENT_W - 158.f, ACTION_Y + 80.f, 70.f, 44.f).contains(mouse)) { tryExecutePendingAction(); return; }
            if (sf::FloatRect(CONTENT_X + CONTENT_W - 80.f, ACTION_Y + 80.f, 66.f, 44.f).contains(mouse)) { closePendingAction(); return; }
        }
        for (int i = 0; i < 6; i++) {
            float by = TOPBAR_H + 50.f + i * 92.f;
            if (sf::FloatRect(22.f, by, SIDEBAR_W - 40.f, 74.f).contains(mouse)) { executeDoctorAction(i); return; }
        }
    }
    if (event.type == sf::Event::TextEntered && pendingAction != PendingAction::NONE && actionInputBox.isActive) {
        if (event.text.unicode == 8) { if (!actionInputBox.value.empty()) actionInputBox.value.pop_back(); }
        else if (event.text.unicode == 13) tryExecutePendingAction();
        else if (event.text.unicode >= 32 && event.text.unicode < 128) actionInputBox.value += static_cast<char>(event.text.unicode);
    }
    if (event.type == sf::Event::MouseWheelScrolled) {
        scrollOffset -= event.mouseWheelScroll.delta * 20.f;
        if (scrollOffset < 0.f) scrollOffset = 0.f;
    }
}

void GUI::handleAdminMenuEvents(sf::Event& event) {
    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    if (event.type == sf::Event::MouseButtonPressed) {
        if (pendingAction != PendingAction::NONE) {
            actionInputBox.isActive = actionInputBox.contains(mouse);
            if (sf::FloatRect(CONTENT_X + CONTENT_W - 158.f, ACTION_Y + 80.f, 70.f, 44.f).contains(mouse)) { tryExecutePendingAction(); return; }
            if (sf::FloatRect(CONTENT_X + CONTENT_W - 80.f, ACTION_Y + 80.f, 66.f, 44.f).contains(mouse)) { closePendingAction(); return; }
        }
        for (int i = 0; i < 11; i++) {
            float by = TOPBAR_H + 42.f + i * 54.f;
            if (sf::FloatRect(22.f, by, SIDEBAR_W - 40.f, 45.f).contains(mouse)) { executeAdminAction(i); return; }
        }
    }
    if (event.type == sf::Event::TextEntered && pendingAction != PendingAction::NONE && actionInputBox.isActive) {
        if (event.text.unicode == 8) { if (!actionInputBox.value.empty()) actionInputBox.value.pop_back(); }
        else if (event.text.unicode == 13) tryExecutePendingAction();
        else if (event.text.unicode >= 32 && event.text.unicode < 128) actionInputBox.value += static_cast<char>(event.text.unicode);
    }
    if (event.type == sf::Event::MouseWheelScrolled) {
        scrollOffset -= event.mouseWheelScroll.delta * 20.f;
        if (scrollOffset < 0.f) scrollOffset = 0.f;
    }
}

void GUI::tryPatientLogin() {
    if (idBox.value.empty() || passwordBox.value.empty()) { showMessage("Please enter both ID and password.", ERROR_COLOR); return; }
    int id = 0;
    try { id = stoi(idBox.value); }
    catch (...) { showMessage("Invalid ID. Please enter a number.", ERROR_COLOR); return; }

    patient* p = patients->findByID(id);
    if (p == nullptr) {
        loginAttempts++; fh->WriteToSecurityLog("Patient", id); showMessage("Patient ID not found.", ERROR_COLOR);
    }
    else if (p->getPassword() != passwordBox.value) {
        loginAttempts++; fh->WriteToSecurityLog("Patient", id); showMessage("Wrong password.", ERROR_COLOR);
    }
    else {
        currentPatient = p;
        loginAttempts = 0;
        idBox.value = ""; passwordBox.value = ""; displayContent = "Welcome " + currentPatient->getName() + ". Select an action from the left.";
        scrollOffset = 0.f; closePendingAction();
        currentScreen = Screen::PATIENT_MENU;
        return;
    }
    if (loginAttempts >= 3) {
        showMessage("Account locked. Contact Admin.", ERROR_COLOR);
        currentScreen = Screen::LOGIN_MENU;
        loginAttempts = 0; idBox.value = ""; passwordBox.value = ""; activeInput = nullptr;
    }
}

void GUI::tryDoctorLogin() {
    if (idBox.value.empty() || passwordBox.value.empty()) { showMessage("Please enter both ID and password.", ERROR_COLOR); return; }
    int id = 0;
    try { id = stoi(idBox.value); }
    catch (...) { showMessage("Invalid ID. Please enter a number.", ERROR_COLOR); return; }

    Doctor* d = doctors->findByID(id);
    if (d == nullptr) {
        loginAttempts++; fh->WriteToSecurityLog("Doctor", id); showMessage("Doctor ID not found.", ERROR_COLOR);
    }
    else if (d->getPassword() != passwordBox.value) {
        loginAttempts++; fh->WriteToSecurityLog("Doctor", id); showMessage("Wrong password.", ERROR_COLOR);
    }
    else {
        currentDoctor = d;
        loginAttempts = 0;
        idBox.value = ""; passwordBox.value = ""; displayContent = "Welcome Dr. " + currentDoctor->getName() + ".";
        scrollOffset = 0.f; closePendingAction();
        currentScreen = Screen::DOCTOR_MENU;
        return;
    }
    if (loginAttempts >= 3) {
        showMessage("Account locked. Contact Admin.", ERROR_COLOR);
        currentScreen = Screen::LOGIN_MENU;
        loginAttempts = 0; idBox.value = ""; passwordBox.value = ""; activeInput = nullptr;
    }
}

void GUI::tryAdminLogin() {
    if (idBox.value.empty() || passwordBox.value.empty()) { showMessage("Please enter both ID and password.", ERROR_COLOR); return; }
    int id = 0;
    try { id = stoi(idBox.value); }
    catch (...) { showMessage("Invalid ID. Please enter a number.", ERROR_COLOR); return; }

    Admin* a = admins->findByID(id);
    if (a == nullptr) {
        loginAttempts++; fh->WriteToSecurityLog("Admin", id); showMessage("Admin ID not found.", ERROR_COLOR);
    }
    else if (a->getPassword() != passwordBox.value) {
        loginAttempts++; fh->WriteToSecurityLog("Admin", id); showMessage("Wrong password.", ERROR_COLOR);
    }
    else {
        currentAdmin = a;
        loginAttempts = 0;
        idBox.value = ""; passwordBox.value = ""; displayContent = "Welcome to admin control center.";
        scrollOffset = 0.f; closePendingAction();
        currentScreen = Screen::ADMIN_MENU;
        return;
    }
    if (loginAttempts >= 3) {
        showMessage("Account locked. Contact Admin.", ERROR_COLOR);
        currentScreen = Screen::LOGIN_MENU;
        loginAttempts = 0; idBox.value = ""; passwordBox.value = ""; activeInput = nullptr;
    }
}

void GUI::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            switch (currentScreen) {
            case Screen::LOGIN_MENU: handleLoginMenuEvents(event); break;
            case Screen::PATIENT_LOGIN: handlePatientLoginEvents(event); break;
            case Screen::DOCTOR_LOGIN: handleDoctorLoginEvents(event); break;
            case Screen::ADMIN_LOGIN: handleAdminLoginEvents(event); break;
            case Screen::PATIENT_MENU: handlePatientMenuEvents(event); break;
            case Screen::DOCTOR_MENU: handleDoctorMenuEvents(event); break;
            case Screen::ADMIN_MENU: handleAdminMenuEvents(event); break;
            default: break;
            }
        }

        switch (currentScreen) {
        case Screen::LOGIN_MENU: drawLoginMenu(); break;
        case Screen::PATIENT_LOGIN: drawPatientLogin(); break;
        case Screen::DOCTOR_LOGIN: drawDoctorLogin(); break;
        case Screen::ADMIN_LOGIN: drawAdminLogin(); break;
        case Screen::PATIENT_MENU: drawPatientMenu(); break;
        case Screen::DOCTOR_MENU: drawDoctorMenu(); break;
        case Screen::ADMIN_MENU: drawAdminMenu(); break;
        default: break;
        }
    }
}
