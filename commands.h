

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iomanip>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <thread>

#include <fstream>
#include <iostream>
#include <vector>

#include "HybridAnomalyDetector.h"

using namespace std;

/**
 * @brief An abstract IO class which contains read and write methods, and also
 * ables to uplaod files to the server.
 *
 */
class DefaultIO {
public:
  virtual ~DefaultIO() {}
  virtual string read() = 0;
  virtual void write(string text) = 0;
  virtual void write(float f) = 0;
  virtual void read(float *f) = 0;
  /**
   * @brief Create a File object
   *
   * @param fileName
   */
  void createFile(string fileName) { ofstream newFile(fileName); }
  /**
   * @brief Adds line to an exist file.
   *
   * @param fileName
   * @param line
   */
  void addLine(string fileName, string line) {
    ofstream outFile;
    outFile.open(fileName, ios::app);
    outFile << line << "\n";
  }
  /**
   * @brief Uploads file using the createFile and addLine methods.
   *
   * @param fileName
   */
  void upload(string fileName) {
    string line = read();
    createFile(fileName);
    while (line != "done") {
      addLine(fileName, line);
      line = read();
    }
  }
};

class SocketIO : public DefaultIO {
  int clientFD;

public:
  SocketIO(int clientFD) : clientFD(clientFD) {}
  ~SocketIO() {}
  string read() {
    string input = "";
    char c = 0;
    ::read(clientFD, &c, sizeof(char));
    while (c != '\n') {
      input += c;
      ::read(clientFD, &c, sizeof(char));
    }
    return input;
  }
  void read(float *f) { ::read(clientFD, f, sizeof(float *)); }
  void write(string text) { ::write(clientFD, text.c_str(), text.length()); }
  void write(float f) { ::write(clientFD, &f, sizeof(float)); }
};

/**
 * @brief A Command-Helper class which contains helper methods that used by the
 * different commands classes.
 *
 */
class CommandTemp {
  TimeSeries *ts;
  HybridAnomalyDetector *ad;

public:
  CommandTemp() { this->ad = new HybridAnomalyDetector(); }
  /**
   * @brief Set the Time Series object
   *
   * @param CSVfileName
   */
  void setTimeSeries(const char *CSVfileName) {
    ts = new TimeSeries(CSVfileName);
  }
  virtual ~CommandTemp() {
    delete this->ad;
    if (ts != NULL)
      delete this->ts;
  }
  /**
   * @brief Get the Time Series object
   *
   * @return TimeSeries*
   */
  TimeSeries *getTimeSeries() { return this->ts; }
  /**
   * @brief Get the Anomaly Decorator object
   *
   * @return HybridAnomalyDetector*
   */
  HybridAnomalyDetector *getAnomalyDecorator() { return this->ad; }
  /**
   * @brief Helper method which gets a vector of anomalies reports and unites
   * every group of reports that come in a row into one report.
   *
   * @param ar
   * @return map<string, vector<float>>
   */
  map<string, vector<float>> newReports(vector<AnomalyReport> ar) {
    float start;
    float end;
    vector<AnomalyReport>::iterator it1;
    vector<AnomalyReport>::iterator it2;
    map<string, vector<float>> reports;
    reports["start"] = vector<float>{};
    reports["end"] = vector<float>{};
    for (it1 = ar.begin(); it1 != ar.end(); it1++) {
      start = it1->timeStep;
      for (it2 = ++it1; it2 != ar.end(); it2++) {
        if ((start + 1) == it2->timeStep) {
          start = it2->timeStep;
          continue;
        } else {
          break;
        }
      }
      --it1;
      start = it1->timeStep;
      end = (--it2)->timeStep;
      reports["start"].push_back(start);
      reports["end"].push_back(end);
      it1 = it2;
    }
    return reports;
  }
  /**
   * @brief Gets an anomalies file and mapps it's data.
   *
   * @param fileName
   * @return map<string, vector<float>>
   */
  map<string, vector<float>> readAnomalyTXT(string fileName) {
    float start;
    float end;
    string line;
    map<string, vector<float>> anomalies;
    anomalies["start"] = vector<float>{};
    anomalies["end"] = vector<float>{};
    ifstream txtFile(fileName);
    if (!txtFile.is_open()) {
      throw runtime_error("Could not open file");
    }
    while (getline(txtFile, line)) {
      if (line == "done") {
        break;
      }
      stringstream strStream(line);
      strStream >> start;
      anomalies["start"].push_back(start);
      if (strStream.peek() != ',') {
        anomalies["end"].push_back(start);
        continue;
      }
      strStream.ignore();
      strStream >> end;
      anomalies["end"].push_back(end);
    }
    txtFile.close();
    return anomalies;
  }
  /**
   * @brief Checks whether two sections have intersection between them.
   *
   * @param s1
   * @param e1
   * @param s2
   * @param e2
   * @return true
   * @return false
   */
  bool checkIntersect(float s1, float e1, float s2, float e2) {
    if ((s1 >= s2 && e1 <= e2) || (s2 >= s1 && e2 <= e1) ||
        (e1 >= s2 && s2 >= s1) || (e2 >= s1 && s1 >= s2)) {
      return true;
    }
    return false;
  }
};

