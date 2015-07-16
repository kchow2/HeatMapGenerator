#include <wx/wx.h>
#include "ImageOptionsDialog.h"

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	ImageOptionsDialog *optionsDialog = new ImageOptionsDialog("HeatMap Generator", wxPoint(50, 50));
	optionsDialog->Show();
    return true;
}