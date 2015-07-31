#pragma once

#include <wx/wx.h>
#include <wx/thread.h>
#include "ImageGenerator.h"
#include "ImageGeneratorThreadController.h"

class ImageGeneratorThreadWorker : public wxThread{
public:
	ImageGeneratorThreadWorker(ImageGeneratorThreadController *controller, wxImage *outputImage, ImageOptions imageOptions, int startRow, int endRow, int threadId) 
		: wxThread(wxTHREAD_JOINABLE), controller(controller), outputImage(outputImage), imageOptions(imageOptions), startRow(startRow), endRow(endRow), threadId(threadId) {};
	~ImageGeneratorThreadWorker(){};

	//Input parameters set by thread creator
	int threadId;
	int startRow, endRow;
	ImageGeneratorThreadController *controller;
	ImageOptions imageOptions;
	wxImage *outputImage;	//shared pointer to output image.
private:
	virtual ExitCode Entry();
};