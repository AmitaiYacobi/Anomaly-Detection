#include "timeseries.h"

map<string, vector<float>> TimeSeries::readCsv() {

  ifstream csvFile(this->fileName);
  if (!csvFile.is_open()) {
    throw runtime_error("Could not open file");
  }

  string line;
  string featureName;
  vector<string> colNames;
  map<string, vector<float>> result;
  float val;
  int j = 0;

  // Reading the first line of the file which is the columns names
  if (csvFile.good()) {
    getline(csvFile, line);
    stringstream strStream(line);
    while (getline(strStream, featureName, ',')) {
      colNames.push_back(featureName);
      result[featureName] = vector<float>{};
    }
  }

  // Read the data line by line
  while (getline(csvFile, line)) {
    j = 0;
    stringstream strStream(line);
    while (strStream >> val) {
      result[colNames[j]].push_back(val);
      if (strStream.peek() == ',')
        strStream.ignore();
      j++;
    }
  }

  csvFile.close();
  this->featuresNames = colNames;
  return result;
}
int TimeSeries::getFeaturePosition(string s) const {
  vector<string>::iterator it;
  auto vec = this->featuresNames;
  int position = 0;

  it = find(vec.begin(), vec.end(), s);
  position = distance(vec.begin(), it);
  return position;
}

vector<string> TimeSeries::getFeaturesNames() const {
  return this->featuresNames;
}

vector<float> TimeSeries::getFeatureValues(string s) const {
  return this->data.at(s);
}

float TimeSeries::getFeatureValue(string s, int i) const {
  return this->data.at(s)[i];
}

map<string, vector<float>> TimeSeries::getData() const { return this->data; }

TimeSeries::~TimeSeries() { delete this->fileName; }
