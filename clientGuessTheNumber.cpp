#include "clientGuessTheNumber.h"

#include <cstdint>
#include <string>
#include <iostream>

#define PROCESS_FINISHED 0

#define NUMBER_OF_ARGUMENTS 3
#define HOST_ARGUMENT_INDEX 1
#define PORT_ARGUMENT_INDEX 2

#define INVALID_ARGUMENTS_TEXT "Error: argumentos inválidos"
#define INVALID_COMMAND_TEXT "Error: comando inválido"

#define HELP_COMMAND_TEXT "AYUDA"
#define GIVE_UP_COMMAND_TEXT "RENDIRSE"

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
  }
}

///////////////////////////////PUBLIC//////////////////////////

int ClientGuessTheNumber::execute(const char** arguments, int number_of_arguments){
  if (number_of_arguments != NUMBER_OF_ARGUMENTS) {
    std::cout << INVALID_ARGUMENTS_TEXT;
    return PROCESS_FINISHED;
  }
  return PROCESS_FINISHED;
}

ClientGuessTheNumber::ClientGuessTheNumber(){
}

ClientGuessTheNumber::~ClientGuessTheNumber(){
}
