#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <cctype>
#include <sqlite3.h>
#include <fstream>
#include <wx/radiobut.h>
#include <wx/listctrl.h>
#include <wx/grid.h>

#include <iostream>
using namespace std;

// Application class
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

// Main frame class
class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);
    ~MyFrame();
    void InitializeDatabase();
    void OnButton1Click(wxCommandEvent& event);
    void OnButton2Click(wxCommandEvent& event);
    void OnButton3Click(wxCommandEvent& event);
    void OnNameText(wxCommandEvent& event);
    void OnWeightText(wxCommandEvent& event);
    void OnPassText(wxCommandEvent& event);
    void OnSaveButtonClick(wxCommandEvent& event);
    void OnLogInButtonClickSetUp(wxCommandEvent& event);
    void DisplayUserData(const wxString& name);
    void OnLogOffButtonClick(wxCommandEvent& event);
    void OnAddTrainingDay(wxCommandEvent& event);
    void OnSaveButtonClickTrainingDay(wxCommandEvent& event);

    // UI components
    wxPanel *nutrition, *profile, *mainPage, *profileMain;
    wxBoxSizer *trainingSizer,*mainSizer, *buttonSizer, *nutritionSizer, *profileSizer, *radioSizer, *profileMainSizer;
    wxStaticText *textOnNameLogin, *textOnPassLogin, *textOrLogin, *textOnProfile, *textOnNutrition, *textOnName, *textOnGender, *textOnDate, *textOnWeight, *textOnPass;
    wxStaticText *profileName, *profileGender, *profileDateOfBirth, *profileWeight;
    wxDatePickerCtrl *dateOfBirth;
    wxRadioButton *radioMale, *radioFemale;
    wxTextCtrl *lineForPassLogin, *lineForNameLogin, *lineForName, *lineForWeight, *lineForPass;
    wxButton *addButtonWeek, *addButton, *buttonSave, *button1, *button2, *button3, *buttonLogIn, *buttonLogOff,*deleteRowButton;
    wxString filteredValueName, filteredValueWeight, filteredValuePass;
    wxFont boldFont;
    wxGrid* gridTrainingDay;
    wxScrolledWindow *training;
    bool profileSaved = false;
    bool logIn = false;
    bool hasUpperCase = false;

    int buttonClickCounter = 1;

    wxColour textColor = wxColour(50, 50, 50);
    wxColour textCtrlColor = wxColour(250, 245, 235);
    wxColour buttonColor = wxColour(70, 90, 120);
    wxColour backgroundColor = wxColour(235, 240, 245);

    sqlite3* db;
};

// Application initialization
bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("Fitness App");
    frame->Show(true);
    return true;
}

