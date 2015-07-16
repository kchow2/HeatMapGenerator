#pragma once
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