
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <math.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

#include <iostream>
#include <vector>

#include "anomaly_detection_util.h"

using namespace std;

// ------------ DO NOT CHANGE -----------

class Circle {
public:
  Point center;
  float radius;
  Circle(Point c, float r) : center(c), radius(r) {}
};
// --------------------------------------

float dist(Point a, Point b);

Circle from2points(Point a, Point b);

Point circumcenter(Point b, Point c);

Circle from3Points(Point a, Point b, Point c);

Circle trivial(vector<Point> &P);

Circle welzl(Point **P, vector<Point> R, size_t n);
Circle findMinCircle(Point **points, size_t size);
Circle findMinCircle(float *x, float *y, int size);

#endif /* MINCIRCLE_H_ */