/**
 * @brief Abstract Command class which has a description and execution method.
 *
 */
class Command {
  DefaultIO *dio;
  CommandTemp *ct;

public:
  Command(DefaultIO *dio, CommandTemp *ct) : dio(dio), ct(ct) {}
  Command() {}
  virtual void execute() = 0;
  virtual string getDescription() = 0;
  virtual DefaultIO *getIO() { return this->dio; }
  virtual CommandTemp *getCmdTemp() { return this->ct; }
  virtual ~Command() {}
};

/**
 * @brief The Upload command class is responsible for getting a csv file from
 * the user and for uploading it to the server.
 *
 */
class Upload : public Command {
  string description;

public:
  Upload(DefaultIO *dio, CommandTemp *ct) : Command(dio, ct) {
    this->description = "1.upload a time series csv file";
  }
  virtual ~Upload() {}
  virtual string getDescription() { return this->description; }
  void execute() {
    getIO()->write("Please upload your local train CSV file.\n");
    // uploads the train csv for the learning step.
    getIO()->upload("anomalyTrain.csv");
    getIO()->write("Upload complete.\n");
    getIO()->write("Please upload your local test CSV file.\n");
    // uploads the test csv for the detection step.
    getIO()->upload("anomalyTest.csv");
    getIO()->write("Upload complete.\n");
  }
};

/**
 * @brief The AlgorithmSetting command class is responsible to present the
 * current correlation threshold  to the user and ask the user to choose
 * different threshold.
 * Finally, according to the new threshold, the program will choose which
 * detection algorith to use - the linear regression algorithm or
 * the minimum circle algorithm.
 *
 */
class AlgorithmSettings : public Command {
  string description;

public:
  AlgorithmSettings(DefaultIO *dio, CommandTemp *ct) : Command(dio, ct) {
    this->description = "2.algorithm settings";
  }
  virtual ~AlgorithmSettings() {}
  virtual string getDescription() { return this->description; }
  void execute() {
    float choice;
    float t = getCmdTemp()->getAnomalyDecorator()->getThreshold();
    getIO()->write("The current correlation threshold is ");
    getIO()->write(t);
    getIO()->write("\n");
    getIO()->write("Type a new threshold \n");
    getIO()->read(&choice);
    // checking whether the user's choice is valid.
    if (choice >= 0 && choice <= 1) {
      getCmdTemp()->getAnomalyDecorator()->setThreshold(choice);
      return;
    } else {
      getIO()->write("please choose a value between 0 and 1.\n");
      getIO()->read();
      execute();
    }
    return;
  }
};

/**
 * @brief The AnomaliesDetection command class is responsible for learning the
 * train data and detecting anomalies according it.
 *
 */
class AnomaliesDetection : public Command {
  string description;

public:
  AnomaliesDetection(DefaultIO *dio, CommandTemp *ct) : Command(dio, ct) {
    this->description = "3.detect anomalies";
  }
  virtual ~AnomaliesDetection() {}
  virtual string getDescription() { return this->description; }
  void execute() {
    TimeSeries *ts;
    vector<AnomalyReport> ar;
    getCmdTemp()->setTimeSeries("anomalyTrain.csv");
    ts = getCmdTemp()->getTimeSeries();
    getCmdTemp()->getAnomalyDecorator()->learnNormal(*ts); // The learning step
    getCmdTemp()->setTimeSeries("anomalyTest.csv");
    ts = getCmdTemp()->getTimeSeries();
    getCmdTemp()->getAnomalyDecorator()->detect(*ts); // The detection step
    getIO()->write("anomaly detection complete.\n");
  }
};

