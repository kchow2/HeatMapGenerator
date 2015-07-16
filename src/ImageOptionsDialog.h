#pragma once
#include <wx/wx.h>
#include <vector>
#include "ImageGenerator.h"
#include "InvertedHeatMapColourProvider.h"
#include "wxImagePanel.h"

struct ImageOptions;
class ImageOptionsDialogWorker;

class ImageOptionsDialog : public wxFrame
{
public:
	ImageOptionsDialog(const wxString& title, const wxPoint& pos);
	//~ImageOptionsDialog(void);
private:
    void okButtonEvent(wxCommandEvent& event);
	void cancelButtonEvent(wxCommandEvent& event);
	void zoomButtonEvent(wxCommandEvent& event);
	void resetPerspectiveButtonEvent(wxCommandEvent& event);
	void translateButtonEvent(wxCommandEvent& event);
	void functionChooserEvent(wxCommandEvent& event);
	void colourChooserEvent(wxCommandEvent& event);
	void textEditEvent(wxCommandEvent& event);
	void invertImageCheckboxEvent(wxCommandEvent& event);
	void addHeatFunc(HeatMapFunc f, wxString name);
	void addColourProvider(HeatMapColourProvider *colourProvider, wxString name);
	void translateXPerspective(double factor);
	void translateYPerspective(double factor);
	void generatePreviewImage();
	void generateColourPreview();
	void generateImage(wxImage& image, HeatMapFunc heatFunc, HeatMapColourProvider &colourProvider, double xMin, double xMax, double yMin, double yMax, bool invertColours);
	wxString generateNewDefaultFilename(wxString outputDir, wxString originalName);
	void startImageGeneratorWorker(ImageOptions imageOptions);
	void OnThreadCompletion(wxThreadEvent&);
	void OnThreadUpdate(wxThreadEvent&);

	enum ID
	{
		ID_OK = 1,
		ID_CANCEL,
		ID_FUNCTION_CHOOSER,
		ID_COLOUR_CHOOSER,
		ID_XMIN,
		ID_XMAX,
		ID_YMIN,
		ID_YMAX,
		ID_XRES,
		ID_YRES,
		ID_ZOOM_IN,
		ID_ZOOM_OUT,
		ID_RESET_PERSPECTIVE,
		ID_INVERT_IMAGE,
		ID_START_THREAD,
		ID_LEFT,
		ID_RIGHT,
		ID_UP,
		ID_DOWN,
		ID_THREAD_UPDATE,
		ID_THREAD_COMPLETE,
	};

	std::vector<wxString> heatFunctionNames;
	std::vector<HeatMapFunc> heatFunctions;
	std::vector<wxString> colourProviderNames;
	std::vector<HeatMapColourProvider*> colourProviders;

	static const double ZOOM_IN_FACTOR;
	static const double ZOOM_OUT_FACTOR;

	static const double X_TRANSLATE_FACTOR;
	static const double Y_TRANSLATE_FACTOR;

	wxButton *okButton, *cancelButton;
	wxListBox *functionChooser;
	//wxStaticText *titleText, *titleText2, 
	wxStaticText *functionChooserLabel;
	wxTextCtrl *xMin, *xMax, *yMin, *yMax;
	wxStaticText *xMinLabel, *yMinLabel, *xMaxLabel, *yMaxLabel;
	wxButton *resetPerspectiveButton;
	wxStaticText *imagePreviewLabel, *resolutionLabel, *resolutionSeparator;
	wxTextCtrl *xRes, *yRes;
	wxButton *zoomInButton, *zoomOutButton;
	wxStaticText *colorSettingsLabel, *outputSettingsLabel;
	wxStaticText *colorLegendLabel_0, *colorLegendLabel_1;
	wxCheckBox *invertImageCheckBox;
	wxImagePanel *colourPreviewPanel, *imagePreviewPanel;
	wxButton *leftButton, *rightButton, *upButton, *downButton;
	wxStaticBox *functionGroupBox, *colourGroupBox, *outputSettingsGroupBox;
	
	wxImage previewImage, colourPreview;
	wxBitmap previewBitmap;

	HeatMapFunc heatMapFunc;
	HeatMapColourProvider *colourProvider;

	bool invertColours;

	//our worker thread to generate the output images
	ImageOptionsDialogWorker *imageGeneratorWorker;
	wxCriticalSection imageGeneratorWorkerCS;
	friend class ImageOptionsDialogWorker;

	wxDECLARE_EVENT_TABLE();
};

class ImageOptionsDialogWorker : public wxThread, public ImageGeneratorProgressListener
{
public:
	ImageOptionsDialogWorker(ImageOptionsDialog *handler)
		: wxThread(wxTHREAD_DETACHED)
	{
		this->handler = handler;
	}
	bool generateImage();
	bool generateImageMT();//Multithreaded generation - WIP
	void onProgressUpdate(int workDone, int totalWork);
	void onThreadProgressUpdate(int threadId, int workDone, int totalWork);
	~ImageOptionsDialogWorker();

	//image options
	ImageOptions imageOptions;

	std::vector<int> threadProgress;	//keeps track of worker thread progress
protected:
	virtual ExitCode Entry();
	ImageOptionsDialog *handler;
};

