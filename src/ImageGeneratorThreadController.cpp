#include "ImageGeneratorThreadController.h"
#include  <wx/wx.h>
#include <wx/wfstream.h>
#include <wx/filename.h>

//The ImageGeneratorThreadController thread is passed the parameters through the imageOptions member by the main thread.
wxThread::ExitCode ImageGeneratorThreadController::Entry()
{
	this->generateImage();
	// signal the event handler that this thread is going to be destroyed
	wxQueueEvent(this->handler, new wxThreadEvent(wxEVT_THREAD, ImageOptionsDialog::ID::ID_THREAD_COMPLETE));
	return (wxThread::ExitCode)0;     // success
}

bool ImageGeneratorThreadController::generateImage(){
	this->wasCancelled = false;

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
	imageGenerator.setSSAALevel(imageOptions.ssaaLevel);
	imageGenerator.setProgressListener(this);

	//generate the final image
	imageGenerator.generateImage(&outputImage);

	//if the user cancelled, return immediately without saving the file
	if (this->wasCancelled){
		return false;
	}

	//notify the main window we have finished generating the image
	wxQueueEvent(this->handler, new wxThreadEvent(wxEVT_THREAD, ImageOptionsDialog::ID::ID_THREAD_GENERATION_FINISHED));

	//save the png file
	wxFileOutputStream outputStream(imageOptions.outputFilename);
	if (!outputStream.IsOk()){
		wxLogError("Cannot open file '%s'.", imageOptions.outputFilename);
		return false;
	}
	bool res = outputImage.SaveFile(outputStream, wxBITMAP_TYPE_PNG);
	if (!res){
		wxMessageBox(wxT("There was a problem saving the image."), wxT("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

//gets called from ImageGenerator while generating the image
bool ImageGeneratorThreadController::onProgressUpdate(int workDone, int totalWork){
	if (this->TestDestroy()){
		this->wasCancelled = true;
		return false; 
	}
	//send a progress update event to the main thread
	wxThreadEvent *threadEvent = new wxThreadEvent(wxEVT_THREAD, ImageOptionsDialog::ID::ID_THREAD_UPDATE);
	threadEvent->SetInt(workDone * 100 / totalWork);
	wxQueueEvent(this->handler, threadEvent);
	return true;
}

ImageGeneratorThreadController::~ImageGeneratorThreadController()
{
	wxCriticalSectionLocker enter(this->handler->imageGeneratorWorkerCS);
	// the thread is being destroyed; make sure not to leave dangling pointers around
	this->handler->imageGeneratorThreadController = NULL;
}