#include "wxPreviewImagePanel.h"

wxBEGIN_EVENT_TABLE(wxPreviewImagePanel, wxPanel)
	EVT_PAINT(wxPreviewImagePanel::paintEvent)
wxEND_EVENT_TABLE()

void wxPreviewImagePanel::paintEvent(wxPaintEvent & evt){
	wxPaintDC dc(this);
	dc.DrawBitmap(*(this->image), wxPoint(0, 0));
}
