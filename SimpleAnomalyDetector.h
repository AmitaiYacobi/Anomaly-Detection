
// 316418300 Amitai Yacobi

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include <math.h>
#include <string.h>

#include <algorithm>
#include <vector>

#include "AnomalyDetector.h"
#include "anomaly_detection_util.h"

struct correlatedFeatures {
  string feature1, feature2; // names of the correlated features
  Line lin_reg;
  Point **points;
  float corrlation;
  float threshold;
  float size;
};

/**
 * @brief Represents a simple anomaly detector which can test and
 * train a time-series data.
 *
 *
 */
class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
  vector<correlatedFeatures> cf;

public:
  SimpleAnomalyDetector();
  virtual ~SimpleAnomalyDetector();

  virtual void learnNormal(const TimeSeries &ts);
  virtual vector<AnomalyReport> detect(const TimeSeries &ts);
  virtual AnomalyReport createReport(correlatedFeatures cf, Point p, int i);
  vector<correlatedFeatures> getNormalModel() { return this->cf; }
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