/**
 * @brief The DisplayResults command class is responsible of displaying
 * all the anomalies that were deteced.
 *
 */
class DisplayResults : public Command {
  string description;

public:
  DisplayResults(DefaultIO *dio, CommandTemp *ct) : Command(dio, ct) {
    this->description = "4.display results";
  }
  virtual ~DisplayResults() {}
  virtual string getDescription() { return this->description; }
  void execute() {
    string timestep;
    string description;
    vector<AnomalyReport>::iterator it;
    TimeSeries *ts = getCmdTemp()->getTimeSeries();
    vector<AnomalyReport> ar = getCmdTemp()->getAnomalyDecorator()->detect(*ts);
    for (it = ar.begin(); it != ar.end(); it++) {
      timestep = to_string(it->timeStep);
      description = it->description;
      getIO()->write(timestep + "     " + description + "\n");
    }
    getIO()->write("Done.\n");
  }
};

/**
 * @brief The UploadAndAnalyze command class is responsible for getting an
 * anomalies file from the user, and for comparing the given file to the
 * anomalies report that we already have from the detection part.
 *
 */
class UploadAndAnalyze : public Command {
  string description;

public:
  UploadAndAnalyze(DefaultIO *dio, CommandTemp *ct) : Command(dio, ct) {
    this->description = "5.upload anomalies and analyze results";
  }
  virtual ~UploadAndAnalyze() {}
  virtual string getDescription() { return this->description; }
  void execute() {
    float P; // number of anomalies in the anomalies file.
    float N; // number of time steps that that there is no anomalies in them.
    float S; // number of reports after the union.
    float TP = 0; // true positive- there is intersection between a report to
                  // an anomaly.
    float FP = 0; // false positive - there is no intersection between some
                  // report to all of the anomalies
    float n;      // number of lines in the input csv file.
    float counter = 0;
    string name;
    map<string, vector<float>> reports;
    map<string, vector<float>> anomalies;
    TimeSeries *ts = getCmdTemp()->getTimeSeries();
    vector<AnomalyReport> ar = getCmdTemp()->getAnomalyDecorator()->detect(*ts);
    getIO()->write("Please upload your local anomalies file.\n");
    getIO()->upload("userAnomalies.txt");
    getIO()->write("Upload complete.\n");
    reports = getCmdTemp()->newReports(ar);
    anomalies = getCmdTemp()->readAnomalyTXT("userAnomalies.txt");
    name = ts->getFeaturesNames()[0];
    P = anomalies["start"].size();
    S = reports["start"].size();
    n = ts->getFeatureValues(name).size();
    for (int i = 0; i < P; ++i) {
      n -= (anomalies["end"][i] - anomalies["start"][i]) + 1;
    }
    N = n;
    for (int i = 0; i < S; ++i) {
      for (int j = 0; j < P; ++j) {
        // checks if there is an intersection between the report to the anomaly.
        bool x = getCmdTemp()->checkIntersect(
            reports["start"][i], reports["end"][i], anomalies["start"][j],
            anomalies["end"][j]);
        if (x) {
          counter++;
          TP++;
        }
      }
      if (!counter)
        FP++;
      counter = 0;
    }
    TP = TP / P;
    FP = FP / N;
    TP = floor(TP * 1000) / 1000; // we want three digits after the decimal.
    FP = floor(FP * 1000) / 1000;
    string tp = to_string(TP);
    string fp = to_string(FP);
    tp.erase(tp.find_last_not_of('0') + 1, string::npos);
    fp.erase(fp.find_last_not_of('0') + 1, string::npos);
    getIO()->write("True Positive Rate: ");
    getIO()->write(tp);
    getIO()->write("\n");
    getIO()->write("False Positive Rate: ");
    getIO()->write(fp);
    getIO()->write("\n");
  }
};

#endif /* COMMANDS_H_ */
