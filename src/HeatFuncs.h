#pragma once

typedef double(*HeatMapFunc)(double x, double y);

double HeatMapFunc_SinXY(double x, double y);
double HeatMapFunc_CosXY(double x, double y);
double HeatMapFunc_E_POW_XY(double x, double y);
double HeatMapFunc_XY(double x, double y);
double HeatMapFunc_1_OVER_XY(double x, double y);
double HeatMapFunc_WEIRD_1(double x, double y);
double HeatMapFunc_1_OVER_SIN_COS(double x, double y);
double HeatMapFunc_SIN_COS(double x, double y);
double HeatMapFunc_COS_COS_SIN(double x, double y);
double HeatMapFunc_COS_COS_MINUS_SIN(double x, double y);
double HeatMapFunc_GRADIENT(double x, double y);
double HeatMapFunc_DONUT(double x, double y);
double HeatMapFunc_X_PLUS_SINXY(double x, double y);
double HeatMapFunc_X2_Y3_COSXY(double x, double y);
double HeatMapFunc_X2_Y2_E_X2_Y2(double x, double y);
double HeatMapFunc_SIN_SQRT_X2_Y2(double x, double y);
double HeatMapFunc_SIN_1_OVER_XY(double x, double y);
double HeatMapFunc_RAND(double x, double y);
double HeatMapFunc_SIN__SIN_COS(double x, double y);
double HeatMapFunc_SIN_X2_Y2(double x, double y);
double HeatMapFunc_X_PLUS_Y(double x, double y);
double HeatMapFunc_HEART1(double x, double y);
double HeatMapFunc_HEART2(double x, double y);
double HeatMapFunc_CHECKERBOARD(double x, double y);
double HeatMapFunc_SPIRAL1(double x, double y);
double HeatMapFunc_SPIRAL2(double x, double y);
double HeatMapFunc_MANDELBROT(double x, double y);
double HeatMapFunc_F1(double x, double y);
double HeatMapFunc_F2(double x, double y);
double HeatMapFunc_F3(double x, double y);
double HeatMapFunc_ZERO(double x, double y);
double HeatMapFunc_ONE(double x, double y);