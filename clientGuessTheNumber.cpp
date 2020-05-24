#include "clientGuessTheNumber.h"

#include <cstdint>
#include <string>
#include <iostream>
#include "clientSocket.h"

#define PROCESS_FINISHED 0

#define NUMBER_OF_ARGUMENTS 3
#define HOST_ARGUMENT_INDEX 1
#define SERVICE_ARGUMENT_INDEX 2

#define INVALID_ARGUMENTS_TEXT "Error: argumentos inválidos"
#define INVALID_COMMAND_TEXT "Error: comando inválido"

#define HELP_COMMAND_TEXT "AYUDA"
#define GIVE_UP_COMMAND_TEXT "RENDIRSE"

#define LOCALHOST_INDICATOR "localhost"

//Returns the char that indicates the server the command that has to be
//executed
char ClientGuessTheNumber::_get_command_indicator(const std::string& command_text) const{
  int number;
  if (command_text == HELP_COMMAND_TEXT) {
    return COMMAND_INDICATOR_HELP;
  } else if (command_text == GIVE_UP_COMMAND_TEXT) {
    return COMMAND_INDICATOR_GIVE_UP;
  } else {
    //If the text cannot be turned into a number then it's an invalid command
    number = std::stoi(command_text);
    //Checks if the value can be stored in a uint16_t type variable
    if ((number < 0) || (number > UINT16_MAX)) {
      throw(std::invalid_argument("Number outside range"));
    }
    return COMMAND_INDICATOR_NUMBER;
  }
}

//Sends the command request to the server and prints it answer
//Returns true if the program should continue running, otherwise returns false
bool ClientGuessTheNumber::_process_command(const std::string& command) const{
  char command_indicator = _get_command_indicator(command);

}

///////////////////////////////PUBLIC//////////////////////////

int ClientGuessTheNumber::execute(const char** arguments, int number_of_arguments){
  if (number_of_arguments != NUMBER_OF_ARGUMENTS) {
    std::cout << INVALID_ARGUMENTS_TEXT;
    return PROCESS_FINISHED;
  }
  std::string host(arguments[HOST_ARGUMENT_INDEX]);
  std::string service(arguments[SERVICE_ARGUMENT_INDEX]);
  if (host == LOCALHOST_INDICATOR) {
    host = "";
  }
  bool keep_running = true;
  std::string command;
  char command_indicator;
  ClientSocket socket;
  try {
    socket.connect(host, service);
  } catch() {

  }
  while (keep_running) {
    std::getline(std::cin, command);
    try {
      //command_indicator = _get_command_indicator(command);
      keep_running = _process_command(command);
    } catch(std::invalid_argument& e) {
      keep_running = true;
      if (std::cin.eof()) {
        keep_running = false;
      }
      std::cout << INVALID_COMMAND_TEXT;
    }
  }
  return PROCESS_FINISHED;
}

ClientGuessTheNumber::ClientGuessTheNumber(){
}

ClientGuessTheNumber::~ClientGuessTheNumber(){
}
