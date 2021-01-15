// 316418300 Amitai Yacobi
#include "timeseries.h"

/**
 * @brief Reads the data from the csv file and organizing it into a map which
 * the keys of the map are the features names, and the values are vectors of
 * float numbers.
 *
 * @return map<string, vector<float>>
 */
map<string, vector<float>> TimeSeries::readCsv(const char* CSVfileName)
{
    string line;
    string featureName;
    vector<string> colNames;
    map<string, vector<float>> result;
    float val;
    int j = 0;

    ifstream csvFile(CSVfileName);
    if (!csvFile.is_open()) {
        throw runtime_error("Could not open file");
    }

    // Reading the first line of the file which is the columns names
    if (csvFile.good()) {
        getline(csvFile, line);
        stringstream strStream(line);
        while (getline(strStream, featureName, ',')) {
            colNames.push_back(featureName);
            result[featureName] = vector<float> {};
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

/**
 * @brief Finds the position of a given feature in
 * the csv file. i.e. the column nuber of the feature.
 *
 * @param s string
 * @return int
 */
int TimeSeries::getFeaturePosition(string s) const
{
    vector<string>::iterator it;
    auto vec = this->featuresNames;
    int position = 0;

    it = find(vec.begin(), vec.end(), s);
    position = distance(vec.begin(), it);
    return position;
}

/**
 * @brief Returns the i value in the vector of a given feature.
 *
 * @param s feature name
 * @param i index in the vector
 * @return float
 */
float TimeSeries::getFeatureValue(string s, int i) const
{
    auto dt = this->data.at(s)[i];
    return dt;
}

/**
 * @brief The function returns a vector of strings which contains all of the
 * features names.
 *
 * @return vector<string>
 */
vector<string> TimeSeries::getFeaturesNames() const
{
    return this->featuresNames;
}

/**
 * @brief Returns the vector of the given feature, using the map
 * that was created above.
 *
 * @param s featue name
 * @return vector<float>
 */
vector<float> TimeSeries::getFeatureValues(string s) const
{
    auto dt = this->data.at(s);
    return dt;
}

/**
 * @brief Returns the map created above.
 *
 * @return map<string, vector<float>>
 */
map<string, vector<float>> TimeSeries::getData() const { return this->data; }
