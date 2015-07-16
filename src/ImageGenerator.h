#pragma once
#include <wx/wx.h>
#include "HeatMapColourProvider.h"
#include "HeatFuncs.h"

struct ImageOptions{
	wxString outputFilename;
	double xMin, xMax, yMin, yMax;
	int xRes, yRes;
	HeatMapFunc heatMapFunc;
	HeatMapColourProvider *colourProvider;
	bool invertColours;
};

class ImageGeneratorProgressListener{
public:
	virtual void onProgressUpdate(int workDone, int totalWork) = 0;
};

class ImageGenerator
{
public:
	
	ImageGenerator(void);
	void setInvertColours(bool invertColours);
	void setFunction(HeatMapFunc f, double xMin, double xMax, double yMin, double yMax);
	void setColourProvider(HeatMapColourProvider *colourProvider);
	void generateImage(wxImage *image);
	void setProgressListener(ImageGeneratorProgressListener* listener);
	virtual ~ImageGenerator(void);

private:
	void generateImageMT(wxImage *image, int startRow, int endRow);
	double xMin, xMax, yMin, yMax;
	bool invertColours;
	HeatMapFunc heatMapFunc;
	HeatMapColourProvider *colourProvider;
	ImageGeneratorProgressListener *progressListener;
};

