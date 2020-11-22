
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

correlatedFeatures createCorrelation(string s1, string s2,
                                     const TimeSeries &ts) {

  correlatedFeatures correlated;
  auto mp = ts.getData();
  int position1 = ts.getFeaturePosition(s1);
  int position2 = ts.getFeaturePosition(s2);
  int size;
  float max = 0;
  float *x;
  float *y;

  correlated.feature1 = (position1 < position2) ? s1 : s2;
  correlated.feature2 = (position2 > position1) ? s2 : s1;
  vector<float> v1 = ts.getFeatureValues(correlated.feature1);
  vector<float> v2 = ts.getFeatureValues(correlated.feature2);
  x = v1.data();
  y = v2.data();
  size = v2.size();

  Point **points = new Point *[size];

  for (int i = 0; i < size; ++i) {
    points[i] = new Point(x[i], y[i]);
  }

  correlated.lin_reg = linear_reg(points, size);

  for (int i = 0; i < size; ++i) {
    if (dev(*points[i], correlated.lin_reg) >= max)
      max = dev(*points[i], correlated.lin_reg);
  }

  max = max * 1.1;
  correlated.threshold = max;

  for (int i = 0; i < size; ++i) {
    delete points[i];
  }

  delete points;
  return correlated;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {

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
  float *x;
  float *y;

  for (it1 = mp.begin(); it1 != mp.end(); it1++) {
    flag = false;
    threshold = 0.9;
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

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {

  vector<AnomalyReport> ar;
  vector<correlatedFeatures>::iterator vecIt;
  string feature1;
  string feature2;
  string description;
  auto mp = ts.getData();
  auto cf = this->cf;
  auto size = ts.getFeatureValues(cf[0].feature1).size();
  float d;
  float threshold;

  for (int i = 0; i < size; ++i) {
    for (vecIt = cf.begin(); vecIt != cf.end(); vecIt++) {
      Point p(mp[vecIt->feature1][i], mp[vecIt->feature2][i]);
      Line line = vecIt->lin_reg;
      feature1 = vecIt->feature1;
      feature2 = vecIt->feature2;
      threshold = vecIt->threshold;
      d = dev(p, line);
      if (d > threshold) {
        description = feature1 + '-' + feature2;
        ar.push_back(AnomalyReport(description, i + 1));
      }
    }
  }
  return ar;
}
