#include "ImageOptionsDialog.h"
#include "ImageGenerator.h"
#include "HeatFuncs.h"
#include <algorithm>
#include <vector>
#include <wx/valnum.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>

//how much the preview image zooms in/out when the zoom controls are pressed.
const double ImageOptionsDialog::ZOOM_IN_FACTOR = 0.5;
const double ImageOptionsDialog::ZOOM_OUT_FACTOR = 2.0;

const double ImageOptionsDialog::X_TRANSLATE_FACTOR = 0.33;
const double ImageOptionsDialog::Y_TRANSLATE_FACTOR = 0.33;

wxBEGIN_EVENT_TABLE(ImageOptionsDialog, wxFrame)
	EVT_BUTTON(ID::ID_OK, ImageOptionsDialog::okButtonEvent)
	EVT_BUTTON(ID::ID_CANCEL, ImageOptionsDialog::cancelButtonEvent)
	EVT_BUTTON(ID::ID_ZOOM_IN, ImageOptionsDialog::zoomButtonEvent)
	EVT_BUTTON(ID::ID_ZOOM_OUT, ImageOptionsDialog::zoomButtonEvent)
	EVT_BUTTON(ID::ID_RESET_PERSPECTIVE, ImageOptionsDialog::resetPerspectiveButtonEvent)
	EVT_BUTTON(ID::ID_LEFT, ImageOptionsDialog::translateButtonEvent)
	EVT_BUTTON(ID::ID_RIGHT, ImageOptionsDialog::translateButtonEvent)
	EVT_BUTTON(ID::ID_UP, ImageOptionsDialog::translateButtonEvent)
	EVT_BUTTON(ID::ID_DOWN, ImageOptionsDialog::translateButtonEvent)
	EVT_LISTBOX(ID::ID_FUNCTION_CHOOSER, ImageOptionsDialog::functionChooserEvent)
	EVT_COMBOBOX(ID::ID_COLOUR_CHOOSER, ImageOptionsDialog::colourChooserEvent)
	EVT_TEXT(ID::ID_XMIN, ImageOptionsDialog::textEditEvent)
	EVT_TEXT(ID::ID_XMAX, ImageOptionsDialog::textEditEvent)
	EVT_TEXT(ID::ID_YMIN, ImageOptionsDialog::textEditEvent)
	EVT_TEXT(ID::ID_YMAX, ImageOptionsDialog::textEditEvent)
	EVT_CHECKBOX(ID::ID_INVERT_IMAGE, ImageOptionsDialog::invertImageCheckboxEvent)
	EVT_THREAD(ID::ID_THREAD_UPDATE, ImageOptionsDialog::OnThreadUpdate)
	EVT_THREAD(ID::ID_THREAD_COMPLETE, ImageOptionsDialog::OnThreadCompletion)
wxEND_EVENT_TABLE()

