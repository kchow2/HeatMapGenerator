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
	HeatMapColourProvider::COLOUR_INTERPOLATION_MODE colourInterpolationMode;
	bool invertColours;
};

struct ImageGenResult {
	wxImage *image;
	bool success;
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
	void setColourProvider(HeatMapColourProvider *colourProvider, HeatMapColourProvider::COLOUR_INTERPOLATION_MODE interpolationMode);
	void setSSAALevel(int nSamplesPerPixel);
	void generateImage(wxImage *image);
	void generateImageRow(wxImage* image, int row);
	void setProgressListener(ImageGeneratorProgressListener* listener);
	virtual ~ImageGenerator(void);

private:
	double xMin, xMax, yMin, yMax;
	bool invertColours;
	int ssaaLevel;
	HeatMapFunc heatMapFunc;
	HeatMapColourProvider *colourProvider;
	ImageGeneratorProgressListener *progressListener;
};

