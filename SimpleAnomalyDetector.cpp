// 316418300 Amitai Yacobi

#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
  vector<correlatedFeatures>::iterator it;
  for (it = this->cf.begin(); it != this->cf.end(); it++) {
    for (int i = 0; i < it->size; ++i) {
      delete it->points[i];
    }
    delete it->points;
  }
}

/**
 * @brief Create a Correlation struct that defined in "SimpleAnomalyDetector.h".
 * This struct contains a pair of features which are correlated with each other.
 *
 * @param s1 feature name
 * @param s2 feature name
 * @param ts the data
 * @return correlatedFeatures
 */
correlatedFeatures createCorrelation(string s1, string s2,
                                     const TimeSeries& ts) {
  correlatedFeatures correlated;
  auto mp = ts.getData();
  int position1 = ts.getFeaturePosition(s1);
  int position2 = ts.getFeaturePosition(s2);
  int size;
  float max = 0;
  float* x;
  float* y;

  correlated.feature1 = (position1 < position2) ? s1 : s2;
  correlated.feature2 = (position2 > position1) ? s2 : s1;
  vector<float> v1 = ts.getFeatureValues(correlated.feature1);
  vector<float> v2 = ts.getFeatureValues(correlated.feature2);
  x = v1.data();
  y = v2.data();
  size = v2.size();
  Point** points = new Point*[size];

  for (int i = 0; i < size; ++i) {
    points[i] = new Point(x[i], y[i]);
  }

  correlated.lin_reg = linear_reg(x, y, size);

  for (int i = 0; i < size; ++i) {
    if (dev(*points[i], correlated.lin_reg) >= max)
      max = dev(*points[i], correlated.lin_reg);
  }

  max = max * 1.1;
  correlated.threshold = max;
  correlated.points = points;
  correlated.size = size;
  return correlated;
}

/**
 * @brief Learns a normal data and defines linear regression of every pair of
 * features which have the biggest correlation between each other.
 *
 * @param ts given data.
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
  map<string, vector<float>>::iterator it1;
  map<string, vector<float>>::iterator it2;
  vector<correlatedFeatures> cf;

  correlatedFeatures c;
  string feature1;
  string feature2;
  string temp;
  auto mp = ts.getData();
  auto featuresNames = ts.getFeaturesNames();
  bool flag;
  float threshold;
  float correlation = 0;
  float* x;
  float* y;

  for (it1 = mp.begin(); it1 != mp.end(); it1++) {
    flag = false;
    threshold = 0.5;
    feature1 = it1->first;
    x = it1->second.data();
    for (it2 = ++it1; it2 != mp.end(); it2++) {
      feature2 = it2->first;
      y = it2->second.data();
      correlation = pearson(x, y, it2->second.size());
      if (fabs(correlation) >= threshold) {
        threshold = correlation;
        temp = feature2;
        flag = true;
      }
    }
    if (flag) {
      c = createCorrelation(feature1, temp, ts);
      c.corrlation = threshold;
      cf.push_back(c);
    }
    --it1;
  }
  this->cf = cf;
}
/**
 * @brief
 *
 * @param cf
 * @param p
 * @param i
 * @return AnomalyReport
 */
AnomalyReport SimpleAnomalyDetector::createReport(correlatedFeatures f, Point p,
                                                  int i) {
  Line line = f.lin_reg;
  string description;
  string feature1 = f.feature1;
  string feature2 = f.feature2;
  float threshold = f.threshold;
  if (dev(p, line) > threshold) {
    description = feature1 + '-' + feature2;
    return AnomalyReport(description, i + 1);
  }
  return AnomalyReport("", -1);
}
/**
 * @brief Detect anomalies in the data and reports on them.
 *
 * @param ts given data.
 * @return vector<AnomalyReport> of the anomalies reports.
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
  vector<AnomalyReport> ar;
  vector<correlatedFeatures>::iterator vecIt;
  auto mp = ts.getData();
  auto cf = this->cf;
  auto size = cf[0].size;
  float d;
  float threshold;

  for (int i = 0; i < size; ++i) {
    for (vecIt = cf.begin(); vecIt != cf.end(); vecIt++) {
      Point p(mp[vecIt->feature1][i], mp[vecIt->feature2][i]);
      AnomalyReport r = createReport(*vecIt, p, i);
      if (r.timeStep == -1) continue;
      ar.push_back(r);
    }
  }
  return ar;
}
