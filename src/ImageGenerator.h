#pragma once
#include <wx/wx.h>
#include "HeatMapColourProvider.h"
#include "HeatFuncs.h"

struct ImageOptions{
	wxString outputFilename;
	double xMin, xMax, yMin, yMax;
	int xRes, yRes;
	int ssaaLevel;
	HeatMapFunc heatMapFunc;
	HeatMapColourProvider *colourProvider;
	bool invertColours;
};

class ImageGeneratorProgressListener{
public:
	virtual bool onProgressUpdate(int workDone, int totalWork) = 0;
};

class ImageGenerator
{
public:
	
	ImageGenerator(void);
	void setInvertColours(bool invertColours);
	void setFunction(HeatMapFunc f, double xMin, double xMax, double yMin, double yMax);
	void setColourProvider(HeatMapColourProvider *colourProvider);
	void setSSAALevel(int nSamplesPerPixel);
	void generateImage(wxImage *image);
	void setProgressListener(ImageGeneratorProgressListener* listener);
	virtual ~ImageGenerator(void);

private:
	void generateImageMT(wxImage *image, int startRow, int endRow);
	double xMin, xMax, yMin, yMax;
	bool invertColours;
	int ssaaLevel;
	HeatMapFunc heatMapFunc;
	HeatMapColourProvider *colourProvider;
	ImageGeneratorProgressListener *progressListener;
};

