#pragma once

#include "ImageOptionsDialog.h"
#include "ImageGenerator.h"
#include <wx/wx.h>

class ImageOptionsDialog;

class ImageGeneratorThreadController : public wxThread, public ImageGeneratorProgressListener
{
public:
	ImageGeneratorThreadController(ImageOptionsDialog *handler)
		: wxThread(wxTHREAD_DETACHED)
	{
		this->handler = handler;
	}
	bool generateImage();
	bool onProgressUpdate(int workDone, int totalWork);
	bool onThreadProgressUpdate(int threadId, int workDone, int totalWork);
	~ImageGeneratorThreadController();

	//image options
	ImageOptions imageOptions;
	bool wasCancelled;
protected:
	virtual ExitCode Entry();
	ImageOptionsDialog *handler;
};