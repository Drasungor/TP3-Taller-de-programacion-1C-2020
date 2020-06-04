#ifndef _CLIENT_GUESS_THE_NUMBER_H_
#define  _CLIENT_GUESS_THE_NUMBER_H_

#include <string>
#include <vector>
#include "clientSocket.h"

//This class represents the program used by the client
class ClientGuessTheNumber {
  void _handle_command_error(bool& keep_running);
  void _run_client(ClientSocket& socket);
  bool _are_strings_equal(const char* str1, const char* str2);
  bool _process_command(ClientSocket& socket, const std::string& command);
  bool _is_game_finished(const char* answer);
  void _receive_message(ClientSocket& socket, std::vector<char>& answer);
  char _get_command_indicator(const std::string& command_text);
  bool _process_command(const std::string& command);

public:
  ClientGuessTheNumber();

  ~ClientGuessTheNumber();

  //Runs the program, returns 0
  int execute(const char** arguments, int number_of_arguments);
};

#endif
