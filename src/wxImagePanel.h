#pragma once
#include <wx/wx.h>

class wxImagePanel: public wxPanel
{
public:
	wxImagePanel(wxWindow *parent, wxWindowID winid, wxPoint &pos, wxSize &size, wxImage *img ) : wxPanel(parent, winid, pos, size), image(img) {};
	void setImage(wxImage *img);
	wxDECLARE_EVENT_TABLE();
private:
	void paintEvent(wxPaintEvent & evt);
	wxImage *image;
};

