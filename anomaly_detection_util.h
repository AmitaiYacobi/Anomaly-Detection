
#ifndef ANOMALYDETECTORUTIL_H_
#define ANOMALYDETECTORUTIL_H_

float avg(float *x, int size);

float var(float *x, int size);

float cov(float *x, float *y, int size);

float pearson(float *x, float *y, int size);

class Line {
public:
  float a, b;
  Line() : a(0), b(0){};
  Line(float a, float b) : a(a), b(b) {}
  float f(float x) { return a * x + b; }
};

class Point {
public:
  const float x, y;
  Point(float x, float y) : x(x), y(y) {}
};

Line linear_reg(Point **points, int size);

Line linear_reg(float *x, float *y, int size);

float dev(Point p, Point **points, int size);

float dev(Point p, Line l);

#endif
