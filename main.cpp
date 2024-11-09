#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <cctype>
#include <sqlite3.h>
#include <fstream>
#include <wx/radiobut.h>

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

    // UI components
    wxPanel *training, *nutrition, *profile, *mainPage, *profileSetUp,*profileMain ;
    wxBoxSizer *mainSizer, *buttonSizer, *nutritionSizer, *profileSizer, *radioSizer;
    wxStaticText *textOnProfile, *textOnNutrition, *textOnName, *textOnGender, *textOnDate, *textOnWeight, *textOnPass;
    wxDatePickerCtrl* dateOfBirth;
    wxRadioButton *radioMale, *radioFemale;
    wxTextCtrl *lineForName, *lineForWeight, *lineForPass;
    wxButton *buttonSave, *button1, *button2, *button3;
    wxString filteredValueName, filteredValueWeight, filteredValuePass;

    bool profileSaved = false;
    bool logIn = false;

    sqlite3* db;
};

// Application initialization
bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("Application on wxWidgets");
    frame->Show(true);
    return true;
}

// Main frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {

    InitializeDatabase();
    buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxColour textColor(60, 60, 60);
    wxColour textCtrlColor(255, 229, 180);

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
    training = new wxPanel(this, wxID_ANY);
    nutrition = new wxPanel(this, wxID_ANY);
    profile = new wxPanel(this, wxID_ANY);
    mainPage = new wxPanel(this, wxID_ANY);
    profileSetUp = new wxPanel(this, wxID_ANY);
    profileMain = new wxPanel(this, wxID_ANY);

    // Setting up content for each panel
    training->SetBackgroundColour(*wxBLUE);
    nutrition->SetBackgroundColour(*wxGREEN);
    profile->SetBackgroundColour(wxColour(230, 230, 250));
    profileSetUp->SetBackgroundColour(*wxRED);
    profileMain->SetBackgroundColour(*wxYELLOW);
    mainPage->SetBackgroundColour(*wxWHITE);

    // Adding elements to the nutrition panel
    nutritionSizer = new wxBoxSizer(wxVERTICAL);
    textOnNutrition = new wxStaticText(nutrition, wxID_ANY, "Text on panel 2");
    nutritionSizer->Add(textOnNutrition, 1, wxALIGN_CENTER | wxALL, 10);
    nutrition->SetSizer(nutritionSizer);

    // Adding elements to the profile panel
    profileSizer = new wxBoxSizer(wxVERTICAL);

    // Setting up text fields and radio buttons
    textOnName = new wxStaticText(profile, wxID_ANY, "Enter your name:");
    textOnGender = new wxStaticText(profile, wxID_ANY, "Select your gender:");
    textOnDate = new wxStaticText(profile, wxID_ANY, "Select date of birth:");
    textOnWeight = new wxStaticText(profile, wxID_ANY, "Enter your weight (kg):");
    textOnPass = new wxStaticText(profile, wxID_ANY, "Enter your password:");

    wxStaticText* textOnNameSetUp = new wxStaticText(profileSetUp, wxID_ANY, "Enter your name:");
    wxStaticText* textOnPassSetUp = new wxStaticText(profileSetUp, wxID_ANY, "Enter your password:");
    wxTextCtrl* lineForNameSetUp = new wxTextCtrl(profileSetUp, wxID_ANY, "", wxDefaultPosition, wxSize(150, 30));
    wxTextCtrl* lineForPassSetUp = new wxTextCtrl(profileSetUp, wxID_ANY, "", wxDefaultPosition, wxSize(150, 30));
    wxButton* buttonLogIn = new wxButton(profileSetUp, wxID_ANY, "Log in");
    buttonLogIn->Bind(wxEVT_BUTTON, &MyFrame::OnLogInButtonClickSetUp, this);

    wxBoxSizer* profileSetUpSizer = new wxBoxSizer(wxVERTICAL);
    profileSetUpSizer->Add(textOnNameSetUp, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);
    profileSetUpSizer->Add(lineForNameSetUp, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    profileSetUpSizer->Add(textOnPassSetUp, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);
    profileSetUpSizer->Add(lineForPassSetUp, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    profileSetUpSizer->Add(buttonLogIn, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxTOP, 20);
    profileSetUpSizer->AddStretchSpacer(1);
    profileSetUp->SetSizer(profileSetUpSizer);

    wxBoxSizer* profileMainSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* textOnProfileMain = new wxStaticText(profileMain, wxID_ANY, "Profile data");
    profileMainSizer->Add(textOnProfileMain, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);
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
    textOnName->SetForegroundColour(textColor);
    textOnGender->SetForegroundColour(textColor);
    textOnDate->SetForegroundColour(textColor);
    textOnWeight->SetForegroundColour(textColor);
    textOnPass->SetForegroundColour(textColor);
    dateOfBirth->SetForegroundColour(textColor);
    lineForName->SetForegroundColour(textColor);
    lineForWeight->SetForegroundColour(textColor);
    lineForPass->SetForegroundColour(textColor);
    buttonSave->SetOwnBackgroundColour(wxColour(60, 60, 60));

    lineForName->SetBackgroundColour(textCtrlColor);
    lineForWeight->SetBackgroundColour(textCtrlColor);
    dateOfBirth->SetBackgroundColour(textCtrlColor);
    lineForPass->SetBackgroundColour(textCtrlColor);

    // Adding elements to profileSizer
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

    // Binding events to text fields and save button
    lineForName->Bind(wxEVT_TEXT, &MyFrame::OnNameText, this);
    lineForWeight->Bind(wxEVT_TEXT, &MyFrame::OnWeightText, this);
    lineForPass->Bind(wxEVT_TEXT, &MyFrame::OnPassText, this);
    buttonSave->Bind(wxEVT_BUTTON, &MyFrame::OnSaveButtonClick, this);
    profileSizer->AddStretchSpacer(1);
    profile->SetSizer(profileSizer);

    // Hide all panels except the first one
    mainPage->Show();
    training->Hide();
    nutrition->Hide();
    profile->Hide();
    profileMain->Hide();
    profileSetUp->Hide();

    // Add buttons and panels to the main sizer
    mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(buttonSizer, 0, wxEXPAND);
    mainSizer->Add(mainPage, 1, wxEXPAND);
    mainSizer->Add(training, 1, wxEXPAND);
    mainSizer->Add(nutrition, 1, wxEXPAND);
    mainSizer->Add(profile, 1, wxEXPAND);
    mainSizer->Add(profileMain, 1, wxEXPAND);
    mainSizer->Add(profileSetUp, 1, wxEXPAND);

    SetSizer(mainSizer);
}

// Initialize the database
void MyFrame::InitializeDatabase() {
    int exit = sqlite3_open("users_data.db", &db);
    if (exit) {
        wxMessageBox("Error opening database", "Error", wxICON_ERROR);
        return;
    }

    char* errorMessage;

    const char* sql = "CREATE TABLE IF NOT EXISTS Users ("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "Name TEXT NOT NULL, "
                      "Gender TEXT NOT NULL, "
                      "DateOfBirth TEXT NOT NULL, "
                      "Weight TEXT NOT NULL);";
    exit = sqlite3_exec(db, sql, NULL, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        wxMessageBox(wxString::Format("Failed to create table: %s", errorMessage), "Error", wxICON_ERROR);
        sqlite3_free(errorMessage);
        return;
    }

    bool hasPasswordColumn = false;
    const char* checkColumnSql = "PRAGMA table_info(Users);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, checkColumnSql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* columnName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            if (strcmp(columnName, "Password") == 0) {
                hasPasswordColumn = true;
                break;
            }
        }
        sqlite3_finalize(stmt);
    }
}

