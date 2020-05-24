#ifndef _CLIENT_GUESS_THE_NUMBER_H_
#define  _CLIENT_GUESS_THE_NUMBER_H_

#include <string>

class ClientGuessTheNumber {
  char _get_command_indicator(const std::string& command_text) const;

public:
  ClientGuessTheNumber();

  ~ClientGuessTheNumber();

  int execute(const char** arguments, int number_of_arguments);
};

#endif
