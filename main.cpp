#include <wx/wxprec.h>
#include <wx/sound.h>

#include <wx/mediactrl.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/taskbar.h>
#include <C:\Users\Raheem\Downloads\bass24\c\bass.h>
#include <iostream>
#include <future>
using namespace std;
int isPlaying = 0;
HCHANNEL channel = NULL;
HSTREAM stream = NULL;
wxStaticText* staticText;
wxStaticText* staticText2; 

wxStaticText* commentText;

wxSlider* VolumeSlider;
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnPlaylist(wxCommandEvent& event);
    void GetDuration(HSTREAM h);
    void OnAbout(wxCommandEvent& event);
    void Notify(wxTimerEvent& event);
    void OnClose(wxCloseEvent& event);
};

enum
{
    ID_Hello = 1,
    ID_Playlist=2
};

enum controlIds
{
    //some ids such as wxID_PLAY and wxID_STOP are standard ids but it is better to
    //make our own custom ids because of the nature of the buttons we are creating
    wxID_PLAYBUTTON = 1000,
    wxID_STOPBUTTON,
    wxID_PAUSE,
    wxID_SEEKER,
    wxID_MEDIACTRL,
    wxID_PMEDIA,
    wxID_TIMER,
    wxID_VOLSLIDER,
    wxID_PLAYLIST,
    wxID_NEXT,
    wxID_PREV,
    wxID_SHUFFLE,
    wxID_REPEAT,
    wxID_AUTOSTART,
    wxID_SHOWPLAYLIST,
    wxID_HIDE,
    wxID_FILEINFO,
    wxID_CROSSFADETIMER,
    ID_DUMMYVALUE
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
    wxTimer* myTimer = new wxTimer(this, 1);

}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Elm Player")
{
    wxMenu* playBackMenu = new wxMenu();
    
    playBackMenu->AppendCheckItem(wxID_SHUFFLE, "Shuffle\tCtrl+S", "Play in shuffle mode");
    playBackMenu->AppendCheckItem(wxID_REPEAT, "Repeat\tCtrl+R", "Repeat playing track");
    playBackMenu->Append(wxID_NEXT, "Next\tCtrl+N", "Skip to next track");
    playBackMenu->Append(wxID_PREV, "Previous\tCtrl+P");

    playBackMenu->Append(wxID_PLAYBUTTON, "Play\tSPACE");

    playBackMenu->Append(wxID_STOPBUTTON, "Stop\tCtrl+T");

    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Choose song...\tCtrl-H",
        "Find a song to play");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar; // creating the  main menu
    menuBar->Append(menuFile, "&File");

    wxMenu* menuPlaylist = new wxMenu; //creating the playlist main menu
 
    menuPlaylist->Append(ID_Playlist, "&View Playlist");
    menuBar->Append(menuPlaylist,"&Playlist");

    menuBar->Append(menuHelp, "&Help");
    menuFile->AppendSeparator();
    menuBar->Append(playBackMenu, "&Playback");

    SetMenuBar(menuBar);
    //SetIcon(wxIcon icon2(wxT("icon2")););
    SetIcon(wxIcon(wxT("tree.ico"), wxBITMAP_TYPE_ICO, 16, 16));
    CreateStatusBar();
    SetStatusText("Welcome to Elm Player!");

    SetBackgroundColour(wxColour(0x006249128));
    wxString test = wxT("Choose a file");
   
    wxStaticBox* staticBox = new wxStaticBox(this, wxID_STATIC,
        wxT(" & "), wxDefaultPosition, wxSize(1920,1080));
    VolumeSlider = new wxSlider(staticBox, 190, 16, 0, 100,
        wxPoint(0, 120), wxSize(200, -1),
        wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
    staticText = new wxStaticText(staticBox, wxID_ANY, wxT(""), wxPoint(0, 30));
    commentText = new wxStaticText(staticBox, wxID_ANY, wxT("Insert comment here"), wxPoint(0, 70));

    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnPlaylist, this, ID_Playlist);

    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_TIMER, &MyFrame::Notify, this);
    wxBoxSizer* bSizer = new wxBoxSizer(wxHORIZONTAL);

   
    bSizer->Add(staticBox, 1, wxEXPAND);

  

    SetSizer(bSizer);

    
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
    Destroy();
    wxWindow::Close();

}