// Event handler for Training button
void MyFrame::OnButton1Click(wxCommandEvent& event) {
    mainPage->Hide();
    training->Show();
    nutrition->Hide();
    profile->Hide();
    profileSetUp->Hide();
    mainSizer->Layout();
}

// Event handler for Journal button
void MyFrame::OnButton2Click(wxCommandEvent& event) {
    mainPage->Hide();
    training->Hide();
    nutrition->Show();
    profile->Hide();
    profileSetUp->Hide();
    mainSizer->Layout();
}

// Event handler for Profile button
void MyFrame::OnButton3Click(wxCommandEvent& event) {
    if(profileSaved == true) {
        profileSetUp->Show();
        profile->Hide();
    } else if(logIn == true and profileSaved == false) {
        mainPage->Hide();
        training->Hide();
        nutrition->Hide();
        profile->Hide();
        profileMain->Show();
        profileSetUp->Hide();
        mainSizer->Layout();
    }
    else{
        mainPage->Hide();
        training->Hide();
        nutrition->Hide();
        profile->Show();
        profileSetUp->Hide();
        mainSizer->Layout();
    }

}

// Event handler for name text field
void MyFrame::OnNameText(wxCommandEvent& event) {
    wxString value = lineForName->GetValue();
    filteredValueName.clear();
    for (wxChar ch : value) {
        if (wxIsalpha(ch) || ch == ' ') {
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
    wxString name = lineForName->GetValue();
    wxString gender = radioMale->GetValue() ? "Male" : "Female";
    wxString dateOfBirthStr = dateOfBirth->GetValue().FormatISODate();
    wxString weight = lineForWeight->GetValue();
    wxString pass = lineForPass->GetValue();

    string sql = "INSERT INTO Users (Name, Gender, DateOfBirth, Weight, Password) VALUES ('" +
                 string(name.mb_str()) + "', '" +
                 string(gender.mb_str()) + "', '" +
                 string(dateOfBirthStr.mb_str()) + "', '" +
                 string(weight.mb_str()) + "', '" +
                 string(pass.mb_str()) + "');";

    char* errorMessage;

    if(name.IsEmpty()) {
        wxMessageBox("Name field is empty", "Error", wxICON_ERROR);
        return;
    } else if(weight.IsEmpty()) {
        wxMessageBox("Weight field is empty", "Error", wxICON_ERROR);
        return;
    } else if(pass.IsEmpty()) {
        wxMessageBox("Password field is empty", "Error", wxICON_ERROR);
        return;
    } else if(!radioMale->GetValue() && !radioFemale->GetValue()) {
        wxMessageBox("Gender field is empty", "Error", wxICON_ERROR);
        return;
    }

    int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        wxMessageBox("Failed to save profile data", "Error", wxICON_ERROR);
        sqlite3_free(errorMessage);
    } else {
        wxMessageBox("Profile data saved successfully!", "Success", wxICON_INFORMATION);
        profile->Hide();
        profileSetUp->Show();
        mainSizer->Layout();
        profileSaved = true;

    }
}

void MyFrame::OnPassText(wxCommandEvent& eventB) {
    wxString value = lineForPass->GetValue();
    bool hasUpperCase = false;
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
    wxString name = lineForName->GetValue();
    wxString pass = lineForPass->GetValue();

    string sql = "SELECT * FROM Users WHERE Name = '" + string(name.mb_str()) + "' AND Password = '" + string(pass.mb_str()) + "';";

    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        wxMessageBox("Failed to check user data", "Error", wxICON_ERROR);
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        wxMessageBox("User data found", "Success", wxICON_INFORMATION);
        mainPage->Hide();
        training->Hide();
        nutrition->Hide();
        profile->Hide();
        profileMain->Show();
        profileSetUp->Hide();
        mainSizer->Layout();
        logIn = true;
        profileSaved = false;

    } else {
        wxMessageBox("User data not found", "Error", wxICON_ERROR);
        }
}

// Destructor to close the database
MyFrame::~MyFrame() {
    sqlite3_close(db);
}

// Macro to launch the application
wxIMPLEMENT_APP(MyApp);