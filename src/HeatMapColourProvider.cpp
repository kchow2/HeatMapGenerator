#include "HeatMapColourProvider.h"
#include <algorithm>

//helper func to interpolate values linearly from a colour array
HeatMapColourProvider::Colour HeatMapColourProvider::getInterpolatedColourValue(double value, const int *rValues, const int *gValues, const int *bValues, int colourArraySize){
	int scaledIntensity = (int)(value*(colourArraySize - 1) * 100);
	int step = (int)(scaledIntensity / 100);	//the 'step' is which two values in the array we should be interpolating between
	double stepDistance = (scaledIntensity - 100.0*step) / 100;	//the distance the value is from the step

	//return the linearly interpolated value between colourArray[step] and colourArray[step+1]
	int r = (int)((rValues[step + 1] - rValues[step])*stepDistance + rValues[step]);
	int g = (int)((gValues[step + 1] - gValues[step])*stepDistance + gValues[step]);
	int b = (int)((bValues[step + 1] - bValues[step])*stepDistance + bValues[step]);
	return Colour(r, g, b);
}

//The default colour provider
HeatMapColourProvider::Colour HeatMapColourProvider::getHeatMapColour(double heatValue){
	//0.0-1.0    black-blue-cyan-green-yellow-red-white
	heatValue = std::max(0.0, std::min(heatValue,1.0));	//clamp intensity to range [0.0, 1.0]
	
	const int nColours = 7;
	const int rValues[] = { 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF };
	const int gValues[] = { 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF };
	const int bValues[] = { 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

//colour ranges from black at 0.0 to white at 1.0
HeatMapColourProvider::Colour GrayscaleColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	
	const int nColours = 2;
	const int rValues[] = { 0x00, 0xFF };
	const int gValues[] = { 0x00, 0xFF };
	const int bValues[] = { 0x00, 0xFF };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

//colour ranges from white at 0.0 to pink at 1.0
HeatMapColourProvider::Colour PinkColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	
	const int nColours = 2;
	const int rValues[] = { 0xFF, 0xFF };
	const int gValues[] = { 0xFF, 0x00 };
	const int bValues[] = { 0xFF, 0xFF };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour RoygbivColourProvider::getHeatMapColour(double heatValue){
	//0.0-1.0    black-red-orange-yellow-green-cyan-blue-indigo-violet
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	const int nColours = 7;
	const int rValues[] = { 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9F };
	const int gValues[] = { 0x00, 0x66, 0xFF, 0xFF, 0xFF, 0x00, 0x00 };
	const int bValues[] = { 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour RoygbivBWColourProvider::getHeatMapColour(double heatValue){
	//0.0-1.0    black-red-orange-yellow-green-cyan-blue-indigo-violet
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	const int nColours = 9;
	const int rValues[] = { 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9F, 0xFF };
	const int gValues[] = { 0x00, 0x00, 0x66, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF };
	const int bValues[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour LogColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	heatValue = heatValue*heatValue*heatValue;

	const int nColours = 7;
	const int rValues[] = { 0, 0, 0, 0, 255, 255, 255 };
	const int gValues[] = { 0, 0, 255, 255, 255, 0, 255 };
	const int bValues[] = { 0, 255, 255, 0, 0, 0, 255 };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}


HeatMapColourProvider::Colour RoygbivBWLogColourProvider::getHeatMapColour(double heatValue){
	//0.0-1.0    black-red-orange-yellow-green-cyan-blue-indigo-violet-white
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	heatValue = heatValue*heatValue*heatValue;

	const int nColours = 9;
	const int rValues[] = { 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9F, 0xFF };
	const int gValues[] = { 0x00, 0x00, 0x66, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF };
	const int bValues[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour GreenRedColourProvider::getHeatMapColour(double heatValue){
	//0.0-1.0    green-yellow-red
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]

	const int nColours = 3;
	const int rValues[] = { 0x00, 0xFF, 0xFF };
	const int gValues[] = { 0xFF, 0xFF, 0x00 };
	const int bValues[] = { 0x00, 0x00, 0x00 };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour TestColourProvider::getHeatMapColour(double heatValue){
	if (heatValue > 0.5)
		return Colour(255, 255, 255);
	return Colour(0, 0, 0);
}
