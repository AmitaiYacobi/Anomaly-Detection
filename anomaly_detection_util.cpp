#include <iostream>

/*
 * anomaly_detection_util.cpp
 *
 * Author: write your ID and name here
 */
#include "AnomalyDetector.h"
#include "anomaly_detection_util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float avg(float *x, int size) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += x[i];
  }
  return sum / size;
}

// returns the variance of X and Y
float var(float *x, int size) {
  float sum = 0;
  float eX = avg(x, size);
  for (int i = 0; i < size; i++) {
    sum += pow(x[i] - eX, 2);
  }
  float varX = sum / size;
  return varX;
}

// returns the covariance of X and Y
float cov(float *x, float *y, int size) {

  float *xy = new float[size];
  for (int i = 0; i < size; i++) {
    xy[i] = x[i] * y[i];
  }
  float eXY = avg(xy, size);
  float eX = avg(x, size);
  float eY = avg(y, size);
  float covXY = eXY - (eX * eY);

  return covXY;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size) {

  float standardDeviation = sqrt(var(x, size) * var(y, size));

  return cov(x, y, size) / standardDeviation;
}

// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size) {
  float *x = new float[size];
  float *y = new float[size];

  for (int i = 0; i < size; i++) {
    x[i] = points[i]->x;
    y[i] = points[i]->y;
  }
  float a = cov(x, y, size) / var(x, size);
  float b = avg(y, size) - a * avg(x, size);

  delete x;
  delete y;

  return Line(a, b);
}

Line linear_reg(float *x, float *y, int size) {
  float a = cov(x, y, size) / var(x, size);
  float b = avg(y, size) - a * avg(x, size);

  return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size) {
  Line line = linear_reg(points, size);
  float fx = line.f(p.x);
  float y = p.y;

  return fabs(fx - y);
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
  float fx = l.f(p.x);
  float y = p.y;

  return fabs(fx - y);
}
