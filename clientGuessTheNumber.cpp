#include "clientGuessTheNumber.h"

#include <cstdint>
#include <string>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <vector>
#include "clientSocket.h"
#include "CommunicationConstants.h"

#define PROCESS_FINISHED 0

#define NUMBER_OF_ARGUMENTS 3
#define HOST_ARGUMENT_INDEX 1
#define SERVICE_ARGUMENT_INDEX 2

#define MESSAGE_RECEIVER_BUFFER_LEN 8


#define INVALID_ARGUMENTS_TEXT "Error: argumentos inválidos\n"
#define INVALID_COMMAND_MESSAGE "Error: comando inválido. Escriba AYUDA para "\
                                "obtener ayuda\n"

#define SOCKET_ERROR_TEXT "Error de scoket"

#define HELP_COMMAND_TEXT "AYUDA"
#define GIVE_UP_COMMAND_TEXT "RENDIRSE"

#define LOCALHOST_INDICATOR "localhost"

//Returns the char that indicates the server the command that has to be
//executed
char ClientGuessTheNumber::_get_command_indicator(
                                        const std::string& command_text){
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

//Receives the answer from the server after sending a command
void ClientGuessTheNumber::_receive_message(ClientSocket& socket,
                                            std::vector<char>& answer){
  uint32_t number_of_chars;
  socket.receive(&number_of_chars, 4);
  number_of_chars = ntohl(number_of_chars);
  answer.resize(number_of_chars + 1);
  socket.receive(answer.data(), number_of_chars + 1);
}

bool ClientGuessTheNumber::_are_strings_equal(const char* str1,
                                              const char* str2){
  if (std::strlen(str1) != std::strlen(str2)) {
    return false;
  }
  return !std::strcmp(str1, str1);
}

//Returns true if the game has finished, otherwise returns false
bool ClientGuessTheNumber::_is_game_finished(const char* answer){
  return ((_are_strings_equal(answer, WIN_MESSAGE)) ||
          (_are_strings_equal(answer, LOSE_MESSAGE)));
}

//Sends the command request to the server and prints it answer
//Returns true if the program should continue running, otherwise returns false
bool ClientGuessTheNumber::_process_command(ClientSocket& socket,
                                            const std::string& command){
  char command_indicator = _get_command_indicator(command);
  //std::string answer;
  std::vector<char> answer;
  uint16_t number;
  socket.send(&command_indicator, 1/*sizeof(char)*/);
  if (command_indicator == COMMAND_INDICATOR_NUMBER) {
    number = htons(std::stoi(command));
    socket.send(&number, 2/*sizeof(uint16_t)*/);
  }
  _receive_message(socket, answer);
  //std::cout << answer;
  std::cout << answer.data();
  return !_is_game_finished(answer.data());
}

//Handles the execution of the program when an invalid command is read
//Prints an invalid command message
//If the client forced an eof it kills the client
void ClientGuessTheNumber::_handle_command_error(bool& keep_running){
  keep_running = true;
  if (std::cin.eof()) {
    keep_running = false;
  }
  std::cout << INVALID_COMMAND_MESSAGE;
}

//This function runs the client's program, it reads a command
void ClientGuessTheNumber::_run_client(ClientSocket& socket){
  bool keep_running = true;
  std::string command;
  while (keep_running) {
    std::getline(std::cin, command);
    try {
      keep_running = _process_command(socket, command);
    } catch(std::invalid_argument& e) {
      _handle_command_error(keep_running);
    } catch(std::out_of_range& e) {
      _handle_command_error(keep_running);
    }
  }
}

///////////////////////////////PUBLIC//////////////////////////

int ClientGuessTheNumber::execute(const char** arguments,
                                  int number_of_arguments){
  if (number_of_arguments != NUMBER_OF_ARGUMENTS) {
    std::cout << INVALID_ARGUMENTS_TEXT;
    return PROCESS_FINISHED;
  }
  std::string host(arguments[HOST_ARGUMENT_INDEX]);
  std::string service(arguments[SERVICE_ARGUMENT_INDEX]);
  if (host == LOCALHOST_INDICATOR) {
    host = "";
  }
  ClientSocket socket(service, host);
  try {
    socket.open_communication_channel();
  } catch(std::system_error e) {
    std::cout << SOCKET_ERROR_TEXT << std::endl;
    return PROCESS_FINISHED;
  }
  _run_client(socket);
  return PROCESS_FINISHED;
}

ClientGuessTheNumber::ClientGuessTheNumber(){
}

ClientGuessTheNumber::~ClientGuessTheNumber(){
}
