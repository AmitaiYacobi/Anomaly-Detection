#include <iostream>

/*
 * anomaly_detection_util.cpp
 *
 * Author: write your ID and name here
 */

#include "anomaly_detection_util.h"
#include <math.h>

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
  float xy[size];
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
  float x[size];
  float y[size];

  for (int i = 0; i < size; i++) {
    x[i] = points[i]->getX();
    y[i] = points[i]->getY();
  }
  float a = cov(x, y, size) / var(x, size);
  float b = avg(y, size) - a * avg(x, size);

  return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size) {
  Line line = linear_reg(points, size);
  float fx = line.f(p.getX());
  float y = p.getY();

  return abs(fx - y);
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
  float fx = l.f(p.getX());
  float y = p.getY();

  return abs(fx - y);
}
