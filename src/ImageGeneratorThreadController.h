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
	bool generateImageMT(int nThreads);
	bool onProgressUpdate(int workDone, int totalWork);
	bool onWorkerThreadProgressUpdate(int threadId, int workDone, int totalWork);
	~ImageGeneratorThreadController();

	//image options
	ImageOptions imageOptions;
	bool wasCancelled;

	//for thread progress updates - need to use CS to access this!
	wxCriticalSection rowsGeneratedCS;
	int rowsGenerated, totalRows;
protected:
	virtual ExitCode Entry();
	ImageOptionsDialog *handler;
};