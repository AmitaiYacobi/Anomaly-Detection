#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries {
  const char *fileName;
  map<string, vector<float>> ts;
  string *featuresNames;

public:
  TimeSeries(const char *CSVfileName) {
    this->fileName = CSVfileName;
    this->ts = readCsv();
  }
  ~TimeSeries() { delete fileName; }

  virtual map<string, vector<float>> readCsv();
  virtual string *getFeaturesNames();
  virtual vector<float> getFeatureValues(string s);
  virtual float getFeatureValue(string s, int i);
};

#endif /* TIMESERIES_H_ */