ImageOptionsDialog::ImageOptionsDialog(const wxString& title, const wxPoint& pos) 
: wxFrame(NULL, wxID_ANY, title, pos, wxSize(720, 480)) {
	//Note: don't call addChild() for the buttons if specifying the parent window in the constructor.
	
	//populate the list of available heatmap functions we can use to generate the image
	addHeatFunc(HeatMapFunc_SinXY, "sin(xy)");
	addHeatFunc(HeatMapFunc_CosXY, "cos(xy)");
	addHeatFunc(HeatMapFunc_E_POW_XY, "e^-abs(xy))");
	addHeatFunc(HeatMapFunc_XY, "abs(x*y)");
	addHeatFunc(HeatMapFunc_1_OVER_XY, "1_over_abs(xy)");
	addHeatFunc(HeatMapFunc_WEIRD_1, "Weird #1");
	addHeatFunc(HeatMapFunc_1_OVER_SIN_COS, "1_over_sin(x)cos(y)");
	addHeatFunc(HeatMapFunc_SIN_COS, "sin(x)cos(y)");
	addHeatFunc(HeatMapFunc_COS_COS_SIN, "cos^2(x)sin(y)");
	addHeatFunc(HeatMapFunc_COS_COS_MINUS_SIN, "cos^2(x) - sin(y)");	
	addHeatFunc(HeatMapFunc_DONUT, "Donut");
	addHeatFunc(HeatMapFunc_X_PLUS_SINXY, "x + sin(xy)");
	addHeatFunc(HeatMapFunc_X2_Y3_COSXY, "x^2y^3cos(xy)");
	addHeatFunc(HeatMapFunc_X2_Y2_E_X2_Y2, "x^2y^2e^-(x^2y^2)");
	addHeatFunc(HeatMapFunc_SIN_SQRT_X2_Y2, "sin(sqrt(x^2+y^2))");
	addHeatFunc(HeatMapFunc_SIN_1_OVER_XY, "sin(1/xy)");
	addHeatFunc(HeatMapFunc_RAND, "Noise");
	addHeatFunc(HeatMapFunc_SIN__SIN_COS, "sin(sin(x+cos(y))");
	addHeatFunc(HeatMapFunc_SIN_X2_Y2, "sin(x^2+y^2)");
	addHeatFunc(HeatMapFunc_X_PLUS_Y, "abs(x+y)");
	addHeatFunc(HeatMapFunc_HEART1, "Heart1");
	addHeatFunc(HeatMapFunc_HEART2, "Heart2");
	addHeatFunc(HeatMapFunc_CHECKERBOARD, "Checkerboard");
	addHeatFunc(HeatMapFunc_SPIRAL1, "Spiral1");
	addHeatFunc(HeatMapFunc_SPIRAL2, "Spiral2");
	addHeatFunc(HeatMapFunc_MANDELBROT, "Mandelbrot");
	//addHeatFunc(HeatMapFunc_F1, "F1");
	//addHeatFunc(HeatMapFunc_F2, "F2");
	//addHeatFunc(HeatMapFunc_F3, "F3");
	addHeatFunc(HeatMapFunc_GRADIENT, "Gradient");
	addHeatFunc(HeatMapFunc_ZERO, "0");
	addHeatFunc(HeatMapFunc_ONE, "1");

	//colour providers
	addColourProvider(new HeatMapColourProvider(), wxT("Default"));
	addColourProvider(new RoygbivColourProvider(), wxT("ROYGBIV"));
	addColourProvider(new RoygbivBWColourProvider(), wxT("ROYGBIV+BW"));
	addColourProvider(new GrayscaleColourProvider(), wxT("Grayscale"));
	addColourProvider(new PinkColourProvider(), wxT("Pink"));
	addColourProvider(new LogColourProvider(), wxT("Default (Log)"));
	addColourProvider(new RoygbivBWLogColourProvider(), wxT("ROYGBIV+BW (Log)"));
	addColourProvider(new TestColourProvider(), wxT("Test"));

	//initialize images for the preview image and the colour palette preview
	wxInitAllImageHandlers();
	previewImage.Create(wxSize(320,180));	//create a small preview image that refreshes real time when user changes any options.
	colourPreview.Create(wxSize(305, 20));

	//function chooser group box
	functionGroupBox = new wxStaticBox(this, wxID_ANY, wxT(""), wxPoint(10,10), wxSize(320, 130));
	functionChooserLabel = new wxStaticText(this, wxID_ANY, wxT("Function"), wxPoint(15, 20));
	wxListBox *functionChooser = new wxListBox(this, ID::ID_FUNCTION_CHOOSER, wxPoint(15, 35), wxSize(130, 90));
	for(unsigned int i = 0; i < this->heatFunctionNames.size(); i++){
		functionChooser->Append(this->heatFunctionNames[i]);
	}
	functionChooser->SetSelection(0);
	//currentFunctionSelectedIndex = 0;

	//xMin, xMax, yMin, yMax edit controls
	xMinLabel = new wxStaticText(this, wxID_ANY, wxT("xMin"), wxPoint(180, 30));
	xMaxLabel = new wxStaticText(this, wxID_ANY, wxT("xMax"), wxPoint(250, 30));
	yMinLabel = new wxStaticText(this, wxID_ANY, wxT("yMin"), wxPoint(180, 70));
	yMaxLabel = new wxStaticText(this, wxID_ANY, wxT("yMax"), wxPoint(250, 70));
	xMin = new wxTextCtrl(this, ID::ID_XMIN, wxT("-10.0"), wxPoint(180, 45), wxSize(60, 20), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC));
	xMax = new wxTextCtrl(this, ID::ID_XMAX, wxT("10.0"), wxPoint(250, 45), wxSize(60, 20), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC));
	yMin = new wxTextCtrl(this, ID::ID_YMIN, wxT("-10.0"), wxPoint(180, 85), wxSize(60, 20), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC));
	yMax = new wxTextCtrl(this, ID::ID_YMAX, wxT("10.0"), wxPoint(250, 85), wxSize(60, 20), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC));
	resetPerspectiveButton = new wxButton(this, ID::ID_RESET_PERSPECTIVE, wxT("Reset"), wxPoint(180, 110));

	imagePreviewLabel = new wxStaticText(this, wxID_ANY, wxT("Preview"), wxPoint(360, 50));
	zoomInButton = new wxButton(this, ID::ID_ZOOM_IN, wxT("Zoom +"), wxPoint(490, 40));
	zoomOutButton = new wxButton(this, ID::ID_ZOOM_OUT, wxT("Zoom -"), wxPoint(590, 40));
	imagePreviewPanel = new wxImagePanel(this, wxID_ANY, wxPoint(360, 70), wxSize(320, 180), &previewImage);

	//translation buttons
	upButton = new wxButton(this, ID::ID_UP, wxT("^"), wxPoint(480, 260));
	downButton = new wxButton(this, ID::ID_DOWN, wxT("v"), wxPoint(480, 320));
	leftButton = new wxButton(this, ID::ID_LEFT, wxT("<"), wxPoint(430, 290));
	rightButton = new wxButton(this, ID::ID_RIGHT, wxT(">"), wxPoint(530, 290));

	//colour settings
	colourGroupBox = new wxStaticBox(this, wxID_ANY, wxT(""), wxPoint(10, 150), wxSize(320, 100));
	colorSettingsLabel = new wxStaticText(this, wxID_ANY, wxT("Colors"), wxPoint(15, 160));
	colourPreviewPanel = new wxImagePanel(this, wxID_ANY, wxPoint(15, 180), wxSize(305, 20), &colourPreview);
	invertImageCheckBox = new wxCheckBox(this, ID::ID_INVERT_IMAGE, wxT("Invert Colours"), wxPoint(180, 220));
	invertColours = false;
	colorLegendLabel_0 = new wxStaticText(this, wxID_ANY, wxT("0.0"), wxPoint(15, 200));
	colorLegendLabel_1 = new wxStaticText(this, wxID_ANY, wxT("1.0"), wxPoint(305, 200));

	//colour chooser
	wxComboBox *colourChooser = new wxComboBox(this, ID::ID_COLOUR_CHOOSER, wxT(""), wxPoint(15, 220), wxSize(130, 25));
	for (unsigned int i = 0; i < this->colourProviderNames.size(); i++){
		colourChooser->Append(this->colourProviderNames[i]);
	}
	colourChooser->SetSelection(0);

	//output settings
	outputSettingsGroupBox = new wxStaticBox(this, wxID_ANY, wxT(""), wxPoint(10, 260), wxSize(320, 100));
	outputSettingsLabel = new wxStaticText(this, wxID_ANY, wxT("Output Settings"), wxPoint(15, 270));
	resolutionLabel = new wxStaticText(this, wxID_ANY, wxT("Resolution"), wxPoint(15, 300));
	resolutionSeparator = new wxStaticText(this, wxID_ANY, wxT("x"), wxPoint(82, 320));
	//get the current screen resolution and use that as the default value for the output resolution of the image
	wxString xResStr = wxString::Format(wxT("%i"), wxSystemSettings::GetMetric(wxSYS_SCREEN_X));
	wxString yResStr = wxString::Format(wxT("%i"), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y));
	xRes = new wxTextCtrl(this, ID::ID_XRES, xResStr, wxPoint(15, 320), wxSize(60, 20), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_DIGITS));
	yRes = new wxTextCtrl(this, ID::ID_YRES, yResStr, wxPoint(95, 320), wxSize(60, 20), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_DIGITS));

	//Add the Save and Cancel buttons
	okButton = new wxButton(this, ID::ID_OK, wxT("Save..."));
	okButton->SetPosition(wxPoint(80, 370));
	cancelButton = new wxButton(this, ID::ID_CANCEL, wxT("Cancel"));
	cancelButton->SetPosition(wxPoint(180, 370));

	//status bar
	CreateStatusBar(1);

	this->imageGeneratorWorker = 0;
	this->heatMapFunc = HeatMapFunc_SinXY;
	this->colourProvider = colourProviders[0];
	this->generatePreviewImage();
	this->generateColourPreview();
	this->Refresh();
}

