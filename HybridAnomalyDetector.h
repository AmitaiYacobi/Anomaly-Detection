

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector {
 public:
  HybridAnomalyDetector();
  virtual void learnNormal(const TimeSeries& ts);
  virtual vector<AnomalyReport> detect(const TimeSeries& ts);
  virtual AnomalyReport createReport(correlatedFeatures cf, Point p, int i);
  virtual ~HybridAnomalyDetector();
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
