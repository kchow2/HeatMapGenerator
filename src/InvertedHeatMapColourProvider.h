#pragma once
#include "HeatMapColourProvider.h"

class InvertedHeatMapColourProvider: public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

