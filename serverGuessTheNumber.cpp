#include "serverGuessTheNumber.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <string>
#include <cstdint>
#include "serverClientsHandler.h"
#include "CommunicationConstants.h"

#define SUCCESS 0
#define INVALID_FILE 1
#define WRONG_NUMBER_OF_ARGUMENTS 1
#define SOCKET_ERROR 1
#define OUT_OF_RANGE_NUMBER 1
#define INVALID_NUMBER 1

#define NUMBER_OF_ARGUMENTS 3
#define NUMBERS_FILE_ARGUMENTS_INDEX 2
#define SERVICE_ARGUMENTS_INDEX 1

#define OUTPUT_MESSAGE_FIRST_LINE "Estadísticas:"
#define OUTPUT_MESSAGE_PLAYER_PREFIX "\n\t"
#define OUTPUT_MESSAGE_WINNERS_TEXT "Ganadores:  "
#define OUTPUT_MESSAGE_LOSERS_TEXT "Perdedores: "

#define INVALID_ARGUMENTS_TEXT "Error: argumentos inválidos\n"
#define OUT_OF_RANGE_FILE_NUMBER_TEXT "Error: archivo con números fuera de "\
                                      "rango\n"
#define INVALID_FILE_NUMBER_TEXT "Error: formato de los números inválidos\n"

#define SOCKET_ERROR_TEXT "Error de comunicación de socket\n"
#define INVALID_FILE_TEXT "Error: archivo inexistente\n"

#define KILL_SERVER_INDICATOR "q"

#define MAX_DIGIT_ASCII_CODE 48
#define MIN_DIGIT_ASCII_CODE 57

//This function returns if the string (of any size) has a repeated digit
bool ServerGuessTheNumber::_has_repeated_digits(
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


bool ServerGuessTheNumber::_is_number(const std::string& number_string) const{
  for (size_t i = 0; i < number_string.length(); i++) {
    if ((number_string[i] < MIN_DIGIT_ASCII_CODE) ||
        (number_string[i] > MAX_DIGIT_ASCII_CODE)) {
      return false;
    }
  }
  return true;
}


//If a read string does not represent a number and is also of an undesired
//length, the same exception as the one with undesired length exception will
//be thrown
void ServerGuessTheNumber::_load_numbers_to_guess(
                            std::ifstream& numbers_file,
                            std::vector<std::string>& numbers_to_guess) const{
  std::string buffer;
  while (!numbers_file.eof()) {
    std::getline(numbers_file, buffer);
    if (buffer.length() != NUMBERS_DIGITS_AMMOUNT) {
      throw(std::domain_error("Number out of range"));
    }
    if (_has_repeated_digits(buffer)) {
      throw(std::invalid_argument("Repeated digits"));
    }
    if (_is_number(buffer)) {
      throw(std::invalid_argument("Not a number"));
    }
    numbers_to_guess.push_back(buffer);
  }
}


void ServerGuessTheNumber::_print_server_output(size_t winners, size_t losers){
  std::cout << OUTPUT_MESSAGE_FIRST_LINE << OUTPUT_MESSAGE_PLAYER_PREFIX
            << OUTPUT_MESSAGE_WINNERS_TEXT << winners <<
            OUTPUT_MESSAGE_PLAYER_PREFIX << OUTPUT_MESSAGE_LOSERS_TEXT <<
            losers << "\n";
}


//Starts running the servers responsibilities, waits for the close server
//command and prints the result of the program
void ServerGuessTheNumber::_run_server(const char* service,
                                  std::vector<std::string>& numbers_to_guess){
  ClientsHandler handler(service, numbers_to_guess);
  size_t winners = 0, losers = 0;
  std::string buffer;
  do {
    std::cin >> buffer;
  } while (buffer != KILL_SERVER_INDICATOR);
  handler(winners, losers);
  _print_server_output(winners, losers);
}


///////////////////////////////PUBLIC//////////////////////////

//This function cannot be reduced to 20 lines due to error checking
int ServerGuessTheNumber::execute(const char** arguments,
                                  int number_of_arguments){
  if (number_of_arguments != NUMBER_OF_ARGUMENTS) {
    std::cerr << INVALID_ARGUMENTS_TEXT;
    return WRONG_NUMBER_OF_ARGUMENTS;
  }
  std::ifstream numbers_file(arguments[NUMBERS_FILE_ARGUMENTS_INDEX]);
  if (!numbers_file.is_open()) {
    std::cerr << INVALID_FILE_TEXT;
    return INVALID_FILE;
  }
  std::vector<std::string> numbers_to_guess;
  try {
    _load_numbers_to_guess(numbers_file, numbers_to_guess);
  } catch(std::domain_error& e1) {
    std::cerr << OUT_OF_RANGE_FILE_NUMBER_TEXT;
    return OUT_OF_RANGE_NUMBER;
  } catch(std::invalid_argument& e2) {
    std::cerr << INVALID_FILE_NUMBER_TEXT;
    return INVALID_NUMBER;
  }
  _run_server(arguments[SERVICE_ARGUMENTS_INDEX], numbers_to_guess);
  return SUCCESS;
}


ServerGuessTheNumber::ServerGuessTheNumber(){
}


ServerGuessTheNumber::~ServerGuessTheNumber(){
}
