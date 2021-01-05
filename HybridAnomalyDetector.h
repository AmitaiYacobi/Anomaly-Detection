

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
/**
 * @brief Another type of anomaly detector which detecs the anomalies by using
 * an algorithm to find minimal enclosing cirlce.
 *
 */
class HybridAnomalyDetector : public SimpleAnomalyDetector {

public:
  HybridAnomalyDetector();
  virtual void learnNormal(const TimeSeries &ts);
  virtual vector<AnomalyReport> detect(const TimeSeries &ts);
  virtual AnomalyReport createReport(correlatedFeatures cf, Point p, int i);
  virtual float getThreshold();
  virtual float setThreshold(float t);
  virtual ~HybridAnomalyDetector();
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
