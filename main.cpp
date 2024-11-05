#include <wx/wx.h>
#include <wx/datectrl.h>   
#include <wx/dateevt.h>
#include <cctype>

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);

private:
    void OnButton1Click(wxCommandEvent& event);
    void OnButton2Click(wxCommandEvent& event);
    void OnButton3Click(wxCommandEvent& event);
    void OnNameText(wxCommandEvent& event);

    wxPanel* training;
    wxPanel* nutrition;
    wxPanel* profile;
    wxPanel* mainPage;
    wxBoxSizer* mainSizer;
    wxBoxSizer* buttonSizer;
    wxBoxSizer* nutritionSizer;
    wxBoxSizer* profileSizer;
    wxStaticText* textOnProfile;
    wxStaticText* textOnNutrition;
    wxDatePickerCtrl* dateOfBirth;
    wxRadioBox* radioBox;
    wxTextCtrl* lineForName;
    wxTextCtrl* lineForWeight;
    wxStaticText* textOnName;
    wxStaticText* textOnGender;
    wxStaticText* textOnDate;
    wxStaticText* textOnWeight;
    wxButton* buttonSave;

    wxButton* button1;
    wxButton* button2;
    wxButton* button3;
};

bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("Application on wxWidgets");
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    
    buttonSizer = new wxBoxSizer(wxHORIZONTAL);
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

    // Setting up content for each panel
    training->SetBackgroundColour(*wxBLUE);
    nutrition->SetBackgroundColour(*wxGREEN);
    profile->SetBackgroundColour(*wxRED);
    mainPage->SetBackgroundColour(*wxWHITE);

    // Adding elements to the nutrition panel
    nutritionSizer = new wxBoxSizer(wxVERTICAL);
    textOnNutrition = new wxStaticText(nutrition, wxID_ANY, "Text on panel 2");
    nutritionSizer->Add(textOnNutrition, 1, wxALIGN_CENTER | wxALL, 10);
    nutrition->SetSizer(nutritionSizer);

    // Adding elements to the profile panel
    profileSizer = new wxBoxSizer(wxVERTICAL);

    wxArrayString choices;
    choices.Add("Male");
    choices.Add("Female");    
    
    radioBox = new wxRadioBox(profile, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_ROWS);
    dateOfBirth = new wxDatePickerCtrl(profile, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
    dateOfBirth->SetBackgroundColour(wxColour(100, 100, 100)); // Установить белый цвет фона

    
    lineForName = new wxTextCtrl(profile, wxID_ANY, "", wxDefaultPosition, wxSize(150, 30));
    lineForWeight = new wxTextCtrl(profile, wxID_ANY, "", wxDefaultPosition, wxSize(150, 30));
    textOnName = new wxStaticText(profile, wxID_ANY, "Enter your name:");
    textOnGender = new wxStaticText(profile, wxID_ANY, "Select your gender:");
    textOnDate = new wxStaticText(profile, wxID_ANY, "Select date of birth:");
    textOnWeight = new wxStaticText(profile, wxID_ANY, "Enter your weight:");
    buttonSave = new wxButton(profile, wxID_ANY, "Save");

    profileSizer->Add(textOnName, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);
    profileSizer->Add(lineForName, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    profileSizer->Add(textOnGender, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);
    profileSizer->Add(radioBox, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    profileSizer->Add(textOnDate, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);
    profileSizer->Add(dateOfBirth, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10); 
    profileSizer->Add(textOnWeight, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);
    profileSizer->Add(lineForWeight, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);
    profileSizer->Add(buttonSave, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxTOP, 20);

    lineForName->Bind(wxEVT_TEXT, &MyFrame::OnNameText, this);
    
    profileSizer->AddStretchSpacer(1);
    profile->SetSizer(profileSizer);

    // Hide all panels except the first one
    mainPage->Show();
    training->Hide();
    nutrition->Hide();
    profile->Hide();

    // Add buttons and panels to the main sizer
    mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(buttonSizer, 0, wxEXPAND); 
    mainSizer->Add(mainPage, 1, wxEXPAND);
    mainSizer->Add(training, 1, wxEXPAND);      
    mainSizer->Add(nutrition, 1, wxEXPAND);     
    mainSizer->Add(profile, 1, wxEXPAND);      

    SetSizer(mainSizer);
}

// Event handlers for buttons
void MyFrame::OnButton1Click(wxCommandEvent& event) {
    mainPage->Hide();
    training->Show();
    nutrition->Hide();
    profile->Hide();
    mainSizer->Layout();
}

void MyFrame::OnButton2Click(wxCommandEvent& event) {
    mainPage->Hide();
    training->Hide();
    nutrition->Show();
    profile->Hide();
    mainSizer->Layout();
}

void MyFrame::OnButton3Click(wxCommandEvent& event) {
    mainPage->Hide();
    training->Hide();
    nutrition->Hide();
    profile->Show();
    mainSizer->Layout();
}

void MyFrame::OnNameText(wxCommandEvent& event) {
    wxString value = lineForName->GetValue();
    wxString filteredValue;
    for (wxChar ch : value) {
        if (wxIsalpha(ch)) {
            filteredValue += ch;
        } else {
            wxMessageBox("Only letters are allowed in the name field", "Error", wxICON_ERROR);
            lineForName->SetValue(filteredValue);
            break;
        }
    }
}

// Macro to launch the application
wxIMPLEMENT_APP(MyApp);