//Save button event handler. Opens a save dialog which prompts the user to export the image as png.
void ImageOptionsDialog::okButtonEvent(wxCommandEvent& event){
	//read the values from the controls for the width and height of the output
	unsigned long imageWidthPx, imageHeightPx;
	bool res = this->xRes->GetValue().ToCULong(&imageWidthPx);
	res &= this->yRes->GetValue().ToCULong(&imageHeightPx);
	if(!res){
		wxMessageBox(wxT("Invalid resolution!"));
		return;
	}

	//read the values for xMin, xMax, yMin, yMax
	double xMinVal, yMinVal, xMaxVal, yMaxVal;
	res = xMin->GetValue().ToCDouble(&xMinVal);
	res &= xMax->GetValue().ToCDouble(&xMaxVal);
	res &= yMin->GetValue().ToCDouble(&yMinVal);
	res &= yMax->GetValue().ToCDouble(&yMaxVal);
	
	//if one of the values could not be read. Don't update the preview until we have valid values in all of the boxes.
	if(!res){	
		wxMessageBox(wxT("Invalid function domain!"));
		return;
	}

	wxListBox* functionListBox = (wxListBox*)this->FindWindowById(ID::ID_FUNCTION_CHOOSER);
	int selection = functionListBox->GetSelection();
	wxString functionName = functionListBox->GetString(selection);
	wxString defaultFilename = wxString::Format("%s_%dx%d", functionName, imageWidthPx, imageHeightPx);
	wxFileDialog saveFileDialog(this, wxT("Save image"), wxT(""), defaultFilename,
                       wxT("PNG file (*.png)|*.png"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	
	//check if we have a name conflict between the default filename and an existing file
	wxString defaultDirectory = saveFileDialog.GetDirectory();
	if (wxFileExists(defaultDirectory + "/" + defaultFilename)){
		defaultFilename = generateNewDefaultFilename(defaultDirectory, defaultFilename);
		saveFileDialog.SetFilename(defaultFilename);
	}

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
    
	//start a worker thread to save the image
	ImageOptions imageOptions;
	imageOptions.outputFilename = saveFileDialog.GetPath();
	imageOptions.heatMapFunc = this->heatMapFunc;
	imageOptions.colourProvider = this->colourProvider;
	imageOptions.invertColours = this->invertColours;
	imageOptions.xMin = xMinVal;
	imageOptions.xMax = xMaxVal;
	imageOptions.yMin = yMinVal;
	imageOptions.yMax = yMaxVal;
	imageOptions.xRes = imageWidthPx;
	imageOptions.yRes = imageHeightPx;
	this->startImageGeneratorWorker(imageOptions);
}

//generates a new default save filename for the user's output image. This is called if an image with that name already exists. 
//To resolve this, we will try appending "_1", "_2", "_3" etc until we have a unique filename
wxString ImageOptionsDialog::generateNewDefaultFilename(wxString outputDir, wxString originalName){
	int i = 1;
	wxString result;
	while (i < 1000){
		result = wxString::Format(wxT("%s_%d"), originalName);
		if (!wxFileExists(outputDir + "/" + result))
			return result;
		i++;
	}
	return originalName;
}

void ImageOptionsDialog::cancelButtonEvent(wxCommandEvent& event){
	this->Close(false);
}

//Event handler for zoom buttons. Zooms out by a percent of the current perspective size.
void ImageOptionsDialog::zoomButtonEvent(wxCommandEvent& event){
	
	//read the values from the edit controls
	double xMinVal, yMinVal, xMaxVal, yMaxVal;
	bool res = xMin->GetValue().ToCDouble(&xMinVal);
	res &= xMax->GetValue().ToCDouble(&xMaxVal);
	res &= yMin->GetValue().ToCDouble(&yMinVal);
	res &= yMax->GetValue().ToCDouble(&yMaxVal);
	
	//if one of the values could not be read, don't update the preview until we have valid values in all of the boxes.
	if(!res){	
		return;
	}

	//determine the point at the center of the screen. Should zoom in/out around this point
	double xCenter, yCenter, xDist, yDist;
	xCenter = (xMaxVal + xMinVal) / 2.0; 
	yCenter = (yMaxVal + yMinVal) / 2.0;
	xDist = xMaxVal - xCenter;
	yDist = yMaxVal - yCenter;

	//if xMin==xMax or yMin==yMax then zoom will get 'stuck', since xDist*ZOOM_IN_FACTOR will be zero.
	//Need a way to prevent this.
	
	//multiply the coordinates by a factor less than 1 to zoom in or more than 1 to zoom out
	switch(event.GetId()){
	case ID::ID_ZOOM_IN:
		xMinVal = xCenter - xDist*ZOOM_IN_FACTOR;
		xMaxVal = xCenter + xDist*ZOOM_IN_FACTOR;
		yMinVal = yCenter - yDist*ZOOM_IN_FACTOR;
		yMaxVal = yCenter + yDist*ZOOM_IN_FACTOR;
		break;
	case ID::ID_ZOOM_OUT:
		xMinVal = xCenter - xDist*ZOOM_OUT_FACTOR;
		xMaxVal = xCenter + xDist*ZOOM_OUT_FACTOR;
		yMinVal = yCenter - yDist*ZOOM_OUT_FACTOR;
		yMaxVal = yCenter + yDist*ZOOM_OUT_FACTOR;
		break;
	}

	//update the edit controls with the new values
	xMin->SetValue(wxString::FromCDouble(xMinVal));
	xMax->SetValue(wxString::FromCDouble(xMaxVal));
	yMin->SetValue(wxString::FromCDouble(yMinVal));
	yMax->SetValue(wxString::FromCDouble(yMaxVal));

	//generate the preview image and redraw the screen
	this->generatePreviewImage();
	this->Refresh();
}

//resets the perspective back to the default of ((-10,10),(-10,10))
void ImageOptionsDialog::resetPerspectiveButtonEvent(wxCommandEvent& event){
	xMin->SetValue(wxString::FromCDouble(-10.0));
	xMax->SetValue(wxString::FromCDouble(10.0));
	yMin->SetValue(wxString::FromCDouble(-10.0));
	yMax->SetValue(wxString::FromCDouble(10.0));

	//generate the preview image and redraw the screen
	this->generatePreviewImage();
	this->Refresh();
}

//Event handler for text edit events. Called whenever any of the values of xMin, xMax, yMin, yMax are changed. 
void ImageOptionsDialog::textEditEvent(wxCommandEvent& event){
	//The values of xMin, xMax, yMin, yMax have changed; we need to regenerate the preview image
	this->generatePreviewImage();
	
	//redraw the preview image
	this->Refresh();
}

//adds a heat function to the list of functions available in the function chooser
void ImageOptionsDialog::addHeatFunc(HeatMapFunc f, wxString name){
	this->heatFunctions.push_back(f);
	this->heatFunctionNames.push_back(name);
}

//adds a colour provider to the list available in the function chooser
void ImageOptionsDialog::addColourProvider(HeatMapColourProvider *colourProvider, wxString name){
	this->colourProviders.push_back(colourProvider);
	this->colourProviderNames.push_back(name);
}

//generates a small 320x180 preview image that will update in real time as the user fiddles with the other settings
void ImageOptionsDialog::generatePreviewImage(){
	ImageGenerator imageGenerator;
	double xMinVal, yMinVal, xMaxVal, yMaxVal;
	bool res = xMin->GetValue().ToCDouble(&xMinVal);
	res &= xMax->GetValue().ToCDouble(&xMaxVal);
	res &= yMin->GetValue().ToCDouble(&yMinVal);
	res &= yMax->GetValue().ToCDouble(&yMaxVal);
	
	//if one of the values could not be read. Don't update the preview until we have valid values in all of the boxes.
	if(!res){	
		return;
	}
	
	//set the colour function used with the heatmap
	imageGenerator.setColourProvider(this->colourProvider);
	imageGenerator.setInvertColours(this->invertColours);
	imageGenerator.setFunction(heatMapFunc, xMinVal, xMaxVal, yMinVal, yMaxVal);
	
	//overwrite the image data with the new generated image.
	imageGenerator.generateImage(&this->previewImage);	//generateImage() preserves the size of the input image.
}

//Generates the color palette preview image
void ImageOptionsDialog::generateColourPreview(){
	ImageGenerator imageGenerator;
	
	//set the colour function used with the heatmap
	imageGenerator.setColourProvider(this->colourProvider);
	imageGenerator.setInvertColours(this->invertColours);
	imageGenerator.setFunction(HeatMapFunc_GRADIENT, -10.0, 10.0, -10.0, 10.0);
	
	//overwrite the image data with the new generated image.
	imageGenerator.generateImage(&this->colourPreview);	//generateImage() preserves the size of the input image.
}

//Event for the function chooser allowing the user to choose the heat function
void ImageOptionsDialog::functionChooserEvent(wxCommandEvent& event){
	int selection = event.GetSelection();
	if(selection >= 0 && selection < (int)heatFunctions.size()){
		this->heatMapFunc = heatFunctions[selection];
	}

	//this->currentFunctionSelectedIndex = selection;
	this->generatePreviewImage();
	this->Refresh();
}

//Event for the colour chooser allowing the user to choose the heat function
void ImageOptionsDialog::colourChooserEvent(wxCommandEvent& event){
	int selection = event.GetSelection();
	if (selection >= 0 && selection < (int)heatFunctions.size()){
		this->colourProvider = colourProviders[selection];
	}

	this->generatePreviewImage();
	this->generateColourPreview();
	this->Refresh();
}

//Inverts the colour palette. The heatmap colour provider will be given 1-heatVal instead of heatVal for heatmap generation
void ImageOptionsDialog::invertImageCheckboxEvent(wxCommandEvent& event){
	this->invertColours = event.IsChecked();
	this->generatePreviewImage();
	this->generateColourPreview();
	this->Refresh();
}

//translate buttons
void ImageOptionsDialog::translateButtonEvent(wxCommandEvent& event){
	switch(event.GetId()){
	case ID::ID_UP:
		translateYPerspective(-Y_TRANSLATE_FACTOR);
		break;
	case ID::ID_DOWN:
		translateYPerspective(Y_TRANSLATE_FACTOR);
		break;
	case ID::ID_LEFT:
		translateXPerspective(-X_TRANSLATE_FACTOR);
		break;
	case ID::ID_RIGHT:
		translateXPerspective(X_TRANSLATE_FACTOR);
		break;
	default:
		return;
		break;
	}
	this->generatePreviewImage();
	this->Refresh();
}

//Shifts the perspective along the x axis by a factor depending on the size of the current perspective.
//For example, if factor==0.5 then it will shift the perspective by half a screen in the positive x direction
void ImageOptionsDialog::translateXPerspective(double factor){
	double xMinVal, xMaxVal;
	bool res = xMin->GetValue().ToCDouble(&xMinVal);
	res &= xMax->GetValue().ToCDouble(&xMaxVal);
	
	//if one of the values could not be read, we can't finish the calculation
	if(!res){	
		return;
	}

	double xTrans = (xMaxVal - xMinVal) * factor;
	xMinVal += xTrans;
	xMaxVal += xTrans;

	xMin->SetValue(wxString::FromCDouble(xMinVal));
	xMax->SetValue(wxString::FromCDouble(xMaxVal));
}

//Shifts the perspective along the y axis by a factor depending on the size of the current perspective.
//For example, if factor==0.5 then it will shift the perspective by half a screen in the positive y direction
void ImageOptionsDialog::translateYPerspective(double factor){
	double yMinVal, yMaxVal;
	bool res = yMin->GetValue().ToCDouble(&yMinVal);
	res &= yMax->GetValue().ToCDouble(&yMaxVal);
	
	//if one of the values could not be read, we can't finish the calculation
	if(!res){	
		return;
	}

	double yTrans = (yMaxVal - yMinVal) * factor;
	yMinVal += yTrans;
	yMaxVal += yTrans;

	yMin->SetValue(wxString::FromCDouble(yMinVal));
	yMax->SetValue(wxString::FromCDouble(yMaxVal));
}

void ImageOptionsDialog::generateImage(wxImage& image, HeatMapFunc heatFunc, HeatMapColourProvider &colourProvider, double xMin, double xMax, double yMin, double yMax, bool invertColours){
	ImageGenerator imageGenerator;

	//set the heatmap function used to generate the preview image
	imageGenerator.setFunction(heatMapFunc, xMin, xMax, yMin, yMax);

	//set the colour function used with the heatmap
	imageGenerator.setColourProvider(this->colourProvider);
	imageGenerator.setInvertColours(this->invertColours);

	//overwrite the image data with the new generated image.
	imageGenerator.generateImage(&image);	//generateImage() preserves the size of the input image.
}

////////worker thread//////////
void ImageOptionsDialog::startImageGeneratorWorker(ImageOptions imageOptions){
	wxCriticalSectionLocker enter(this->imageGeneratorWorkerCS);
	//start the thread
	if (this->imageGeneratorWorker == 0){
		this->imageGeneratorWorker = new ImageOptionsDialogWorker(this);

		//copy the image generation parameters for the thread
		this->imageGeneratorWorker->imageOptions = imageOptions;

		if (this->imageGeneratorWorker->Run() != wxTHREAD_NO_ERROR){
			wxMessageBox(wxT("There was an error creating the worker thread."));
		}
		else{
			//thread was created ok. Disable the 'save' button until the thread has finished writing the image
			this->okButton->Disable();
		}
	}
	else{
		wxMessageBox(wxT("Error: worker thread already running."));
	}
}

void ImageOptionsDialog::OnThreadCompletion(wxThreadEvent&){
	this->SetStatusText(wxT(""));
	this->okButton->Enable();	//enable the save button again
}
void ImageOptionsDialog::OnThreadUpdate(wxThreadEvent& threadEvent){
	int progress = threadEvent.GetInt();
	if (progress < 100){
		wxString msg = wxString::Format("Generating image (%d%%)...", threadEvent.GetInt());
		this->SetStatusText(msg);
	}
	else{
		this->SetStatusText(wxT("Saving image..."));
	}
}

//The ImageOptionsDialogWorker thread is passed the parameters through the imageOptions member by the main thread.
wxThread::ExitCode ImageOptionsDialogWorker::Entry()
{
	this->generateImage();
	// signal the event handler that this thread is going to be destroyed
	wxQueueEvent(this->handler, new wxThreadEvent(wxEVT_THREAD, ImageOptionsDialog::ID::ID_THREAD_COMPLETE));
	return (wxThread::ExitCode)0;     // success
}

bool ImageOptionsDialogWorker::generateImage(){
	wxFileOutputStream outputStream(imageOptions.outputFilename);
	if (!outputStream.IsOk()){
		wxLogError("Cannot open file '%s'.", imageOptions.outputFilename);
		return false;
	}

	//generate the image and write to file
	ImageGenerator imageGenerator;
	wxImage outputImage(imageOptions.xRes, imageOptions.yRes);
	if (!outputImage.IsOk()){
		wxMessageBox(wxT("Failed to create the image!"));
		return false;
	}

	//set the colour function used with the heatmap
	imageGenerator.setColourProvider(imageOptions.colourProvider);
	imageGenerator.setInvertColours(imageOptions.invertColours);
	imageGenerator.setFunction(imageOptions.heatMapFunc, imageOptions.xMin, imageOptions.xMax, imageOptions.yMin, imageOptions.yMax);
	imageGenerator.setProgressListener(this);

	//generate the final image
	imageGenerator.generateImage(&outputImage);

	bool res = outputImage.SaveFile(outputStream, wxBITMAP_TYPE_PNG);
	if (!res){
		wxMessageBox(wxT("There was a problem saving the image."), wxT("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

//gets called from ImageGenerator while generating the image
void ImageOptionsDialogWorker::onProgressUpdate(int workDone, int totalWork){
	//send a progress update event to the main thread
	wxThreadEvent *threadEvent = new wxThreadEvent(wxEVT_THREAD, ImageOptionsDialog::ID::ID_THREAD_UPDATE);
	threadEvent->SetInt(workDone*100/totalWork);
	wxQueueEvent(this->handler, threadEvent);
}

ImageOptionsDialogWorker::~ImageOptionsDialogWorker()
{
	wxCriticalSectionLocker enter(this->handler->imageGeneratorWorkerCS);
	// the thread is being destroyed; make sure not to leave dangling pointers around
	this->handler->imageGeneratorWorker = NULL;
}