void MyFrame::GetDuration(HSTREAM h) {
    QWORD len = BASS_ChannelGetLength(h, BASS_POS_BYTE); // the length in bytes
    int timeR = BASS_ChannelBytes2Seconds(h, len); // the length in seconds
    wxStaticText* staticText2 = new wxStaticText(this, wxID_ANY, wxT(""), wxPoint(0, 44));
    QWORD pos = NULL;
    int realPos = NULL;
    int secs = 0;
    int mins = 0;
  
    secs = static_cast<int>(realPos) % 60;
    mins = (realPos - secs) / 60;
}

void MyFrame::OnPlaylist(wxCommandEvent& event)
{
    wxMessageBox("This is the playlist page",
        "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World", wxOK | wxICON_INFORMATION);
   
}

void MyFrame::OnHello(wxCommandEvent& event)
{
   

    wxString defaultDir = wxT("C:\\Users\\%USERPROFILE%\\");
    wxString defaultFilename = wxEmptyString;
    wxString caption = wxT("Choose a file");
    wxString wildcard = "MP3 Files (*.mp3) | *.mp3";
    
    wxString path="";
   
    wxFileDialog dialog(NULL, caption, defaultDir, defaultFilename,
        wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
   
    if (dialog.ShowModal() == wxID_OK)
    {
        if (isPlaying == 1) {
            BASS_ChannelFree(stream);
            BASS_ChannelSetPosition(stream, 0, BASS_POS_BYTE);
            QWORD tpos = BASS_ChannelGetPosition(stream, BASS_POS_BYTE);
            int trealPos = BASS_ChannelBytes2Seconds(stream, tpos);
            wxString theSongLength = wxString::Format(wxT("%i"), trealPos);
           
            staticText2->SetLabel("Inside:" + trealPos);
           

        }
        
            isPlaying = 1;

            BASS_ChannelFree(stream);
            path = "";
            wxTimer* myTimer = new wxTimer(this, 1);
            myTimer->SetOwner(this, 1);
            isPlaying = 1;

            path = "" + dialog.GetPath();

            staticText->SetLabel("Now playing :"+path);
            BASS_Init(-1, 44100, 0, 0, NULL);
            BASS_SetVolume(.02);
            HSAMPLE sample = BASS_SampleLoad(false, path.char_str(), 0, 0, 1, BASS_SAMPLE_MONO);

            stream = BASS_StreamCreateFile(FALSE, path.char_str(), 0, 0, 0);
            myTimer->Start();
            channel = BASS_SampleGetChannel(stream, FALSE);
            BASS_ChannelPlay(stream, TRUE);
            staticText2 = new wxStaticText(this, wxID_ANY, wxT(""), wxPoint(0, 45));

            const char* comments = BASS_ChannelGetTags(stream, BASS_TAG_OGG); // get a pointer to the 1st comment
            if (comments)
                while (*comments) {
                    commentText->SetLabel(comments);
                    comments += strlen(comments) + 1; // move on to next comment
                }
            int filterIndex = dialog.GetFilterIndex();
        
    }
    
}
void MyFrame::Notify(wxTimerEvent& event) {
    int realPos;
   
    QWORD pos = BASS_ChannelGetPosition(stream, BASS_POS_BYTE);
    realPos = BASS_ChannelBytes2Seconds(stream, pos);
    wxString theSongLength = wxString::Format(wxT("%i"), realPos);
    staticText2->SetLabel(theSongLength);

    double volume = static_cast<double>(VolumeSlider->GetValue()) ;
    //BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, volume);
    BASS_SetVolume(volume/100);
}

void MyFrame::OnClose(wxCloseEvent& event)
{
    wxMessageBox("test close");
    Close(true);
    wxWindow::Close();
    wxWindow::Destroy();
    Destroy();

}
