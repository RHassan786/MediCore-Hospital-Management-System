#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <utility>
#include "menus.h"
using namespace std;

const sf::Color BACKGROUND_COLOR(15, 25, 50);
const sf::Color PRIMARY_COLOR(0, 150, 200);
const sf::Color SECONDARY_COLOR(0, 100, 140);
const sf::Color WHITE_COLOR(255, 255, 255);
const sf::Color GRAY_COLOR(180, 180, 180);
const sf::Color ERROR_COLOR(220, 60, 60);
const sf::Color SUCCESS_COLOR(60, 180, 100);
const sf::Color INPUT_BG(30, 45, 80);
const sf::Color BUTTON_HOVER(0, 180, 240);
const sf::Color DARK_PANEL(20, 35, 65);

enum class Screen {
    LOGIN_MENU,
    PATIENT_LOGIN,
    DOCTOR_LOGIN,
    ADMIN_LOGIN,
    PATIENT_MENU,
    DOCTOR_MENU,
    ADMIN_MENU,
    EXIT
};

enum class PendingAction {
    NONE,
    PATIENT_BOOK_APPOINTMENT,
    PATIENT_CANCEL_APPOINTMENT,
    PATIENT_PAY_BILL,
    PATIENT_TOP_UP,
    DOCTOR_MARK_COMPLETE,
    DOCTOR_MARK_NO_SHOW,
    DOCTOR_WRITE_PRESCRIPTION,
    DOCTOR_VIEW_HISTORY,
    ADMIN_ADD_PATIENT,
    ADMIN_ADD_DOCTOR,
    ADMIN_REMOVE_DOCTOR,
    ADMIN_DISCHARGE_PATIENT
};

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    bool isHovered = false;

    void draw(sf::RenderWindow& window) {
        if (isHovered)
            shape.setFillColor(BUTTON_HOVER);
        window.draw(shape);
        window.draw(text);
    }

    bool contains(sf::Vector2f point) {
        return shape.getGlobalBounds().contains(point);
    }
};

struct InputBox {
    sf::RectangleShape shape;
    sf::Text text;
    sf::Text placeholder;
    string value = "";
    bool isActive = false;
    bool isPassword = false;

    void draw(sf::RenderWindow& window) {
        if (isActive)
            shape.setOutlineColor(sf::Color(0, 150, 200));
        else
            shape.setOutlineColor(sf::Color(80, 100, 140));

        window.draw(shape);

        if (value.empty()) {
            window.draw(placeholder);
        }
        else {
            if (isPassword) {
                string masked(value.size(), '*');
                text.setString(masked);
            }
            else {
                text.setString(value);
            }
            window.draw(text);
        }
    }

    bool contains(sf::Vector2f point) {
        return shape.getGlobalBounds().contains(point);
    }
};

class GUI {
private:
    sf::RenderWindow window;
    sf::Font font;
    Screen currentScreen;

    Storage<patient>* patients;
    Storage<Doctor>* doctors;
    Storage<Admin>* admins;
    Storage<Appointment>* appointments;
    Storage<Bill>* bills;
    Storage<prescription>* prescriptions;
    FileHandler* fh;

    patient* currentPatient = nullptr;
    Doctor* currentDoctor = nullptr;
    Admin* currentAdmin = nullptr;

    string    message = "";
    sf::Color messageColor = sf::Color::White;
    sf::Clock messageClock;

    int loginAttempts = 0;
    InputBox  idBox;
    InputBox  passwordBox;
    InputBox* activeInput = nullptr;

    float  scrollOffset = 0.f;
    string displayContent = "";
    PendingAction pendingAction = PendingAction::NONE;
    InputBox actionInputBox;
    vector<pair<string, string>> pendingFields;
    vector<string> pendingValues;
    int currentFieldIndex = 0;

    void setupButton(Button& btn, float x, float y, float w, float h,
        const string& label, unsigned int fontSize = 18);
    void setupInput(InputBox& box, float x, float y, float w, float h,
        const string& ph, bool isPass = false);
    sf::Text makeText(const string& str, float x, float y,
        unsigned int size, sf::Color color);
    void showMessage(const string& msg, sf::Color color = sf::Color::White);
    void drawTitle(const string& title, float y = 60.f);
    void drawPanel(float x, float y, float w, float h);

    void drawLoginMenu();
    void drawPatientLogin();
    void drawDoctorLogin();
    void drawAdminLogin();
    void drawPatientMenu();
    void drawDoctorMenu();
    void drawAdminMenu();

    void handleLoginMenuEvents(sf::Event& event);
    void handlePatientLoginEvents(sf::Event& event);
    void handleDoctorLoginEvents(sf::Event& event);
    void handleAdminLoginEvents(sf::Event& event);
    void handlePatientMenuEvents(sf::Event& event);
    void handleDoctorMenuEvents(sf::Event& event);
    void handleAdminMenuEvents(sf::Event& event);

    void tryPatientLogin();
    void tryDoctorLogin();
    void tryAdminLogin();
    bool actionRequiresInput(PendingAction action);
    string getActionPrompt(PendingAction action);
    vector<pair<string, string>> getActionFields(PendingAction action);
    vector<string> splitInput(const string& input, char delimiter);
    string buildScriptFromPendingValues();
    bool isPositiveIntString(const string& s);
    bool isPositiveDoubleString(const string& s);
    bool validateFieldForAction(PendingAction action, int fieldIndex, const string& value, string& errorMsg);
    string normalizeOutputText(const string& raw);
    string wrapOutputText(const string& input, size_t maxCharsPerLine);
    string runConsoleAction(const string& scriptedInput, const function<void()>& actionFn);
    string buildActionPreview(PendingAction action);
    void refreshActionPreview();
    void executePatientAction(int index);
    void executeDoctorAction(int index);
    void executeAdminAction(int index);
    void beginAction(PendingAction action, const string& title);
    void tryExecutePendingAction();
    void closePendingAction();

public:
    GUI(Storage<patient>& p, Storage<Doctor>& d, Storage<Admin>& a,
        Storage<Appointment>& ap, Storage<Bill>& b,
        Storage<prescription>& pr, FileHandler& fh);
    void run();
};
