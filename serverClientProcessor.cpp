#include "serverClientProcessor.h"

#include <cstdint>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <vector>
#include <utility>
#include "serverPeerSocket.h"
#include "CommunicationConstants.h"

#define HELP_MESSAGE "Comandos válidos:\n\tAYUDA: despliega la lista "\
                     "de comandos válidos\n\tRENDIRSE: pierde el juego "\
                     "automáticamente\n\tXXX: Número de 3 cifras a ser "\
                     "enviado al servidor para adivinar el número "\
                     "secreto\n"
#define INVALID_COMMAND_MESSAGE "Comando inexistente\n"
#define INVALID_NUMBER_MESSAGE "Número inválido. Debe ser de 3 cifras "\
                               "no repetidas\n"
#define GOOD_GUESS_MESSAGE_PART " bien"
#define REGULAR_GUESS_MESSAGE_PART " regular"
#define BAD_GUESS_MESSAGE_PART " mal"

#define MAX_NUMBER_OF_TRIES 10

//This function returns if the string (of any size) has a repeated digit
bool ClientProcessor::_has_repeated_digits(
                                      const std::string& number_string) const{
  for (size_t i = 0; i < number_string.length() - 1; i++) {
    for (size_t j = i + 1; j < number_string.length(); j++) {
      if (number_string[i] == number_string[j]) {
        return true;
      }
    }
  }
  return false;
}

//Stores in the received ints the ammount of correct and regular digits
void ClientProcessor::_calculate_score(const std::string guessed_number_string,
                                       int& correct_digits,
                                       int& regular_digits) const{
  correct_digits = 0;
  regular_digits = 0;
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


//Stores in message_to_send the ammount og good, bad, or regular digits with
//the appropiate format
void ClientProcessor::_build_non_winning_message(
                                                std::string& message_to_send,
                                                int correct_digits,
                                                int regular_digits){
  if ((correct_digits == 0) && (regular_digits == 0)) {
    message_to_send = std::to_string(NUMBERS_DIGITS_AMMOUNT) +
                      BAD_GUESS_MESSAGE_PART + "\n";
  } else {
    if (correct_digits != 0) {
      message_to_send = std::to_string(correct_digits) +
                        GOOD_GUESS_MESSAGE_PART;
      if (regular_digits != 0) {
        message_to_send += ", ";
      } else {
        message_to_send += "\n";
      }
    }
    if (regular_digits != 0) {
      message_to_send += std::to_string(regular_digits) +
      REGULAR_GUESS_MESSAGE_PART + "\n";
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
                                                int regular_digits){
  if (correct_digits == NUMBERS_DIGITS_AMMOUNT) {
    message_to_send = WIN_MESSAGE;
    winners++;
  } else if (current_number_of_guesses == MAX_NUMBER_OF_TRIES) {
    message_to_send = LOSE_MESSAGE;
    losers++;
  } else {
    _build_non_winning_message(message_to_send, correct_digits,
                               regular_digits);
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
    losers++;
    return false;
  } else {
    message_to_send += INVALID_NUMBER_MESSAGE;
    return true;
  }
}





//Stores in message_to_send the message that corresponds to the received guess
//Returns true if the game should continue, false otherwise
bool ClientProcessor::_process_guessed_number(
                                        std::string& message_to_send,
                                        const std::string& number_to_guess,
                                        uint16_t guessed_number,
                                        int& current_number_of_guesses){
  current_number_of_guesses++;
  message_to_send = "";
  std::string guessed_number_string = std::to_string(guessed_number);
  if ((guessed_number_string.length() != NUMBERS_DIGITS_AMMOUNT) ||
      (_has_repeated_digits(guessed_number_string))) {
    return _store_invalid_number_answer_message(message_to_send,
                                                current_number_of_guesses);
  }
  int correct_digits, regular_digits;
  _calculate_score(guessed_number_string, correct_digits, regular_digits);
  return _store_normal_answer_message(message_to_send,
                                      current_number_of_guesses,
                                      correct_digits, regular_digits);
}

//Sends a string to the client
void ClientProcessor::_send_built_message(const std::string&& message) const{
  uint32_t message_len = message.length();
  message_len = htonl(message_len);
  client.send(&message_len, 4);
  client.send(message.data(), message.length());
  client.send("", 1);
}


//Executes the help command, sending the help message to the client
void ClientProcessor::_execute_help() const{
  _send_built_message(HELP_MESSAGE);
}


//Executes the give up command, sending the losing message to the client
void ClientProcessor::_execute_give_up() const{
  losers++;
  _send_built_message(LOSE_MESSAGE);
}


//Sends the client the appropiate response corresponding to the received number
//Returns true if the program should continue running, otherwise returns false
bool ClientProcessor::_execute_number(int& current_number_of_guesses){
  std::string message_to_send;
  uint16_t guessed_number;
  uint32_t answer_message_len;
  bool should_game_continue;
  client.receive(&guessed_number, 2);
  guessed_number = ntohs(guessed_number);
  should_game_continue = _process_guessed_number(message_to_send,
                                                 number_to_guess,
                                                 guessed_number,
                                                 current_number_of_guesses);
  answer_message_len = message_to_send.length();
  answer_message_len = htonl(answer_message_len);
  client.send(&answer_message_len, 4);
  client.send(message_to_send.data(), message_to_send.length());
  client.send("", 1);
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
      throw(std::invalid_argument("Non existant command"));
  }
}

//Executes the game for the client
void ClientProcessor::_run_game(){
  int current_number_of_guesses = 0;
  bool should_continue = true;
  char command_indicator;
  while (should_continue) {
    client.receive(&command_indicator, 1);
    try {
      should_continue = _execute_command(command_indicator,
                                         current_number_of_guesses);
    } catch(std::invalid_argument& e) {
      _send_built_message(INVALID_COMMAND_MESSAGE);
    }
  }
  has_program_ended = true;
}


///////////////////////////////PUBLIC//////////////////////////

void ClientProcessor::join(){
  thrd.join();
}

bool ClientProcessor::has_ended() const{
  return has_program_ended;
}

ClientProcessor::ClientProcessor(PeerSocket&& peer_socket,
                                 const std::string& number_to_guess,
                                 std::atomic<size_t>& winners,
                                 std::atomic<size_t>& losers):
                                 number_to_guess(number_to_guess),
                                 client(std::move(peer_socket)),
                                 has_program_ended(false),
                                 winners(winners), losers(losers),
                                 thrd(&ClientProcessor::_run_game, this){
}

ClientProcessor::~ClientProcessor(){
  if (thrd.joinable()) {
    thrd.join();
  }
}
