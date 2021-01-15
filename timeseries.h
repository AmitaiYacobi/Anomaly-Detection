// 316418300 Amitai Yacobi

#include <algorithm>
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

/**
 * @brief Represents the data which we want to test and train in order
 * to detect anomalies.
 */

class TimeSeries {
    const char* fileName;
    map<string, vector<float>> data;
    vector<string> featuresNames;

public:
    TimeSeries(const char* CSVfileName)
    {
        this->fileName = CSVfileName;
        this->data = readCsv(CSVfileName);
    }
    ~TimeSeries() { }
    virtual map<string, vector<float>> readCsv(const char* CSVfileName);

    virtual vector<string> getFeaturesNames() const;
    virtual vector<float> getFeatureValues(string s) const;
    virtual map<string, vector<float>> getData() const;
    virtual int getFeaturePosition(string s) const;
    virtual float getFeatureValue(string s, int i) const;
};

#endif /*TIMESERIES_H_*/
