#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {}

HybridAnomalyDetector::~HybridAnomalyDetector() {}

void HybridAnomalyDetector::learnNormal(const TimeSeries& ts) {
  SimpleAnomalyDetector::learnNormal(ts);
}

AnomalyReport HybridAnomalyDetector::createReport(correlatedFeatures f, Point p,
                                                  int i) {
  if (cf.corrlation >= 0.9) {
    return SimpleAnomalyDetector::createReport(cf, p, i);
  }
  Point** points = cf.points;
  Circle c = findMinCircle(points, cf.size);
  string description;
  string feature1 = cf.feature1;
  string feature2 = cf.feature2;
  float threshold = c.radius * 1.1;
  if (dist(c.center, p) > threshold) {
    description = feature1 + '-' + feature2;
    return AnomalyReport(description, i + 1);
  }
  return AnomalyReport("", -1);
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries& ts) {
  return SimpleAnomalyDetector::detect(ts);
}
