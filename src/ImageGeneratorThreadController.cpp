#include "ImageGeneratorThreadController.h"
#include "ImageGeneratorThreadWorker.h"
#include  <wx/wx.h>
#include <wx/wfstream.h>
#include <wx/filename.h>

//The ImageGeneratorThreadController thread is passed the parameters through the imageOptions member by the main thread.
wxThread::ExitCode ImageGeneratorThreadController::Entry()
{
	//wxStopWatch timer;
	//timer.Start();
	this->generateImageMT(wxThread::GetCPUCount()); //create as many cores as we have CPU cores
	//timer.Pause();
	//int timeElapsedMs = (int)timer.Time();
	//wxMessageBox(wxString::Format("Generation took %d ms.", timeElapsedMs));
	
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
	imageGenerator.setColourProvider(imageOptions.colourProvider, imageOptions.colourInterpolationMode);
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

bool ImageGeneratorThreadController::generateImageMT(int nThreads){
	this->wasCancelled = false;

	//generate the image and write to file
	wxImage outputImage(imageOptions.xRes, imageOptions.yRes);
	if (!outputImage.IsOk()){
		wxMessageBox(wxT("Failed to create the image!"));
		return false;
	}

	//set up progress updating for the threads
	this->rowsGenerated = 0;
	this->totalRows = imageOptions.yRes;

	//create N threads. Each thread will generate 1/N of the image
	std::vector<wxThread*> threads;
	threads.reserve(nThreads);
	int rowsPerThread = this->totalRows / nThreads;

	for (int i = 0; i < nThreads - 1; i++){
		wxThread *thread = new ImageGeneratorThreadWorker(this, &outputImage, this->imageOptions, i*rowsPerThread, (i+1)*rowsPerThread, 0);
		thread->Run();
		threads.push_back(thread);
	}
	wxThread *thread = new ImageGeneratorThreadWorker(this, &outputImage, this->imageOptions, (nThreads-1)*rowsPerThread, this->totalRows, 0);
	thread->Run();
	threads.push_back(thread);
	for (unsigned int i = 0; i < threads.size(); i++){
		threads[i]->Wait();
	}
	//cleanup the memory
	for (unsigned int i = 0; i < threads.size(); i++){
		delete threads[i];
	}
	threads.clear();

	//notify the main window we have finished generating the image
	wxQueueEvent(this->handler, new wxThreadEvent(wxEVT_THREAD, ImageOptionsDialog::ID::ID_THREAD_GENERATION_FINISHED));

	//if the user cancelled, return immediately without saving the file
	//If we save the image when the user cancelled in the middle of generation, we will end up with a half generated image with black parts where it wasn't generated yet.
	if (this->wasCancelled){
		return false;
	}

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

//gets called from worker threads while generating the image
bool ImageGeneratorThreadController::onWorkerThreadProgressUpdate(int threadId, int workDone, int totalWork){
	if (this->TestDestroy()){
		this->wasCancelled = true;
		return false;
	}

	wxCriticalSectionLocker csLocker(this->rowsGeneratedCS);
	this->rowsGenerated++;
	//send a progress update event to the main thread
	wxThreadEvent *threadEvent = new wxThreadEvent(wxEVT_THREAD, ImageOptionsDialog::ID::ID_THREAD_UPDATE);
	threadEvent->SetInt(this->rowsGenerated * 100 / this->totalRows);
	wxQueueEvent(this->handler, threadEvent);
	return true;
}

ImageGeneratorThreadController::~ImageGeneratorThreadController()
{
	wxCriticalSectionLocker enter(this->handler->imageGeneratorWorkerCS);
	// the thread is being destroyed; make sure not to leave dangling pointers around
	this->handler->imageGeneratorThreadController = NULL;
}