// Main frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxPoint(1020,400), wxSize(800, 600)) {

    InitializeDatabase();

    buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    // Цвет текста


    // Main menu buttons
    button1 = new wxButton(this, wxID_ANY, "Training");
    button2 = new wxButton(this, wxID_ANY, "Journal");
    button3 = new wxButton(this, wxID_ANY, "Profile");

    buttonSizer->Add(button1, 1, wxALL, 5);
    buttonSizer->Add(button2, 1, wxALL, 5);
    buttonSizer->Add(button3, 1, wxALL, 5);

    // Binding events to buttons
    button1->Bind(wxEVT_BUTTON, &MyFrame::OnButton1Click, this);
    button2->Bind(wxEVT_BUTTON, &MyFrame::OnButton2Click, this);
    button3->Bind(wxEVT_BUTTON, &MyFrame::OnButton3Click, this);

    // Creating panels for each menu item
    training = new wxScrolledWindow(this, wxID_ANY);
    training->SetScrollRate(5, 5);
    nutrition = new wxPanel(this, wxID_ANY);
    profile = new wxPanel(this, wxID_ANY);
    mainPage = new wxPanel(this, wxID_ANY);
    profileMain = new wxPanel(this, wxID_ANY);



    // Setting up content for each panel
    training->SetBackgroundColour(backgroundColor);
    nutrition->SetBackgroundColour(*wxRED);
    profile->SetBackgroundColour(backgroundColor);
    profileMain->SetBackgroundColour(backgroundColor);
    mainPage->SetBackgroundColour(backgroundColor);

    // Adding elements to the nutrition panel
    nutritionSizer = new wxBoxSizer(wxVERTICAL);
    textOnNutrition = new wxStaticText(nutrition, wxID_ANY, "Text on panel 2");
    nutritionSizer->Add(textOnNutrition, 1, wxALIGN_CENTER | wxALL, 10);
    nutrition->SetSizer(nutritionSizer);

    trainingSizer = new wxBoxSizer(wxVERTICAL);
    training->SetSizer(trainingSizer);

    // Adding elements to the profile panel
    profileSizer = new wxBoxSizer(wxVERTICAL);

    // Setting up text fields and radio buttons
    boldFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    textOnName = new wxStaticText(profile, wxID_ANY, "Enter your username:");
    textOnGender = new wxStaticText(profile, wxID_ANY, "Select your gender:");
    textOnDate = new wxStaticText(profile, wxID_ANY, "Select date of birth:");
    textOnWeight = new wxStaticText(profile, wxID_ANY, "Enter your weight (kg):");
    textOnPass = new wxStaticText(profile, wxID_ANY, "Enter your password:");
    wxStaticText* textOnRegister = new wxStaticText(profile, wxID_ANY, "Register:");
    textOnRegister->SetFont(boldFont);

    profileMainSizer = new wxBoxSizer(wxVERTICAL);
    profileName = new wxStaticText(profileMain, wxID_ANY, "Name: ");
    profileGender = new wxStaticText(profileMain, wxID_ANY, "Gender: ");
    profileDateOfBirth = new wxStaticText(profileMain, wxID_ANY, "Date of Birth: ");
    profileWeight = new wxStaticText(profileMain, wxID_ANY, "Weight: ");
    buttonLogOff = new wxButton(profileMain, wxID_ANY, "Log off");
    buttonLogOff->Bind(wxEVT_BUTTON, &MyFrame::OnLogOffButtonClick, this);
    buttonLogOff->SetOwnBackgroundColour(wxColour(60, 60, 60));

    profileName->SetForegroundColour(textColor);
    profileGender->SetForegroundColour(textColor);
    profileDateOfBirth->SetForegroundColour(textColor);
    profileWeight->SetForegroundColour(textColor);

    profileMainSizer->Add(profileName, 0, wxALIGN_LEFT | wxALL, 10);
    profileMainSizer->Add(profileGender, 0, wxALIGN_LEFT | wxALL, 10);
    profileMainSizer->Add(profileDateOfBirth, 0, wxALIGN_LEFT | wxALL, 10);
    profileMainSizer->Add(profileWeight, 0, wxALIGN_LEFT | wxALL, 10);
    profileMainSizer->Add(buttonLogOff, 0, wxALIGN_LEFT | wxALL, 10);
    profileMain->SetSizer(profileMainSizer);

    // Radio buttons with labels using wxStaticText for custom color
    radioMale = new wxRadioButton(profile, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    wxStaticText* labelMale = new wxStaticText(profile, wxID_ANY, "Male");
    labelMale->SetForegroundColour(textColor);

    radioFemale = new wxRadioButton(profile, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    wxStaticText* labelFemale = new wxStaticText(profile, wxID_ANY, "Female");
    labelFemale->SetForegroundColour(textColor);

    dateOfBirth = new wxDatePickerCtrl(profile, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT);
    wxDateTime minDate(1, wxDateTime::Jan, 1900);
    dateOfBirth->SetRange(minDate, wxDateTime::Now());

    lineForName = new wxTextCtrl(profile, wxID_ANY, "", wxDefaultPosition, wxSize(150, 30));
    lineForWeight = new wxTextCtrl(profile, wxID_ANY, "", wxDefaultPosition, wxSize(150, 30));
    lineForPass = new wxTextCtrl(profile, wxID_ANY, "", wxDefaultPosition, wxSize(150, 30));
    buttonSave = new wxButton(profile, wxID_ANY, "Save");


    // Setting colors for texts and controls
    textOnRegister->SetForegroundColour(textColor);
    textOnName->SetForegroundColour(textColor);
    textOnGender->SetForegroundColour(textColor);
    textOnDate->SetForegroundColour(textColor);
    textOnWeight->SetForegroundColour(textColor);
    textOnPass->SetForegroundColour(textColor);
    dateOfBirth->SetForegroundColour(textColor);
    lineForName->SetForegroundColour(textColor);
    lineForWeight->SetForegroundColour(textColor);
    lineForPass->SetForegroundColour(textColor);
    buttonSave->SetOwnBackgroundColour(buttonColor);

    lineForName->SetBackgroundColour(textCtrlColor);
    lineForWeight->SetBackgroundColour(textCtrlColor);
    dateOfBirth->SetBackgroundColour(textCtrlColor);
    lineForPass->SetBackgroundColour(textCtrlColor);

    // Adding elements to profileSizer
    profileSizer->Add(textOnRegister, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);
    profileSizer->Add(textOnName, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);
    profileSizer->Add(lineForName, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    profileSizer->Add(textOnGender, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);

    radioSizer = new wxBoxSizer(wxHORIZONTAL);
    radioSizer->Add(radioMale, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    radioSizer->Add(labelMale, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    radioSizer->Add(radioFemale, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    radioSizer->Add(labelFemale, 0, wxALIGN_CENTER_VERTICAL);
    profileSizer->Add(radioSizer, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    profileSizer->Add(textOnDate, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);
    profileSizer->Add(dateOfBirth, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);
    profileSizer->Add(textOnWeight, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);
    profileSizer->Add(lineForWeight, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);
    profileSizer->Add(textOnPass, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxTOP, 10);
    profileSizer->Add(lineForPass, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    profileSizer->Add(buttonSave, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxTOP, 20);

    wxStaticText* textOrLogin = new wxStaticText(profile, wxID_ANY, "Or Login:");
    textOrLogin->SetFont(boldFont);
    textOnNameLogin = new wxStaticText(profile, wxID_ANY, "Enter your name:");
    textOnPassLogin = new wxStaticText(profile, wxID_ANY, "Enter your password:");
    lineForNameLogin = new wxTextCtrl(profile, wxID_ANY, "", wxDefaultPosition, wxSize(150, 30));
    lineForPassLogin = new wxTextCtrl(profile, wxID_ANY, "", wxDefaultPosition, wxSize(150, 30));
    wxButton* buttonOrLogIn= new wxButton(profile, wxID_ANY, "Log in");

    buttonOrLogIn->SetBackgroundColour(buttonColor);
    buttonOrLogIn->Bind(wxEVT_BUTTON, &MyFrame::OnLogInButtonClickSetUp, this);

    textOrLogin->SetForegroundColour(textColor);
    textOnNameLogin->SetForegroundColour(textColor);
    textOnPassLogin->SetForegroundColour(textColor);
    lineForNameLogin->SetForegroundColour(textColor);
    lineForPassLogin->SetForegroundColour(textColor);

    lineForNameLogin->SetBackgroundColour(textCtrlColor);
    lineForPassLogin->SetBackgroundColour(textCtrlColor);

    profileSizer->Add(textOrLogin, 0, wxALIGN_CENTER | wxTOP, 20);
    profileSizer->Add(textOnNameLogin, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);
    profileSizer->Add(lineForNameLogin, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    profileSizer->Add(textOnPassLogin, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);
    profileSizer->Add(lineForPassLogin, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    profileSizer->Add(buttonOrLogIn, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Binding events to text fields and save button
    lineForName->Bind(wxEVT_TEXT, &MyFrame::OnNameText, this);
    lineForWeight->Bind(wxEVT_TEXT, &MyFrame::OnWeightText, this);
    lineForPass->Bind(wxEVT_TEXT, &MyFrame::OnPassText, this);
    buttonSave->Bind(wxEVT_BUTTON, &MyFrame::OnSaveButtonClick, this);
    profileSizer->AddStretchSpacer(1);
    profile->SetSizer(profileSizer);

    //create button and bind onAddTrainingDay
    addButtonWeek = new wxButton(training, wxID_ANY, "Add training day");
    addButtonWeek->SetOwnBackgroundColour(buttonColor);
    addButtonWeek->Bind(wxEVT_BUTTON, &MyFrame::OnAddTrainingDay, this);
    trainingSizer->Add(addButtonWeek, 0, wxALIGN_CENTER | wxALL, 10);

    // Hide all panels except the first one
    mainPage->Show();
    training->Hide();
    nutrition->Hide();
    profile->Hide();
    profileMain->Hide();

    // Add buttons and panels to the main sizer
    mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(buttonSizer, 0, wxEXPAND);
    mainSizer->Add(mainPage, 1, wxEXPAND);
    mainSizer->Add(training, 1, wxEXPAND);
    mainSizer->Add(nutrition, 1, wxEXPAND);
    mainSizer->Add(profile, 1, wxEXPAND);
    mainSizer->Add(profileMain, 1, wxEXPAND);

    SetSizer(mainSizer);
}
// Initialize the database
void MyFrame::InitializeDatabase() {
    int exit = sqlite3_open("users_data.db", &db);
    if (exit != SQLITE_OK) {
        wxMessageBox(wxString::Format("Failed to open database: %s", sqlite3_errmsg(db)), "Error", wxICON_ERROR);
        db = nullptr;
        return;
    }

    char* errorMessage = nullptr;

    const char* sqlUsers = R"(
        CREATE TABLE IF NOT EXISTS Users (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT NOT NULL,
            Gender TEXT NOT NULL,
            DateOfBirth TEXT NOT NULL,
            Weight TEXT NOT NULL,
            Password TEXT NOT NULL
        );
    )";
    exit = sqlite3_exec(db, sqlUsers, nullptr, nullptr, &errorMessage);
    if (exit != SQLITE_OK) {
        wxMessageBox(wxString::Format("Failed to create Users table: %s", errorMessage), "Error", wxICON_ERROR);
        sqlite3_free(errorMessage);
        return;
    }

    const char* sqlTrainingDays = R"(
        CREATE TABLE IF NOT EXISTS TrainingDays (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT NOT NULL,
            Day TEXT NOT NULL,
            Exercise TEXT NOT NULL,
            Weight TEXT NOT NULL,
            Reps TEXT NOT NULL
        );
    )";
    exit = sqlite3_exec(db, sqlTrainingDays, nullptr, nullptr, &errorMessage);
    if (exit != SQLITE_OK) {
        wxMessageBox(wxString::Format("Failed to create TrainingDays table: %s", errorMessage), "Error", wxICON_ERROR);
        sqlite3_free(errorMessage);
        return;
    }
}

// Event handler for Training button
void MyFrame::OnButton1Click(wxCommandEvent& event) {
    mainPage->Hide();
    training->Show();
    nutrition->Hide();
    profileMain->Hide();
    profile->Hide();
    mainSizer->Layout();
}

// Event handler for Journal button
void MyFrame::OnButton2Click(wxCommandEvent& event) {
    mainPage->Hide();
    training->Hide();
    nutrition->Show();
    profileMain->Hide();
    profile->Hide();
    mainSizer->Layout();
}

// Event handler for Profile button
void MyFrame::OnButton3Click(wxCommandEvent& event) {
    if(profileSaved == true) {
        profile->Hide();
    } else if(logIn == true && !profileSaved) {
        mainPage->Hide();
        training->Hide();
        nutrition->Hide();
        profile->Hide();
        profileMain->Show();
        mainSizer->Layout();
    }
    else{
        mainPage->Hide();
        training->Hide();
        nutrition->Hide();
        profile->Show();
        mainSizer->Layout();
    }
}

void MyFrame::DisplayUserData(const wxString& name) {
    wxString sql = "SELECT Name, Gender, DateOfBirth, Weight FROM Users WHERE Name = '" + name + "';";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        wxMessageBox("Failed to retrieve user data", "Error", wxICON_ERROR);
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        wxString userName = wxString::FromUTF8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        wxString userGender = wxString::FromUTF8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        wxString userDateOfBirth = wxString::FromUTF8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        wxString userWeight = wxString::FromUTF8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));

        profileName->SetLabel("Name: " + userName);
        profileGender->SetLabel("Gender: " + userGender);
        profileDateOfBirth->SetLabel("Date of Birth: " + userDateOfBirth);
        profileWeight->SetLabel("Weight: " + userWeight);
    } else {
        wxMessageBox("User data not found", "Error", wxICON_ERROR);
    }

    sqlite3_finalize(stmt);
}

// Event handler for name text field
void MyFrame::OnNameText(wxCommandEvent& event) {
    wxString value = lineForName->GetValue();
    filteredValueName.clear();
    for (wxChar ch : value) {
        if (wxIsalpha(ch) || wxIsdigit(ch)) {
            filteredValueName += ch;
        } else {
            lineForName->SetValue(filteredValueName);
            lineForName->SetInsertionPointEnd();
            break;
        }
    }
}

// Event handler for weight text field
void MyFrame::OnWeightText(wxCommandEvent& event) {
    wxString value = lineForWeight->GetValue();
    filteredValueWeight.clear();
    for (wxChar ch : value) {
        if (wxIsdigit(ch)) {
            filteredValueWeight += ch;
        } else if (ch == ' ') {
            lineForWeight->SetValue(filteredValueWeight);
            lineForWeight->SetInsertionPointEnd();
            return;
        } else {
            lineForWeight->SetValue(filteredValueWeight);
            lineForWeight->SetInsertionPointEnd();
            break;
        }
    }
}

// Event handler for save button
void MyFrame::OnSaveButtonClick(wxCommandEvent& event) {
    if (!db) {
        wxMessageBox("Database connection is not initialized!", "Error", wxICON_ERROR);
        return;
    }

    if (!lineForName || !lineForWeight || !lineForPass || !radioMale || !radioFemale) {
        wxMessageBox("Some UI elements are not initialized properly!", "Error", wxICON_ERROR);
        return;
    }

    wxString name = lineForName->GetValue();
    wxString gender = (radioMale->GetValue()) ? "Male" : (radioFemale->GetValue()) ? "Female" : "";
    wxString dateOfBirthStr = dateOfBirth ? dateOfBirth->GetValue().FormatISODate() : "";
    wxString weight = lineForWeight->GetValue();
    wxString pass = lineForPass->GetValue();

    if (name.IsEmpty()) {
        wxMessageBox("Name field is empty", "Error", wxICON_ERROR);
        return;
    }
    if (weight.IsEmpty()) {
        wxMessageBox("Weight field is empty", "Error", wxICON_ERROR);
        return;
    }
    if (pass.IsEmpty()) {
        wxMessageBox("Password field is empty", "Error", wxICON_ERROR);
        return;
    }
    if (gender.IsEmpty()) {
        wxMessageBox("Gender field is empty", "Error", wxICON_ERROR);
        return;
    }
    if (!hasUpperCase) {
        wxMessageBox("Password must contain at least one uppercase letter", "Error", wxICON_ERROR);
        return;
    }

    wxMessageBox("Database connection is open. Proceeding with SQL checks...", "Debug", wxICON_INFORMATION);

    const char* checkSql = "SELECT COUNT(*) FROM Users WHERE Name = ?;";
    sqlite3_stmt* stmt = nullptr;
    int exit = sqlite3_prepare_v2(db, checkSql, -1, &stmt, nullptr);

    if (exit != SQLITE_OK) {
        wxMessageBox(wxString::Format("Failed to prepare SQL statement (check user): %s", sqlite3_errmsg(db)), "Error", wxICON_ERROR);
        return;
    }

    sqlite3_bind_text(stmt, 1, name.mb_str(), -1, SQLITE_TRANSIENT);

    exit = sqlite3_step(stmt);
    if (exit == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);

        if (count > 0) {
            wxMessageBox("A user with this username already exists.", "Error", wxICON_ERROR);
            return;
        }
    } else {
        wxMessageBox(wxString::Format("Error checking for existing username: %s", sqlite3_errmsg(db)), "Error", wxICON_ERROR);
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);

    wxMessageBox("Proceeding to save user data...", "Debug", wxICON_INFORMATION);

    const char* sql = "INSERT INTO Users (Name, Gender, DateOfBirth, Weight, Password) VALUES (?, ?, ?, ?, ?);";
    exit = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (exit != SQLITE_OK) {
        wxMessageBox(wxString::Format("Failed to prepare SQL statement (save user): %s", sqlite3_errmsg(db)), "Error", wxICON_ERROR);
        return;
    }

    sqlite3_bind_text(stmt, 1, name.mb_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, gender.mb_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, dateOfBirthStr.mb_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, weight.mb_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, pass.mb_str(), -1, SQLITE_TRANSIENT);

    exit = sqlite3_step(stmt);

    if (exit != SQLITE_DONE) {
        wxMessageBox(wxString::Format("Failed to save profile data: %s", sqlite3_errmsg(db)), "Error", wxICON_ERROR);
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    wxMessageBox("Profile data saved successfully!", "Success", wxICON_INFORMATION);
    profileSaved = true;
}
void MyFrame::OnPassText(wxCommandEvent& eventB) {
    wxString value = lineForPass->GetValue();
    filteredValuePass.clear();

    for (wxChar ch : value) {
        if (filteredValuePass.length() < 20) {
            if (ch == ' ') {
                lineForPass->SetValue(filteredValuePass);
                lineForPass->SetInsertionPointEnd();
                return;
            }

            if (wxIsupper(ch)) {
                hasUpperCase = true;
            }

            if (wxIsdigit(ch) || wxIsalpha(ch)) {
                filteredValuePass += ch;
            }
        } else {
            wxMessageBox("Password must be less than 20 characters", "Error", wxICON_ERROR);
            lineForPass->SetValue(filteredValuePass);
            lineForPass->SetInsertionPointEnd();
            return;
        }
    }
}

void MyFrame::OnLogInButtonClickSetUp(wxCommandEvent& event) {
    wxString log = lineForNameLogin->GetValue();
    wxString passLog = lineForPassLogin->GetValue();

    string sql = "SELECT Name, Gender, DateOfBirth, Weight FROM Users WHERE Name = '" + string(log.mb_str()) + "' AND Password = '" + string(passLog.mb_str()) + "';";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (exit != SQLITE_OK) {
        wxMessageBox("Failed to check user data", "Error", wxICON_ERROR);
        sqlite3_finalize(stmt);
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        mainPage->Hide();
        training->Hide();
        nutrition->Hide();
        profile->Hide();
        profileMain->Show();
        mainSizer->Layout();
        logIn = true;
        profileSaved = false;

        DisplayUserData(log);
    } else {
        wxMessageBox("User data not found", "Error", wxICON_ERROR);
    }

    sqlite3_finalize(stmt);
}

void MyFrame::OnLogOffButtonClick(wxCommandEvent& event) {
    training->Hide();
    nutrition->Hide();
    profile->Show();
    mainPage->Hide();
    profileMain->Hide();
    logIn = false;
}
void MyFrame::OnAddTrainingDay(wxCommandEvent& event) {
    // Create a new grid for the training day
    wxGrid* gridTrainingDay = new wxGrid(training, wxID_ANY, wxDefaultPosition, wxSize(350, 200));

    // Initialize the grid with 1 row and 4 columns (3 visible + 1 hidden for ID)
    gridTrainingDay->CreateGrid(1, 4);

    // Set column headers
    gridTrainingDay->SetColLabelValue(0, "Exercise");
    gridTrainingDay->SetColLabelValue(1, "Weight");
    gridTrainingDay->SetColLabelValue(2, "Reps");
    gridTrainingDay->SetColLabelValue(3, "ID"); // Hidden column for database ID
    gridTrainingDay->HideCol(3);               // Hide the ID column

    // Create a label for the day number
    wxStaticText* dayNumber = new wxStaticText(training, wxID_ANY,
        "Day " + wxString::Format("%d", buttonClickCounter));
    dayNumber->SetForegroundColour(textColor); // Set text color

    // Add the label and the grid to the sizer for the training panel
    trainingSizer->Add(dayNumber, 0, wxALL, 10);
    trainingSizer->Add(gridTrainingDay, 0, wxALL, 10);

    // Create a horizontal sizer for the buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    // Create a button to add rows to the grid
    wxButton* addRowButton = new wxButton(training, wxID_ANY, "Add Row");
    addRowButton->SetOwnBackgroundColour(buttonColor);

    // Bind the button event to add rows specifically for this grid
    addRowButton->Bind(wxEVT_BUTTON, [gridTrainingDay](wxCommandEvent&) {
        gridTrainingDay->AppendRows(1);

        int newRow = gridTrainingDay->GetNumberRows() - 1;
        gridTrainingDay->SetCellValue(newRow, 0, "New Exercise");
        gridTrainingDay->SetCellValue(newRow, 1, "0"); // Default weight
        gridTrainingDay->SetCellValue(newRow, 2, "0"); // Default reps
        gridTrainingDay->SetCellValue(newRow, 3, "");  // Empty ID for new rows
    });

    // Create a button to delete rows
    wxButton* deleteRowButton = new wxButton(training, wxID_ANY, "Delete");
    deleteRowButton->SetOwnBackgroundColour(buttonColor);

    deleteRowButton->Bind(wxEVT_BUTTON, [this, gridTrainingDay](wxCommandEvent&) {
    int selectedRow = gridTrainingDay->GetGridCursorRow();
    if (selectedRow < 0 || selectedRow >= gridTrainingDay->GetNumberRows()) {
        wxMessageBox("No valid row selected to delete", "Error", wxICON_ERROR);
        return;
    }

    // Get the ID from the hidden column
    wxString id = gridTrainingDay->GetCellValue(selectedRow, 3); // Hidden column for ID
    if (!id.IsEmpty()) {
        // Delete the corresponding row in the database
        const char* sqlDelete = "DELETE FROM TrainingDays WHERE ID = ?;";
        sqlite3_stmt* stmt;
        int exit = sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, nullptr);

        if (exit != SQLITE_OK) {
            wxMessageBox(wxString::Format("Failed to prepare SQL statement for deletion: %s", sqlite3_errmsg(db)), "Error", wxICON_ERROR);
            return;
        }

        sqlite3_bind_text(stmt, 1, id.mb_str(), -1, SQLITE_TRANSIENT);
        exit = sqlite3_step(stmt);

        if (exit != SQLITE_DONE) {
            wxMessageBox(wxString::Format("Failed to delete row from database: %s", sqlite3_errmsg(db)), "Error", wxICON_ERROR);
            sqlite3_finalize(stmt);
            return;
        }

        sqlite3_finalize(stmt);
    }

    // Remove the row from the grid
    gridTrainingDay->DeleteRows(selectedRow);
    wxMessageBox("Row deleted successfully", "Success", wxICON_INFORMATION);
});

    // Create a button to save the grid data to the database
    wxButton* saveButton = new wxButton(training, wxID_ANY, "Save");
    saveButton->SetOwnBackgroundColour(buttonColor);
    saveButton->Bind(wxEVT_BUTTON, &MyFrame::OnSaveButtonClickTrainingDay, this);

    // Add buttons to the horizontal sizer
    buttonSizer->Add(addRowButton, 0, wxALL, 10);
    buttonSizer->Add(deleteRowButton, 0, wxALL, 10);
    buttonSizer->Add(saveButton, 0, wxALL, 10);

    // Add the horizontal sizer to the training sizer
    trainingSizer->Add(buttonSizer, 0, wxALL, 10);

    training->FitInside();
    training->Layout();

    // Increment the day counter
    buttonClickCounter++;
}

void MyFrame::OnSaveButtonClickTrainingDay(wxCommandEvent& event) {
    if (!gridTrainingDay) {
        wxMessageBox("Grid is not initialized", "Error", wxICON_ERROR);
        return;
    }

    int rows = gridTrainingDay->GetNumberRows();
    if (rows == 0) {
        wxMessageBox("No data in the grid", "Error", wxICON_ERROR);
        return;
    }

    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO TrainingDays (Name, Day, Exercise, Weight, Reps) VALUES (?, ?, ?, ?, ?);";

    wxString name = lineForNameLogin->GetValue();
    wxString day = wxString::Format("%d", buttonClickCounter - 1);

    for (int row = 0; row < rows; ++row) {
        wxString exercise = gridTrainingDay->GetCellValue(row, 0);
        wxString weight = gridTrainingDay->GetCellValue(row, 1);
        wxString reps = gridTrainingDay->GetCellValue(row, 2);
        wxString id = gridTrainingDay->GetCellValue(row, 3);

        if (exercise.IsEmpty() || weight.IsEmpty() || reps.IsEmpty()) {
            wxMessageBox(wxString::Format("Row %d has empty fields", row), "Error", wxICON_ERROR);
            continue;
        }

        if (id.IsEmpty()) {
            // Insert new row into the database
            int exit = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (exit != SQLITE_OK) {
                wxMessageBox(wxString::Format("SQLite Prepare Error: %s", sqlite3_errmsg(db)), "Error", wxICON_ERROR);
                return;
            }

            sqlite3_bind_text(stmt, 1, name.mb_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, day.mb_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, exercise.mb_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, weight.mb_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 5, reps.mb_str(), -1, SQLITE_TRANSIENT);

            exit = sqlite3_step(stmt);
            if (exit != SQLITE_DONE) {
                wxMessageBox(wxString::Format("SQLite Step Error: %s", sqlite3_errmsg(db)), "Error", wxICON_ERROR);
                sqlite3_finalize(stmt);
                return;
            }

            int lastInsertRowID = sqlite3_last_insert_rowid(db);
            gridTrainingDay->SetCellValue(row, 3, wxString::Format("%d", lastInsertRowID));
            sqlite3_finalize(stmt);
        }
    }

    wxMessageBox("Data saved successfully!", "Success", wxICON_INFORMATION);
}


// Destructor to close the database
MyFrame::~MyFrame() {
    sqlite3_close(db);
}

// Macro to launch the application
wxIMPLEMENT_APP(MyApp);