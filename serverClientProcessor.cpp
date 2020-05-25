#include "serverClientProcessor.h"

#include <cstdint>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <vector>
#include "serverPeerSocket.h"
#include "CommunicationConstants.h"


//BORRAR ESTE INCLUDE
#include <iostream>

#define HELP_MESSAGE_PART_1 "Comandos válidos:\n\tAYUDA: despliega la lista "
#define HELP_MESSAGE_PART_2 "de comandos válidos\n\tRENDIRSE: pierde el juego "
#define HELP_MESSAGE_PART_3 "automáticamente\n\tXXX: número de 3 cifras a ser "
#define HELP_MESSAGE_PART_4 "enviado al servidor para adivinar el número "
#define HELP_MESSAGE_PART_5 "secreto\n" //<-- BORRAR ESTOS \n

#define INVALID_COMMAND_MESSAGE_PART_1 "Error: comando inválido. Escriba "
#define INVALID_COMMAND_MESSAGE_PART_2 "AYUDA para obtener ayuda\n"

#define INVALID_NUMBER_MESSAGE_PART_1 "Número inválido. Debe ser de 3 cifras "
#define INVALID_NUMBER_MESSAGE_PART_2 "no repetidas\n"


#define GOOD_GUESS_MESSAGE_PART " bien"
#define REGULAR_GUESS_MESSAGE_PART " regular"
#define BAD_GUESS_MESSAGE_PART " mal"

#define MAX_NUMBER_OF_TRIES 10

//This function returns if the string (of any size) has a repeated digit
bool ClientProcessor::_has_repeated_digits(const std::string& number_string) const{
  for (size_t i = 0; i < number_string.length() - 1; i++) {
    for (size_t j = i + 1; j < number_string.length(); j++) {
      if (number_string[i] == number_string[j]) {
        return true;
      }
    }
  }
  return false;
}


void ClientProcessor::_calculate_score(const std::string guessed_number_string,
                                       int& correct_digits,
                                       int& regular_digits) const{
  for (size_t i = 0; i < NUMBERS_DIGITS_AMMOUNT; i++) {
    for (size_t j = 0; j < NUMBERS_DIGITS_AMMOUNT; j++) {
      if (number_to_guess[i] == guessed_number_string[j]) {
        if (i == j) {
          correct_digits++;
        } else {
          regular_digits++;
        }
        break;
      }
    }
  }
}

//Stores the answer message that corresponds to the received number when the
//number is valid. This function should not be used with an invalid number
//Returns true if the program should continue running, otherwise returns false
bool ClientProcessor::_store_normal_answer_message(
                                                std::string& message_to_send,
                                                int current_number_of_guesses,
                                                int correct_digits,
                                                int regular_digits) const{
/*
  if (correct_digits == NUMBERS_DIGITS_AMMOUNT) {
    message_to_send = WIN_MESSAGE;
    return false;
  } else {
    if (current_number_of_guesses == MAX_NUMBER_OF_TRIES) {
      message_to_send = LOSE_MESSAGE;
      return false;
    } else {
      if ((correct_digits == 0) && (regular_digits == 0)) {
        message_to_send = std::to_string(NUMBERS_DIGITS_AMMOUNT) + BAD_GUESS_MESSAGE_PART;
      } else {
        message_to_send = std::to_string(correct_digits) + GOOD_GUESS_MESSAGE_PART
                          + " " + std::to_string(regular_digits) +
                          REGULAR_GUESS_MESSAGE_PART;
        return true;
      }
    }
  }
*/
  if (correct_digits == NUMBERS_DIGITS_AMMOUNT) {
    message_to_send = WIN_MESSAGE;
  } else if (current_number_of_guesses == MAX_NUMBER_OF_TRIES) {
    message_to_send = LOSE_MESSAGE;
  } else {
    if ((correct_digits == 0) && (regular_digits == 0)) {
      message_to_send = std::to_string(NUMBERS_DIGITS_AMMOUNT) + BAD_GUESS_MESSAGE_PART + "\n";
    } else {
      message_to_send = std::to_string(correct_digits) + GOOD_GUESS_MESSAGE_PART
                        + ", " + std::to_string(regular_digits) +
                        REGULAR_GUESS_MESSAGE_PART + "\n";
      }
      return true;
    }
  return false;
}



