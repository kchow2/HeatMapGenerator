#pragma once

class HeatMapColourProvider	//default colour provider
{
public:

	enum COLOUR_INTERPOLATION_MODE{
		CIM_LINEAR = 1,
		CIM_QUADRATIC,
		CIM_CUBIC,
		CIM_SQRT,
		CIM_CUBERT,
		CIM_CLOSEST_MATCH
	};

	struct Colour{
		int r,g,b;
		Colour(): r(0), g(0), b(0) {};
		Colour(int r, int g, int b) : r(r), g(g), b(b) {};
	};
	HeatMapColourProvider(void){ interpolationMode = CIM_LINEAR; }
	virtual ~HeatMapColourProvider(void){}
	
	/*
	Set the colour interpolation mode. Possible values are: {LINEAR,QUADRATIC,CUBIC,SQRT,CUBERT,CLOSEST_MATCH}.
	LINEAR (Default) - The colours are evenly spaced out and the two closest colours to the point are used to do a linear interpolation.
	QUADRATIC - The input is first squared, then linear interpolation is done using this value.
	CUBIC - Input is cubed
	SQRT - Input is square rooted
	CUBERT - Input is cube rooted
	CLOSEST_MATCH - No interpolation is done. The colour closest to the input value is chosen. This creates sharp discontinuities in the output.
	*/
	void setInterpolationMode(COLOUR_INTERPOLATION_MODE mode);
	
	/*
	*	returns the RGB color value for the specified 'heat value'. The heat value can range from 0.0-1.0.
	*	Default colors range from Black-Blue-Green-Yellow-Red linearly as the value increases.
	*	May add ability to change color palette in the future.
	*/
	virtual Colour getHeatMapColour(double heatValue);	


protected:
	/*
	* Linearly interpolates the colour value for the input value from a provided array of colour values.
	* Input value is a value in [0.0,1.0] and an array of colour values to be interpolated from and the size of the colour array.
	* Output is the interpolated value.
	*/
	HeatMapColourProvider::Colour getInterpolatedColourValue(double value, const int *rValues, const int *gValues, const int *bValues, int colourArraySize);

	/*
	* Gets the closest colour to the input value
	*/
	HeatMapColourProvider::Colour getClosestColourMatch(double value, const int *rValues, const int *gValues, const int *bValues, int colourArraySize);

	COLOUR_INTERPOLATION_MODE interpolationMode;
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

class GreenRedColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class BlackBodyColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class IslandParadiseColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class OceanColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class FBColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class GryffindorColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class CedarColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class PokeballColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class FireColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class BlueGoldColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class PinkGreenColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class GoogleColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

class TestColourProvider : public HeatMapColourProvider
{
public:
	virtual HeatMapColourProvider::Colour getHeatMapColour(double heatValue);
};

