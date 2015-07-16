#include "wxImagePanel.h"

wxBEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
	EVT_PAINT(wxImagePanel::paintEvent)
wxEND_EVENT_TABLE()

void wxImagePanel::paintEvent(wxPaintEvent & evt){
	wxPaintDC dc(this);
	dc.DrawBitmap(*(this->image), wxPoint(0, 0));
}
