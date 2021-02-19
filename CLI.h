

#ifndef CLI_H_
#define CLI_H_

#include <string>

#include "commands.h"

using namespace std;

/**
 * @brief "Command Line Interface" class which represents the interface between
 * the client to the server.
 *
 */
class CLI {
  DefaultIO *dio;
  Command **commands;
  CommandTemp *ct;

public:
  CLI(DefaultIO *dio);
  float choice;
  void start();
  void printMenu();
  float getChoice();
  Command **getCommands();
  virtual ~CLI();
};

#endif /* CLI_H_ */