//Stores the answer message that corresponds to the received number when the
//number is invalid. This function should not be used with a valid number
//Returns true if the program should continue running, otherwise returns false
bool ClientProcessor::_store_invalid_number_answer_message(
                                          std::string& message_to_send,
                                          int current_number_of_guesses) const{
  if (current_number_of_guesses == MAX_NUMBER_OF_TRIES) {
    message_to_send += LOSE_MESSAGE;
    return false;
  } else {
    message_to_send += INVALID_NUMBER_MESSAGE_PART_1;
    message_to_send += INVALID_NUMBER_MESSAGE_PART_2;
    return true;
  }
}





//Stores in message_to_send the message that corresponds to the received guess
//Returns true if the game should continue, false otherwise
bool ClientProcessor::_process_guessed_number(
                                        std::string& message_to_send,
                                        const std::string& number_to_guess,
                                        uint16_t guessed_number,
                                        int& current_number_of_guesses) const{
  current_number_of_guesses++;
  message_to_send = "";
  std::string guessed_number_string = std::to_string(guessed_number);
  if ((guessed_number_string.length() != NUMBERS_DIGITS_AMMOUNT) ||
      (_has_repeated_digits(guessed_number_string))) {
    return _store_invalid_number_answer_message(message_to_send,
                                                current_number_of_guesses);
  }
  int correct_digits = 0, regular_digits = 0;
  _calculate_score(guessed_number_string, correct_digits, regular_digits);
  return _store_normal_answer_message(message_to_send,
                                      current_number_of_guesses,
                                      correct_digits, regular_digits);

  /*
  if ((correct_digits == 0) && (regular_digits == 0)) {
    message_to_send = std::to_string(NUMBERS_DIGITS_AMMOUNT) + BAD_GUESS_MESSAGE_PART;
  } else if (correct_digits == NUMBERS_DIGITS_AMMOUNT) {
    message_to_send = WIN_MESSAGE;
  } else {
    message_to_send = std::to_string(correct_digits) + GOOD_GUESS_MESSAGE_PART
                      + " " + std::to_string(regular_digits) +
                      REGULAR_GUESS_MESSAGE_PART;
  }
  */
  //return message_to_send;
}

void ClientProcessor::_execute_help(){
  uint32_t message_len = 0;
  uint32_t aux;
  std::vector<std::string> message_parts = {HELP_MESSAGE_PART_1,
        HELP_MESSAGE_PART_2, HELP_MESSAGE_PART_3, HELP_MESSAGE_PART_4,
        HELP_MESSAGE_PART_5};
  std::vector<uint32_t> message_parts_lens;
  for (size_t i = 0; i < message_parts.size(); i++) {
    aux = message_parts[i].length();
    message_parts_lens.push_back(aux);
    message_len += aux;
  }
  message_len = htonl(message_len);
  client.send(&message_len, sizeof(uint32_t));
  for (size_t i = 0; i < message_parts.size(); i++) {
    client.send(message_parts[i].data(), message_parts_lens[i]);
  }
  //client.send("\0", sizeof(char));
}


void ClientProcessor::_execute_give_up(){
  uint32_t message_len = std::strlen(LOSE_MESSAGE);
  uint32_t message_len_to_send = htonl(message_len);
  client.send(&message_len_to_send, sizeof(uint32_t));
  client.send(LOSE_MESSAGE, message_len);
}

