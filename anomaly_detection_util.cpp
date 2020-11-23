#include <iostream>

/*
 * anomaly_detection_util.cpp
 *
 * Author: Amitai Yacobi 316418300
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

/**
 * @brief Returns the variance of X and Y
 *
 * @param x
 * @param size
 * @return float
 */
float var(float *x, int size) {
  float sum = 0;
  float eX = avg(x, size);
  for (int i = 0; i < size; i++) {
    sum += pow(x[i] - eX, 2);
  }
  float varX = sum / size;
  return varX;
}

/**
 * @brief Returns the covariance of X and Y
 *
 * @param x
 * @param y
 * @param size
 * @return float
 */
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

/**
 * @brief Returns the Pearson correlation coefficient of X and Y
 *
 * @param x
 * @param y
 * @param size
 * @return float
 */
float pearson(float *x, float *y, int size) {

  float standardDeviation = sqrt(var(x, size) * var(y, size));

  return cov(x, y, size) / standardDeviation;
}

/**
 * @brief Performs a linear regression and returns the line equation
 *
 * @param points
 * @param size
 * @return Line
 */
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

/**
 * @brief Returns the deviation between point p and the line equation of the
 * points
 *
 * @param p
 * @param points
 * @param size
 * @return float
 */
float dev(Point p, Point **points, int size) {
  Line line = linear_reg(points, size);
  float fx = line.f(p.x);
  float y = p.y;

  return fabs(fx - y);
}

/**
 * @brief Returns the deviation between point p and the line
 *
 * @param p
 * @param l
 * @return float
 */
float dev(Point p, Line l) {
  float fx = l.f(p.x);
  float y = p.y;

  return fabs(fx - y);
}
