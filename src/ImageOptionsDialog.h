#pragma once
#include <wx/wx.h>
#include <vector>
#include "ImageGenerator.h"
#include "HeatMapColourProvider.h"
#include "wxImagePanel.h"
#include "ImageGeneratorThreadController.h"

//struct ImageOptions;
class ImageGeneratorThreadController;

class ImageOptionsDialog : public wxFrame
{
public:
	ImageOptionsDialog(const wxString& title, const wxPoint& pos);
	//~ImageOptionsDialog(void);
private:
	void initInterpolationModes();
	void initSSAAOptions();
    void okButtonEvent(wxCommandEvent& event);
	void cancelButtonEvent(wxCommandEvent& event);
	void zoomButtonEvent(wxCommandEvent& event);
	void resetPerspectiveButtonEvent(wxCommandEvent& event);
	void translateButtonEvent(wxCommandEvent& event);
	void testImageButtonEvent(wxCommandEvent& event);
	void functionChooserEvent(wxCommandEvent& event);
	void colourChooserEvent(wxCommandEvent& event);
	void interpolationModeChooserEvent(wxCommandEvent& event);
	void ssaaChooserEvent(wxCommandEvent& event);
	void textEditEvent(wxCommandEvent& event);
	void invertImageCheckboxEvent(wxCommandEvent& event);
	void addHeatFunc(HeatMapFunc f, wxString name);
	void addColourProvider(HeatMapColourProvider *colourProvider, wxString name);
	void translateXPerspective(double factor);
	void translateYPerspective(double factor);
	void generatePreviewImage();
	void generateColourPreview();
	void generateImage(wxImage& image, HeatMapFunc heatFunc, HeatMapColourProvider &colourProvider, HeatMapColourProvider::COLOUR_INTERPOLATION_MODE interpolationMode, double xMin, double xMax, double yMin, double yMax, bool invertColours);
	wxString generateNewDefaultFilename(wxString outputDir, wxString originalName);
	void startImageGeneratorThreadController(ImageOptions imageOptions);
	void onThreadCompletion(wxThreadEvent&);
	void onThreadUpdate(wxThreadEvent&);
	void onThreadGenerationFinished(wxThreadEvent& threadEvent);
	void onClose(wxCloseEvent& event);
	void cleanup();

	enum ID
	{
		ID_OK = 1,
		ID_CANCEL,
		ID_FUNCTION_CHOOSER,
		ID_COLOUR_CHOOSER,
		ID_INTERPOLATION_MODE_CHOOSER,
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
		ID_SSAA_CHOOSER,
		ID_LEFT,
		ID_RIGHT,
		ID_UP,
		ID_DOWN,
		ID_TEST_IMAGE,
		ID_THREAD_UPDATE,
		ID_THREAD_GENERATION_FINISHED,
		ID_THREAD_COMPLETE,	
	};

	enum SSAA_OPTIONS{
		SSAA_NONE=1,
		SSAA_2X,
		SSAA_4X,
		SSAA_8X,
		SSAA_16X,
	};

	std::vector<wxString> heatFunctionNames;
	std::vector<HeatMapFunc> heatFunctions;
	std::vector<wxString> colourProviderNames;
	std::vector<HeatMapColourProvider*> colourProviders;
	std::vector<wxString> interpolationModeNames;
	std::vector<HeatMapColourProvider::COLOUR_INTERPOLATION_MODE> interpolationModes;
	std::vector<wxString> ssaaOptionNames;
	std::vector<SSAA_OPTIONS> ssaaOptions;
	std::vector<int> ssaaLevels;

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
	int ssaaLevel;
	HeatMapColourProvider::COLOUR_INTERPOLATION_MODE colourInterpolationMode;

	//used for worker thread. Flag that indicates whether the main window should close when the worker thread finishes.
	bool shouldQuit;

	//our worker thread to generate the output images
	ImageGeneratorThreadController *imageGeneratorThreadController;
	wxCriticalSection imageGeneratorWorkerCS;
	friend class ImageGeneratorThreadController;

	wxDECLARE_EVENT_TABLE();
};

