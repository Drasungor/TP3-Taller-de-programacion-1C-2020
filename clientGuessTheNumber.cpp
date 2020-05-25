#include "clientGuessTheNumber.h"

#include <cstdint>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include "clientSocket.h"
#include "CommunicationConstants.h"

#define PROCESS_FINISHED 0

#define NUMBER_OF_ARGUMENTS 3
#define HOST_ARGUMENT_INDEX 1
#define SERVICE_ARGUMENT_INDEX 2

#define MESSAGE_RECEIVER_BUFFER_LEN 8


#define INVALID_ARGUMENTS_TEXT "Error: argumentos inválidos\n"
//#define INVALID_COMMAND_TEXT "Error: comando inválido"
#define INVALID_COMMAND_MESSAGE_PART_1 "Error: comando inválido. Escriba "
#define INVALID_COMMAND_MESSAGE_PART_2 "AYUDA para obtener ayuda\n"


#define SOCKET_ERROR_TEXT "Error de scoket"

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

//VER SI TIENE SENTIDO PONERLE CONST A LOS METODOS DE UNA CLASE QUE NO TIENE ATRIBUTOS
void ClientGuessTheNumber::_receive_message(ClientSocket& socket, std::string& answer) const{
  uint32_t number_of_chars;
  char buffer[MESSAGE_RECEIVER_BUFFER_LEN + 1];
  socket.receive(&number_of_chars, sizeof(uint32_t));
  number_of_chars = ntohl(number_of_chars);

  //VER SI SE CAMBIA ESTE WHILE POR UN SOLO PEDIDO USANDO UN BUFFER DEL TAMAÑO DEL
  //MENSAJE MAS LARGO (FIJARSE EL STRLEN DEL MENSAJE LARGO)
  size_t received_chars = 0;
  size_t asked_chars = MESSAGE_RECEIVER_BUFFER_LEN;

  while (received_chars < number_of_chars) {
    if ((number_of_chars - received_chars) < MESSAGE_RECEIVER_BUFFER_LEN) {
      asked_chars = number_of_chars - received_chars;
    }
    socket.receive(buffer, sizeof(char) * asked_chars);
    buffer[asked_chars] = '\0';
    std::string aux(buffer);
    answer += aux;
    received_chars += asked_chars;
  }
}

bool ClientGuessTheNumber::_is_game_finished(const std::string& answer) const{
  return ((answer == WIN_MESSAGE) || (answer == LOSE_MESSAGE));
}

//Sends the command request to the server and prints it answer
//Returns true if the program should continue running, otherwise returns false
bool ClientGuessTheNumber::_process_command(ClientSocket& socket,
                                            const std::string& command) const{
  char command_indicator = _get_command_indicator(command);
  std::string answer;
  uint16_t number;
  socket.send(&command_indicator, sizeof(char));
  if (command_indicator == COMMAND_INDICATOR_NUMBER) {
    number = htons(std::stoi(command));
    socket.send(&number, sizeof(uint16_t));
  }
  _receive_message(socket, answer);
  std::cout << answer;
  return !_is_game_finished(answer);
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
  //ClientSocket socket();
  ClientSocket socket(host, service);
  //HACER SOCKET_CONNECT
  try {
    //socket.connect(host, service);
    socket.open_communication_channel();
  } catch(std::system_error e) {
    //VER SI EL RETURN ACA HACE QUE SE GENERE ALGUN LEAK
    std::cout << SOCKET_ERROR_TEXT << std::endl;
    std::cout << e.what() << std::endl;
    return PROCESS_FINISHED;
  }
  while (keep_running) {
    std::getline(std::cin, command);
    try {
      keep_running = _process_command(socket, command);
    } catch(std::invalid_argument& e) {
      keep_running = true;
      if (std::cin.eof()) {
        keep_running = false;
      }
      std::cout << INVALID_COMMAND_MESSAGE_PART_1 <<
                   INVALID_COMMAND_MESSAGE_PART_2;
    }
  }
  return PROCESS_FINISHED;
}

ClientGuessTheNumber::ClientGuessTheNumber(){
}

ClientGuessTheNumber::~ClientGuessTheNumber(){
}
