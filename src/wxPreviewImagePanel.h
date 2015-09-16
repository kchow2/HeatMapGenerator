#pragma once
#include <wx/wx.h>

class wxPreviewImagePanel: public wxPanel
{
public:
	wxPreviewImagePanel(wxWindow *parent, wxWindowID winid, const wxPoint &pos, const wxSize &size, wxImage *img ) : wxPanel(parent, winid, pos, size), image(img) {};
	void setImage(wxImage *img);
	wxDECLARE_EVENT_TABLE();
private:
	void paintEvent(wxPaintEvent & evt);
	wxImage *image;
};

