#include "timeseries.h"

map<string, vector<float>> TimeSeries::readCsv() {

  ifstream csvFile(this->fileName);

  if (!csvFile.is_open()) {
    throw runtime_error("Could not open file");
  }

  string line;
  string featureName;
  string *colNames = {};
  map<string, vector<float>> ts;
  float val;
  int i = 0;
  int j = 1;

  // Reading the first line of the file which is the columns names
  if (csvFile.good()) {

    getline(csvFile, line);
    stringstream strStream(line);

    while (getline(strStream, featureName, ',')) {
      colNames[i] = featureName;
      ts[featureName] = vector<float>{};
      i++;
    }
  }

  // Read the data line by line
  while (getline(csvFile, line)) {
    j = 0;
    stringstream strStream(line);
    while (strStream >> val) {
      ts[colNames[j]].push_back(val);
      if (strStream.peek() == ',')
        strStream.ignore();
      j++;
    }
  }

  csvFile.close();
  this->featuresNames = colNames;
  return ts;
}

string *TimeSeries::getFeaturesNames() { return this->featuresNames; }

vector<float> TimeSeries::getFeatureValues(string s) { return this->ts[s]; }

float TimeSeries::getFeatureValue(string s, int i) { return this->ts[s][i]; }
