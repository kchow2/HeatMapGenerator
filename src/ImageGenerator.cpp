#include "ImageGenerator.h"
#include "HeatMapColourProvider.h"

double xSampleCoords[8] = { -0.115, 0.334, 0.115, -0.334, -0.150, 0.150, 0.150, -0.150 };
double ySampleCoords[8] = { -0.334, 0.115, 0.334, -0.115, -0.150, 0.150, 0.150, -0.150 };

ImageGenerator::ImageGenerator(void){
	this->heatMapFunc = 0;
	this->xMin = -1.0;
	this->xMax = 1.0;
	this->yMin = -1.0;
	this->yMax = 1.0;
	this->invertColours = false;
	this->progressListener = 0;
	this->ssaaLevel = 1;
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

void ImageGenerator::setSSAALevel(int nSamplesPerPixel){
	this->ssaaLevel = nSamplesPerPixel;
}

void ImageGenerator::setInvertColours(bool invertColours){
	this->invertColours = invertColours;
}

void ImageGenerator::setProgressListener(ImageGeneratorProgressListener *listener){
	this->progressListener = listener;
}

void ImageGenerator::generateImage(wxImage *image){
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
	
	HeatMapColourProvider::Colour colour, sampleColour;
	int maxSamples = this->ssaaLevel;
	//double xSamplingSpacing = xScale / maxSamples;
	//double ySamplingSpacing = yScale / maxSamples;
	
	for(int yPx = 0; yPx < imageHeightPx; yPx++){
		for(int xPx = 0; xPx < imageWidthPx; xPx++){
			colour.r = 0;
			colour.g = 0;
			colour.b = 0;
			for (int sample = 0; sample < maxSamples; sample++){	//supersampling
				
				double x = xMin + xPx*xScale + xSampleCoords[sample] * xScale;
				double y = yMin + yPx*yScale + ySampleCoords[sample] * yScale;

				double heatVal = (*heatMapFunc)(x, y);
				if (invertColours)
					heatVal = 1.0 - heatVal;

				sampleColour = colourProvider->getHeatMapColour(heatVal);
				//accumulate the samples in colour
				colour.r += sampleColour.r;
				colour.g += sampleColour.g;
				colour.b += sampleColour.b;
				
			}

			//average out all the samples to form the final colour
			colour.r /= maxSamples;
			colour.g /= maxSamples;
			colour.b /= maxSamples;
			image->SetRGB(wxRect(xPx, yPx, 1, 1), colour.r, colour.g, colour.b);
		}
		if (this->progressListener){
			bool shouldContinue = this->progressListener->onProgressUpdate(yPx+1, imageHeightPx);
			if (!shouldContinue)
				break;
		}
	}
}


