#include "HeatMapColourProvider.h"
#include <algorithm>

HeatMapColourProvider::Colour HeatMapColourProvider::getHeatMapColour(double heatValue){
	//0    blue
	//0.25 cyan
	//0.5  green
	//0.75 yellow
	//1    red
	heatValue = std::max(0.0, std::min(heatValue,1.0));	//clamp intensity to range [0.0, 1.0]
	int colors = 6;
	int heatMapR[] = {0, 0, 0, 0, 255, 255, 255, 255};
	int heatMapG[] = {0, 0, 255, 255, 255, 0, 255, 255};
	int heatMapB[] = {0, 255, 255, 0, 0, 0, 255, 255};

	int scaledIntensity = (int)(heatValue*colors*100); //int from 0-500
	int step = (int)(scaledIntensity / 100);
	double stepIntensity = (scaledIntensity - 100.0*step)/100;

	int r = (int)std::min((heatMapR[step+1]-heatMapR[step])*stepIntensity + heatMapR[step], 255.0);
	int g = (int)std::min((heatMapG[step+1]-heatMapG[step])*stepIntensity + heatMapG[step], 255.0);
	int b = (int)std::min((heatMapB[step+1]-heatMapB[step])*stepIntensity + heatMapB[step], 255.0);
	return Colour(r,g,b);
}

//colour ranges from black at 0.0 to white at 1.0
HeatMapColourProvider::Colour GrayscaleColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	int heatMapR[] = { 0x00, 0xFF };
	int heatMapG[] = { 0x00, 0xFF };
	int heatMapB[] = { 0x00, 0xFF };

	int r = (int)(heatMapR[1] * heatValue + heatMapR[0] * (1.0 - heatValue));
	int g = (int)(heatMapG[1] * heatValue + heatMapG[0] * (1.0 - heatValue));
	int b = (int)(heatMapB[1] * heatValue + heatMapB[0] * (1.0 - heatValue));
	return Colour(r, g, b);
}

//colour ranges from white at 0.0 to pink at 1.0
HeatMapColourProvider::Colour PinkColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	int heatMapR[] = { 0xFF, 0xFF };
	int heatMapG[] = { 0xFF, 0x00 };
	int heatMapB[] = { 0xFF, 0xFF };

	int r = (int)(heatMapR[1] * heatValue + heatMapR[0] * (1.0 - heatValue));
	int g = (int)(heatMapG[1] * heatValue + heatMapG[0] * (1.0 - heatValue));
	int b = (int)(heatMapB[1] * heatValue + heatMapB[0] * (1.0 - heatValue));
	return Colour(r, g, b);
}

HeatMapColourProvider::Colour RoygbivColourProvider::getHeatMapColour(double heatValue){
	//0.0-1.0    black-red-orange-yellow-green-cyan-blue-indigo-violet
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	int colors = 6;
	int heatMapR[] = { 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9F, 0x9F };
	int heatMapG[] = { 0x00, 0x66, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00 };
	int heatMapB[] = { 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF };

	int scaledIntensity = (int)(heatValue*colors * 100);
	int step = (int)(scaledIntensity / 100);
	double stepIntensity = (scaledIntensity - 100.0*step) / 100;

	int r = (int)std::min((heatMapR[step + 1] - heatMapR[step])*stepIntensity + heatMapR[step], 255.0);
	int g = (int)std::min((heatMapG[step + 1] - heatMapG[step])*stepIntensity + heatMapG[step], 255.0);
	int b = (int)std::min((heatMapB[step + 1] - heatMapB[step])*stepIntensity + heatMapB[step], 255.0);
	return Colour(r, g, b);
}

