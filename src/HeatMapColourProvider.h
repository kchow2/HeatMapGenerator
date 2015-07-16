#pragma once

class HeatMapColourProvider	//default colour provider
{
public:
	struct Colour{
		int r,g,b;
		Colour(): r(0), g(0), b(0) {};
		Colour(int r, int g, int b) : r(r), g(g), b(b) {};
	};
	HeatMapColourProvider(void){}
	virtual ~HeatMapColourProvider(void){}
	
	/*
	*	returns the RGB color value for the specified 'heat value'. The heat value can range from 0.0-1.0.
	*	Default colors range from Black-Blue-Green-Yellow-Red linearly as the value increases.
	*	May add ability to change color palette in the future.
	*/
	virtual Colour getHeatMapColour(double heatValue);	
};

class GrayscaleColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class PinkColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class RoygbivColourProvider : public HeatMapColourProvider 
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class RoygbivBWColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class LogColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class RoygbivBWLogColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class TestColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

