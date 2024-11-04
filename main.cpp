#include <wx/wx.h>
#include <wx/datectrl.h>   
#include <wx/dateevt.h>
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
    wxRadioBox* radioBox = new wxRadioBox(profile, wxID_ANY, "Your gender:", wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_ROWS);
    wxDatePickerCtrl* dateOfBirth = new wxDatePickerCtrl(profile, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);

    profileSizer->Add(radioBox, 1, wxALIGN_CENTER | wxALL, 10);
    profileSizer->Add(dateOfBirth, 1, wxALIGN_CENTER | wxALL, 10);
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

// Macro to launch the application
wxIMPLEMENT_APP(MyApp);
