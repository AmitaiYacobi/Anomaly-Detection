
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
  // TODO Auto-generated constructor stub
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
  // TODO Auto-generated destructor stub
}
correlatedFeatures &createCorrelation(string s1, string s2,
                                      const TimeSeries &ts) {

  correlatedFeatures correlated;
  auto mp = ts.getData();
  int position1 = ts.getFeaturePosition(s1);
  int position2 = ts.getFeaturePosition(s2);
  int size;
  float *x;
  float *y;
  Point **points;

  correlated.feature1 = (position1 < position2) ? s1 : s2;
  correlated.feature2 = (position2 > position1) ? s2 : s1;
  x = ts.getFeatureValues(correlated.feature1).data();
  y = ts.getFeatureValues(correlated.feature2).data();
  size = ts.getFeatureValues(correlated.feature2).size();

  for (int i = 0; i < size; ++i) {
    points[i] = new Point(x[i], y[i]);
  }

  // Here needs to be the code for findig the threshold which is the max
  // distance of any point from the points list to the regresion line

  correlated.lin_reg = linear_reg(points, size);
  for (int i = 0; i < size; ++i) {
    delete points[i];
  }

  delete points;
  return correlated;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {

  map<string, vector<float>>::iterator it1;
  map<string, vector<float>>::iterator it2;

  string feature1;
  string feature2;
  Point **p;
  auto mp = ts.getData();
  auto featuresNames = ts.getFeaturesNames();
  auto threshold = 0.9;
  float correlation = 0;
  float *x;
  float *y;
  int position1;
  int position2;
  int size;

  for (it1 = mp.begin(); it1 != mp.end(); it1++) {
    feature1 = it1->first;
    x = it1->second.data();
    for (it2 = it1; it2 != mp.end(); it2++) {
      feature2 = it2->first;
      y = it2->second.data();
      correlation = pearson(x, y, it2->second.size());
      if (correlation >= threshold) {
        createCorrelation(feature1, feature2, ts).corrlation = correlation;
      }
    }
  }
  // TODO Auto-generated destructor stub
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
  // TODO Auto-generated destructor stub
}
