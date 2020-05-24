#ifndef _CLIENT_GUESS_THE_NUMBER_H_
#define  _CLIENT_GUESS_THE_NUMBER_H_

#include <string>
#include "clientSocket.h"

class ClientGuessTheNumber {
  bool _process_command(ClientSocket& socket,
                        const std::string& command) const;
  bool _is_game_finished(const std::string& answer) const;
  void _receive_message(ClientSocket& socket, std::string& answer) const;
  char _get_command_indicator(const std::string& command_text) const;
  bool _process_command(const std::string& command) const;
public:
  ClientGuessTheNumber();

  ~ClientGuessTheNumber();

  int execute(const char** arguments, int number_of_arguments);
};

#endif
