#include <stdlib.h>
#include <math.h>

double HeatMapFunc_SinXY(double x, double y){
	return sin(x*y) / 2.0 + 0.5;
}

double HeatMapFunc_CosXY(double x, double y){
	return cos(x*y) / 2.0 + 0.5;
}

double HeatMapFunc_E_POW_XY(double x, double y){
	const double e = 2.7178;
	return pow(e, fabs(x*y / 10.0) * -1.0);
}

double HeatMapFunc_XY(double x, double y){
	return fabs(x*y) / 10.0;
}

double HeatMapFunc_1_OVER_XY(double x, double y){
	return 1.0 / fabs(x*y);
}

double HeatMapFunc_WEIRD_1(double x, double y){
	return sin(x + y) - cos(x*y)*sin(x);
}
double HeatMapFunc_1_OVER_SIN_COS(double x, double y){
	return (1.0 / cos(x)*sin(y)) / 5.0 + 0.5;
}
double HeatMapFunc_SIN_COS(double x, double y){
	return cos(x)*sin(y) / 2.0 + 0.5;
}
double HeatMapFunc_COS_COS_SIN(double x, double y){
	return cos(x)*cos(x)*sin(y) / 2.0 + 0.5;
}
double HeatMapFunc_COS_COS_MINUS_SIN(double x, double y){
	return (cos(x)*cos(x) - sin(y)) / 2.0;
}
double HeatMapFunc_GRADIENT(double x, double y){
	return x / 20.0 + 0.5;
}
double HeatMapFunc_DONUT(double x, double y){
	double d = 5.0 - sqrt(x*x + y*y);
	return 1.0 - (d*d - 0.05)*0.025;
}
double HeatMapFunc_X_PLUS_SINXY(double x, double y){
	return fabs(x + sin(x*y)) / 4.0;
}
double HeatMapFunc_X2_Y3_COSXY(double x, double y){
	return fabs(x*x + y*y*y - x*y + cos(x*y)) / 40.0;
}
double HeatMapFunc_X2_Y2_E_X2_Y2(double x, double y){
	const double e = 2.7178;
	x = x / 8.0;
	y = y / 8.0;
	return 2.0*(x*x - y*y) * pow(e, -1.0*(x*x + y*y)) + 0.5;
}

double HeatMapFunc_SIN_SQRT_X2_Y2(double x, double y){
	return sin(sqrt(x*x + y*y)) / 2 + 0.5;
}
double HeatMapFunc_SIN_1_OVER_XY(double x, double y){
	if (fabs(x*y) < 0.0000001)
		return 0.5;
	return sin(1 / (x*y)) / 2 + 0.5;
}
double HeatMapFunc_RAND(double x, double y){
	int r = rand();
	return (double)r / RAND_MAX;
}
double HeatMapFunc_SIN__SIN_COS(double x, double y){
	return sin(sin(x) + cos(y)) / 2 + 0.5;
}
double HeatMapFunc_SIN_X2_Y2(double x, double y){
	return fabs(sin(x*x - y*y));
}
double HeatMapFunc_X_PLUS_Y(double x, double y){
	return (fabs(x) + abs(y)) / 20.0;
}

double HeatMapFunc_HEART1(double x, double y){
	double r = sqrt(x*x + y*y);
	//double theta = atan2(y, x);
	double theta_inv = atan2(x, -(y / 2));

	return theta_inv / r + 0.5;
}
double HeatMapFunc_HEART2(double x, double y){
	double r = sqrt(x*x + y*y);
	//double theta = atan2(y, x);
	double theta_inv = atan2(x, -(y / 2));

	return fabs(theta_inv / r);
}
double HeatMapFunc_CHECKERBOARD(double x, double y){
	//checkerboard
	double d1, d2;
	if(x >= 0.0)
		d1 = fabs(fmod(x, 2.0)) > 1.0 ? 1.0 : 0.0;
	else
		d1 = fabs(fmod(x, 2.0)) < 1.0 ? 1.0 : 0.0;
	if(y >= 0.0)
		d2 = fabs(fmod(y, 2.0)) < 1.0 ? 1.0 : 0.0;	
	else
		d2 = fabs(fmod(y, 2.0)) > 1.0 ? 1.0 : 0.0;

	return fmod(d1 + d2, 2.0) > 0.5 ? 1.0 : 0.0;
}

double HeatMapFunc_SPIRAL1(double x, double y){
	double r = sqrt(x*x + y*y);

	double theta = fmod(atan2(y, x) + 3.141, 6.282);
	//double N = floor(r/theta);
	//theta *= N*6.282;
	return fmod(r / theta, 1.0);
}
double HeatMapFunc_SPIRAL2(double x, double y){
	double r = sqrt(x*x + y*y);

	double theta = fmod(atan2(y, x) + 3.141, 6.282);
	return r / theta;
}
double HeatMapFunc_MANDELBROT(double x, double y){
	//mandelbrot fractal algorithm
	double x_ = 0.0;
	double y_ = 0.0;
	int iteration = 0;
	int maxIterations = 1000;
	while (x_*x_ + y_*y_ < 2 * 2 && iteration < maxIterations){
		double xtemp = x_*x_ - y_*y_ + x;
		double ytemp = 2 * x_*y_ + y;

		if (x_ == xtemp && y_ == ytemp){	//This is an optimization to stop the calculation if x,y are the same as the prev iteration. We know it converges, so we can stop the calculation.
			iteration = maxIterations;
			break;
		}
		x_ = xtemp;
		y_ = ytemp;
		iteration = iteration + 1;
	}

	return (double)iteration / maxIterations;
}

double HeatMapFunc_F1(double x, double y){
	double r = sqrt(x*x + y*y);

	double theta = fmod(atan2(y, x) + 3.141, 6.282);
	//double N = floor(r/theta);
	//theta *= N*6.282;
	return fmod(r / theta, 1.0);
}
double HeatMapFunc_F2(double x, double y){
	double r = sqrt(x*x + y*y);

	double theta = fmod(atan2(y, x) + 3.141, 6.282);
	return r / theta;
}
double HeatMapFunc_F3(double x, double y){
	//mandelbrot fractal algorithm
	double x_ = 0.0;
	double y_ = 0.0;
	int iteration = 0;
	int maxIterations = 1000;
	while (x_*x_ + y_*y_ < 2 * 2 && iteration < maxIterations){
		double xtemp = x_*x_ - y_*y_ + x;
		y_ = 2 * x_*y_ + y;
		x_ = xtemp;
		iteration = iteration + 1;
	}

	return (double)iteration / maxIterations;
}

double HeatMapFunc_ZERO(double x, double y){
	return 0.0;
}
double HeatMapFunc_ONE(double x, double y){
	return 1.0;
}
