#include "InvertedHeatMapColourProvider.h"

HeatMapColourProvider::Colour InvertedHeatMapColourProvider::getHeatMapColour(double heatValue){
	return HeatMapColourProvider::getHeatMapColour(1.0-heatValue);
}
