#include <wx/wx.h>

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

    wxPanel* panel1;
    wxPanel* panel2;
    wxPanel* panel3;
    wxBoxSizer* mainSizer;
};

// Реализация метода инициализации приложения
bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("Приложение на wxWidgets");
    frame->Show(true);
    return true;
}

// Реализация главного окна
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* button1 = new wxButton(this, wxID_ANY, "Button 1");
    wxButton* button2 = new wxButton(this, wxID_ANY, "Button 2");
    wxButton* button3 = new wxButton(this, wxID_ANY, "Button 3");
    wxTextCtrl* textCtrl = new wxTextCtrl(this, wxID_ANY, "Hello, World!", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

    buttonSizer->Add(button1, 1, wxALL, 5);
    buttonSizer->Add(button2, 1, wxALL, 5);
    buttonSizer->Add(button3, 1, wxALL, 5);



    // Привязка событий к кнопкам
    button1->Bind(wxEVT_BUTTON, &MyFrame::OnButton1Click, this);
    button2->Bind(wxEVT_BUTTON, &MyFrame::OnButton2Click, this);
    button3->Bind(wxEVT_BUTTON, &MyFrame::OnButton3Click, this);

    // Создаём панели для каждого экрана
    panel1 = new wxPanel(this, wxID_ANY);
    panel2 = new wxPanel(this, wxID_ANY);
    panel3 = new wxPanel(this, wxID_ANY);

    // Настраиваем контент для каждой панели (для примера просто меняем цвет фона)
    panel1->SetBackgroundColour(*wxBLUE);
    panel2->SetBackgroundColour(*wxGREEN);
    panel3->SetBackgroundColour(*wxRED);
    panel1->textCtrl;
    // Скрываем панель 2 и панель 3 по умолчанию
    panel2->Hide();
    panel3->Hide();

    // Добавляем кнопки и панели в основной sizer
    mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(buttonSizer, 0, wxEXPAND); // Панель с кнопками
    mainSizer->Add(panel1, 1, wxEXPAND);      // Начальная видимая панель
    mainSizer->Add(panel2, 1, wxEXPAND);      // Панель 2 скрыта по умолчанию
    mainSizer->Add(panel3, 1, wxEXPAND);      // Панель 3 скрыта по умолчанию

    SetSizer(mainSizer);
}

// Обработчики событий для кнопок
void MyFrame::OnButton1Click(wxCommandEvent& event) {
    panel1->Show(); // Показываем панель 1
    panel2->Hide(); // Скрываем остальные панели
    panel3->Hide();
    mainSizer->Layout(); // Обновляем компоновку
}

void MyFrame::OnButton2Click(wxCommandEvent& event) {
    panel1->Hide();
    panel2->Show(); // Показываем панель 2
    panel3->Hide();
    mainSizer->Layout(); 
}

void MyFrame::OnButton3Click(wxCommandEvent& event) {
    panel1->Hide();
    panel2->Hide();
    panel3->Show(); // Показываем панель 3
    mainSizer->Layout();
}
// Макрос для запуска приложения
wxIMPLEMENT_APP(MyApp);