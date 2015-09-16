#include "HeatMapColourProvider.h"
#include <math.h>
#include <algorithm>

void HeatMapColourProvider::setInterpolationMode(COLOUR_INTERPOLATION_MODE mode){
	this->interpolationMode = mode;
}

//helper func to interpolate values linearly from a colour array
HeatMapColourProvider::Colour HeatMapColourProvider::getInterpolatedColourValue(double value, const int *rValues, const int *gValues, const int *bValues, int colourArraySize){
	if (this->interpolationMode == CIM_LINEAR){
		//value = value;
	}
	else if (this->interpolationMode == CIM_QUADRATIC){
		value = value*value;
	}
	else if (this->interpolationMode == CIM_CUBIC){
		value = value*value*value;
	}
	else if (this->interpolationMode == CIM_SQRT){
		value = sqrt(value);
	}
	else if (this->interpolationMode == CIM_CUBERT){
		value = pow(value, 1.0 / 3.0);
	}
	else if (this->interpolationMode == CIM_CLOSEST_MATCH){
		return getClosestColourMatch(value, rValues, gValues, bValues, colourArraySize);
	}
		
	
	int scaledIntensity = (int)(value*(colourArraySize - 1) * 100);
	int step = (int)(scaledIntensity / 100);	//the 'step' is which two values in the array we should be interpolating between
	double stepDistance = (scaledIntensity - 100.0*step) / 100;	//the distance the value is from the step

	//return the linearly interpolated value between colourArray[step] and colourArray[step+1]
	int r = (int)((rValues[step + 1] - rValues[step])*stepDistance + rValues[step]);
	int g = (int)((gValues[step + 1] - gValues[step])*stepDistance + gValues[step]);
	int b = (int)((bValues[step + 1] - bValues[step])*stepDistance + bValues[step]);
	return Colour(r, g, b);
}