//ESTO ES PARA PROBAR POR AHORA EL PROGRAMA, VER SI TIENE QUE DEVOLVER VOID
//Returns if the program should end
//DESPUES DE HACER EL SOCKET EN SERIO AGREGAR EL CONST DEVUELTA
bool ClientProcessor::_execute_number(int& current_number_of_guesses){
  std::string message_to_send;
  uint16_t guessed_number;
  uint32_t answer_message_len;
  bool should_game_continue;
  client.receive(&guessed_number, sizeof(uint16_t));
  guessed_number = ntohs(guessed_number);

  /*
  message_to_send = _process_guessed_number(number_to_guess, guessed_number,
                                            current_number_of_guesses);
  */
  should_game_continue = _process_guessed_number(message_to_send,
                                                 number_to_guess,
                                                 guessed_number,
                                                 current_number_of_guesses);
  answer_message_len = message_to_send.length();
  answer_message_len = htonl(answer_message_len);
  client.send(&answer_message_len, sizeof(uint32_t));
  client.send(message_to_send.data(), message_to_send.length());

  //BORRAR
  std::cout << "CANTIDAD DE PRUEBAS ACTUALES: " << current_number_of_guesses <<"\n\n";

  return should_game_continue;
}


//Executes the command that corresponds to the command_indicator
//Returns true if the game should continue and false if it should stop
bool ClientProcessor::_execute_command(char command_indicator,
                                       int& current_number_of_guesses){
  switch (command_indicator) {
    case COMMAND_INDICATOR_HELP:
      _execute_help();
      return true;
    case COMMAND_INDICATOR_GIVE_UP:
    _execute_give_up();
      return false;
    case COMMAND_INDICATOR_NUMBER:
      return _execute_number(current_number_of_guesses);
    default:
      //VER SI HACE FALTA TIRAR UNA EXCEPCION EN CASO DE QUE NO SE CONOZCA EL
      //COMANDO, EL CLIENTE CHEQUEA QUE EL COMANDO ESCRITO SEA VALIDO
      return false;
  }
}


//void ClientProcessor::_run_game(uint16_t& number_to_guess){
//VER SI HAY QUE AGREGAR CONST
void ClientProcessor::_run_game(){
  int current_number_of_guesses = 0;
  bool should_continue = true;
  char command_indicator;
  while (should_continue) {
    client.receive(&command_indicator, sizeof(char));
    should_continue = _execute_command(command_indicator,
                                       current_number_of_guesses);
  }
  has_program_ended = true;
}


///////////////////////////////PUBLIC//////////////////////////

bool ClientProcessor::join(){
  thrd.join();
  return has_player_won;
}

bool ClientProcessor::has_ended(){
  return has_program_ended;
}


//VER SI HAY QUE AGREGAR CONST
void ClientProcessor::operator()(){
  std::thread aux_thread(&ClientProcessor::_run_game, this);
  thrd = std::move(aux_thread);
  //LLAMAR A ESTA FUNCION EN EL THREAD
  //_run_game();
}

/*
ClientProcessor& ClientProcessor::operator=(ClientProcessor&& other) noexcept{

}
*/

//ClientProcessor::ClientProcessor(PeerSocket& peer_socket, int& number_to_guess){
ClientProcessor::ClientProcessor(PeerSocket& peer_socket,
                                 const std::string& number_to_guess):
                                 //number_to_guess(std::move(number_to_guess)),
                                 number_to_guess(number_to_guess),
                                 client(std::move(peer_socket)),
                                 has_program_ended(false){
  has_player_won = false;
}

ClientProcessor::ClientProcessor(ClientProcessor&& other) noexcept:
                            number_to_guess(std::move(other.number_to_guess)),
                            client(std::move(other.client)){
  //const std::string& number_to_guess;
  //const PeerSocket client;
  std::thread thrd = std::move(other.thrd);
  has_program_ended = static_cast<bool>(other.has_program_ended);
  has_player_won = other.has_player_won;
}

ClientProcessor::~ClientProcessor(){
  //For exception safety
  if (thrd.joinable()) {
    thrd.join();
  }
}
