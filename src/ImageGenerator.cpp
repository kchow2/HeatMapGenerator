#include "ImageGenerator.h"
#include "HeatMapColourProvider.h"

ImageGenerator::ImageGenerator(void){
	this->heatMapFunc = 0;
	this->xMin = -1.0;
	this->xMax = 1.0;
	this->yMin = -1.0;
	this->yMax = 1.0;
	this->invertColours = false;
	this->progressListener = 0;
}

ImageGenerator::~ImageGenerator(void){

}

void ImageGenerator::setColourProvider(HeatMapColourProvider *colourProvider){
	this->colourProvider = colourProvider;
}

void ImageGenerator::setFunction(HeatMapFunc f, double xMin, double xMax, double yMin, double yMax){
	this->heatMapFunc = f;
	this->xMin = xMin;
	this->xMax = xMax;
	this->yMin = yMin;
	this->yMax = yMax;
}

void ImageGenerator::setInvertColours(bool invertColours){
	this->invertColours = invertColours;
}

void ImageGenerator::setProgressListener(ImageGeneratorProgressListener *listener){
	this->progressListener = listener;
}

void ImageGenerator::generateImage(wxImage *image){
	//TODO: if this is a very large image, do this in a worker thread so the UI doesn't lock up
	//use the width and height of the input image.
	int imageWidthPx = image->GetWidth();
	int imageHeightPx = image->GetHeight();

	double xScale = (xMax - xMin) / imageWidthPx;
	double yScale = (yMax - yMin) / imageHeightPx;

	if(!heatMapFunc){
		wxMessageBox(wxT("Error: heatMapFunc is null!"));
		return;
	}
	if(!colourProvider){
		wxMessageBox(wxT("Error: colourProvider is null!"));
		return;
	}
	
	HeatMapColourProvider::Colour colour;
	
	for(int yPx = 0; yPx < imageHeightPx; yPx++){
		for(int xPx = 0; xPx < imageWidthPx; xPx++){
			double x = xMin + xPx*xScale;
			double y = yMin + yPx*yScale;
		
			double heatVal = (*heatMapFunc)(x, y);
			if (invertColours)
				heatVal = 1.0 - heatVal;

			colour = colourProvider->getHeatMapColour(heatVal);
			image->SetRGB(wxRect(xPx,yPx,1,1), colour.r, colour.g, colour.b);
		}
		if (this->progressListener){
			this->progressListener->onProgressUpdate(yPx+1, imageHeightPx);
		}
	}
}