//helper func to get closest colour match for given heat value
HeatMapColourProvider::Colour HeatMapColourProvider::getClosestColourMatch(double value, const int *rValues, const int *gValues, const int *bValues, int colourArraySize){
	int scaledIntensity = (int)(value*(colourArraySize - 1) * 100);
	int step = (int)(scaledIntensity / 100);	//the 'step' is which two values in the array we should be interpolating between
	double stepDistance = (scaledIntensity - 100.0*step) / 100;	//the distance the value is from the step

	//return the closest colour to the point
	int r, g, b;
	if (stepDistance < 0.5){
		r = rValues[step];
		g = gValues[step];
		b = bValues[step];
	}
	else{
		r = rValues[step+1];
		g = gValues[step+1];
		b = bValues[step+1];
	}

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


HeatMapColourProvider::Colour GreenRedColourProvider::getHeatMapColour(double heatValue){
	//0.0-1.0    green-yellow-red
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]

	const int nColours = 3;
	const int rValues[] = { 0x00, 0xFF, 0xFF };
	const int gValues[] = { 0xFF, 0xFF, 0x00 };
	const int bValues[] = { 0x00, 0x00, 0x00 };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

/* Taken from http://www.vendian.org/mncharity/dir3/blackbody/
Temperature    x      y      z       R     G     B
----------- ------ ------ ------   ----- ----- -----
1000 K      0.6528 0.3444 0.0028   1.000 0.007 0.000 (Approximation)
1500 K      0.5857 0.3931 0.0212   1.000 0.126 0.000 (Approximation)
2000 K      0.5267 0.4133 0.0600   1.000 0.234 0.010
2500 K      0.4770 0.4137 0.1093   1.000 0.349 0.067
3000 K      0.4369 0.4041 0.1590   1.000 0.454 0.151
3500 K      0.4053 0.3907 0.2040   1.000 0.549 0.254
4000 K      0.3805 0.3768 0.2428   1.000 0.635 0.370
4500 K      0.3608 0.3636 0.2756   1.000 0.710 0.493
5000 K      0.3451 0.3516 0.3032   1.000 0.778 0.620
5500 K      0.3325 0.3411 0.3265   1.000 0.837 0.746
6000 K      0.3221 0.3318 0.3461   1.000 0.890 0.869
6500 K      0.3135 0.3237 0.3628   1.000 0.937 0.988
7000 K      0.3064 0.3166 0.3770   0.907 0.888 1.000
7500 K      0.3004 0.3103 0.3893   0.827 0.839 1.000
8000 K      0.2952 0.3048 0.4000   0.762 0.800 1.000
8500 K      0.2908 0.3000 0.4093   0.711 0.766 1.000
9000 K      0.2869 0.2956 0.4174   0.668 0.738 1.000
9500 K      0.2836 0.2918 0.4246   0.632 0.714 1.000
10000 K     0.2807 0.2884 0.4310   0.602 0.693 1.000
*/
HeatMapColourProvider::Colour BlackBodyColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]

	const int nColours = 19;
	const int rValues[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xD2, 0xC2, 0xB5, 0xAA, 0xA1, 0x99 };
	const int gValues[] = { 0x01, 0x20, 0x3B, 0x58, 0x73, 0x8B, 0xA1, 0xB5, 0xC6, 0xD5, 0xE2, 0xEE, 0xE2, 0xD5, 0xCC, 0xC3, 0xBC, 0xB6, 0xB0 };
	const int bValues[] = { 0x00, 0x00, 0x02, 0x11, 0x26, 0x40, 0x5E, 0x7D, 0x9E, 0xBE, 0xDD, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour IslandParadiseColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	//Obtained from http://www.colourlovers.com/palette/3842439/
	//23BAD7
	//0C73DC
	//89D537
	//F8D68E
	//D3F0FF
	const int nColours = 5;
	const int rValues[] = { 0x23, 0x0C, 0x89, 0xF8, 0xD3 };
	const int gValues[] = { 0xBA, 0x73, 0xD5, 0xD6, 0xF0 };
	const int bValues[] = { 0xD7, 0xDC, 0x37, 0x8E, 0xFF };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour OceanColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	//131C45
	//618548E
	//B1F0D2
	//619E79
	//003F34
	const int nColours = 5;
	const int rValues[] = { 0x13, 0x61, 0xB1, 0x61, 0x00 };
	const int gValues[] = { 0x1C, 0x85, 0xF0, 0x9E, 0x3F };
	const int bValues[] = { 0x45, 0x8E, 0xD2, 0x79, 0x34 };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour FBColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	//Obtained from http://www.colourlovers.com
	//3b5998
	//8b9dc3
	//dfe3ee
	//f7f7f7
	//ffffff
	const int nColours = 5;
	const int rValues[] = { 0x3B, 0x8B, 0xDF, 0xF7, 0xFF };
	const int gValues[] = { 0x59, 0x9D, 0xE3, 0xF7, 0xFF };
	const int bValues[] = { 0x98, 0xC3, 0xEE, 0xF7, 0xFF };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour GryffindorColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	//Obtained from http://www.colourlovers.com
	//740000
	//ae0000
	//eeba30
	//d3a625
	//000000
	const int nColours = 5;
	const int rValues[] = { 0x74, 0xAE, 0xEE, 0xD3, 0x00 };
	const int gValues[] = { 0x00, 0x00, 0xBA, 0xA6, 0x00 };
	const int bValues[] = { 0x00, 0x00, 0x30, 0x25, 0x00 };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour CedarColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	//cc1614
	//e5ceb0
	//2a334f
	//6b4423
	//ac8f57
	const int nColours = 5;
	const int rValues[] = { 0xCC, 0xE5, 0x2A, 0x6B, 0xAC };
	const int gValues[] = { 0x16, 0xCE, 0x33, 0x44, 0x8F };
	const int bValues[] = { 0x14, 0xB0, 0x4F, 0x23, 0x57 };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour PokeballColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	//ee1515
	//222224
	//f0f0f0
	const int nColours = 3;
	const int rValues[] = { 0xEE, 0x22, 0xF0 };
	const int gValues[] = { 0x15, 0x22, 0xF0 };
	const int bValues[] = { 0x15, 0x24, 0xF0 };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour FireColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	//fdcf58
	//757676
	//f27d0c
	//800909
	//000000
	const int nColours = 5;
	const int rValues[] = { 0xFD, 0x75, 0xF2, 0x80, 0x00 };
	const int gValues[] = { 0xCF, 0x76, 0x7D, 0x09, 0x00 };
	const int bValues[] = { 0x58, 0x76, 0x0C, 0x09, 0x00 };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour BlueGoldColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	//B28200
	//FFC119
	//FFBA00
	//0029B2
	//003BFF
	const int nColours = 5;
	const int rValues[] = { 0xB2, 0xFF, 0xFF, 0x00, 0x00 };
	const int gValues[] = { 0x82, 0xC1, 0xBA, 0x29, 0x3B };
	const int bValues[] = { 0x00, 0x19, 0x00, 0xB2, 0xFF };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour PinkGreenColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	const int nColours = 4;
	//E90084
	//FF0000
	//00E900
	//AAFF00
	const int rValues[] = { 0xE9, 0xFF, 0x00, 0xAA };
	const int gValues[] = { 0x00, 0x00, 0xE9, 0xFF };
	const int bValues[] = { 0x84, 0x00, 0x00, 0x00 };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour GoogleColourProvider::getHeatMapColour(double heatValue){
	//blue-green-yellow-red
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	const int nColours = 4;
	const int rValues[] = { 0x00, 0x00, 0xFF, 0xFF };
	const int gValues[] = { 0x00, 0xFF, 0xFF, 0x00 };
	const int bValues[] = { 0xFF, 0x00, 0x00, 0x00 };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}

HeatMapColourProvider::Colour TestColourProvider::getHeatMapColour(double heatValue){
	heatValue = std::max(0.0, std::min(heatValue, 1.0));	//clamp intensity to range [0.0, 1.0]
	const int nColours = 4;
	//E90084
	//FF0000
	//00E900
	//AAFF00
	const int rValues[] = { 0xE9, 0xFF, 0x00, 0xAA };
	const int gValues[] = { 0x00, 0x00, 0xE9, 0xFF };
	const int bValues[] = { 0x84, 0x00, 0x00, 0x00 };

	return this->getInterpolatedColourValue(heatValue, rValues, gValues, bValues, nColours);
}
