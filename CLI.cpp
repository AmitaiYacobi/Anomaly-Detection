#include "CLI.h"

CLI::CLI(DefaultIO *dio) {
  this->dio = dio;
  this->ct = new CommandTemp();
  this->commands = new Command *[5];
  this->commands[0] = new Upload(dio, ct);
  this->commands[1] = new AlgorithmSettings(dio, ct);
  this->commands[2] = new AnomaliesDetection(dio, ct);
  this->commands[3] = new DisplayResults(dio, ct);
  this->commands[4] = new UploadAndAnalyze(dio, ct);
}
CLI::~CLI() {
  delete this->ct;
  for (int i = 0; i < 5; i++)
    delete this->commands[i];
  delete this->commands;
}
/**
 * @brief Gets the optional chosen commands except of "exit" command.
 *
 * @return Command**
 */
Command **CLI::getCommands() { return this->commands; }

/**
 * @brief Prints the main menu for the user the menu comtains six options.
 *
 */
void CLI::printMenu() {
  Command **commands = getCommands();
  dio->write("Welcome to the Anomaly Detection Server.\n");
  dio->write("Please choose an option:\n");
  for (int i = 0; i < 5; ++i)
    dio->write(commands[i]->getDescription() + "\n");
  dio->write("6.exit\n");
}
/**
 * @brief Gets the user choice using the "read" IO function.
 *
 * @return float
 */
float CLI::getChoice() {
  float f;
  float *g = &f;
  this->dio->read(g);
  return f;
}
/**
 * @brief Boots the program and used as an interface between the client to the
 * server.
 *
 */
void CLI::start() {
  float choice = 0;
  while (true) {
    printMenu();
    choice = getChoice();
    if (choice == 6)
      break;
    Command **commands = getCommands();
    commands[(int)choice - 1]->execute();
  }
}
