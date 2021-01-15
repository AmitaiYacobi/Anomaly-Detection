#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {}

HybridAnomalyDetector::~HybridAnomalyDetector() {}
/**
 * @brief Creates anomaly report according to the correlation. If the
 * correlation is greater then 0.9, the 'super' createReport method is called.
 * Otherwise, it gets overridden.
 *
 * @param f
 * @param p
 * @param i
 * @return AnomalyReport
 */
AnomalyReport HybridAnomalyDetector::createReport(correlatedFeatures f, Point p,
                                                  int i) {

  Point **points = f.points;
  Circle c = findMinCircle(points, f.size);
  string description;
  string feature1 = f.feature1;
  string feature2 = f.feature2;
  // the distance threshold- the maximum valid length from the center
  float disThreshold = c.radius * 1.1;
  if (f.corrlation >= getThreshold()) {
    return SimpleAnomalyDetector::createReport(f, p, i);
  }
  if (dist(c.center, p) > disThreshold) {
    description = feature1 + '-' + feature2;
    return AnomalyReport(description, i + 1);
  }
  return AnomalyReport("", -1);
}
float HybridAnomalyDetector::getThreshold() {
  return SimpleAnomalyDetector::getThreshold();
}
float HybridAnomalyDetector::setThreshold(float t) {
  return SimpleAnomalyDetector::setThreshold(t);
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
  SimpleAnomalyDetector::learnNormal(ts);
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
  return SimpleAnomalyDetector::detect(ts);
}