HeatMapColourProvider::Colour RoygbivBWColourProvider::getHeatMapColour(double heatValue){
	//0.0-1.0    black-red-orange-yellow-green-cyan-blue-indigo-violet
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	int colors = 8;
	int heatMapR[] = { 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9F, 0xFF, 0xFF };
	int heatMapG[] = { 0x00, 0x00, 0x66, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF };
	int heatMapB[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	int scaledIntensity = (int)(heatValue*colors * 100);
	int step = (int)(scaledIntensity / 100);
	double stepIntensity = (scaledIntensity - 100.0*step) / 100;

	int r = (int)std::min((heatMapR[step + 1] - heatMapR[step])*stepIntensity + heatMapR[step], 255.0);
	int g = (int)std::min((heatMapG[step + 1] - heatMapG[step])*stepIntensity + heatMapG[step], 255.0);
	int b = (int)std::min((heatMapB[step + 1] - heatMapB[step])*stepIntensity + heatMapB[step], 255.0);
	return Colour(r, g, b);
}

HeatMapColourProvider::Colour LogColourProvider::getHeatMapColour(double heatValue){
	//0    blue
	//0.25 cyan
	//0.5  green
	//0.75 yellow
	//1    red
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	heatValue = heatValue*heatValue*heatValue;

	int colors = 6;
	int heatMapR[] = { 0, 0, 0, 0, 255, 255, 255, 255 };
	int heatMapG[] = { 0, 0, 255, 255, 255, 0, 255, 255 };
	int heatMapB[] = { 0, 255, 255, 0, 0, 0, 255, 255 };

	int scaledIntensity = (int)(heatValue*colors * 100); //int from 0-500
	int step = (int)(scaledIntensity / 100);
	double stepIntensity = (scaledIntensity - 100.0*step) / 100;

	int r = (int)std::min((heatMapR[step + 1] - heatMapR[step])*stepIntensity + heatMapR[step], 255.0);
	int g = (int)std::min((heatMapG[step + 1] - heatMapG[step])*stepIntensity + heatMapG[step], 255.0);
	int b = (int)std::min((heatMapB[step + 1] - heatMapB[step])*stepIntensity + heatMapB[step], 255.0);
	return Colour(r, g, b);
}


HeatMapColourProvider::Colour RoygbivBWLogColourProvider::getHeatMapColour(double heatValue){
	//0.0-1.0    black-red-orange-yellow-green-cyan-blue-indigo-violet
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	heatValue = heatValue*heatValue*heatValue;
	//heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]

	int colors = 8;
	int heatMapR[] = { 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9F, 0xFF, 0xFF };
	int heatMapG[] = { 0x00, 0x00, 0x66, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF };
	int heatMapB[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	int scaledIntensity = (int)(heatValue*colors * 100);
	int step = (int)(scaledIntensity / 100);
	double stepIntensity = (scaledIntensity - 100.0*step) / 100;

	int r = (int)std::min((heatMapR[step + 1] - heatMapR[step])*stepIntensity + heatMapR[step], 255.0);
	int g = (int)std::min((heatMapG[step + 1] - heatMapG[step])*stepIntensity + heatMapG[step], 255.0);
	int b = (int)std::min((heatMapB[step + 1] - heatMapB[step])*stepIntensity + heatMapB[step], 255.0);
	return Colour(r, g, b);
}

HeatMapColourProvider::Colour TestColourProvider::getHeatMapColour(double heatValue){
	//0.0-1.0    black-red-orange-yellow-green-cyan-blue-indigo-violet
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	heatValue = heatValue*heatValue*heatValue;
	//heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]

	int colors = 8;
	int heatMapR[] = { 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF };
	int heatMapG[] = { 0x00, 0x00, 0x66, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF };
	int heatMapB[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	int scaledIntensity = (int)(heatValue*colors * 100);
	int step = (int)(scaledIntensity / 100);
	double stepIntensity = (scaledIntensity - 100.0*step) / 100;

	int r = (int)std::min((heatMapR[step + 1] - heatMapR[step])*stepIntensity + heatMapR[step], 255.0);
	int g = (int)std::min((heatMapG[step + 1] - heatMapG[step])*stepIntensity + heatMapG[step], 255.0);
	int b = (int)std::min((heatMapB[step + 1] - heatMapB[step])*stepIntensity + heatMapB[step], 255.0);
	return Colour(r, g, b);
}
