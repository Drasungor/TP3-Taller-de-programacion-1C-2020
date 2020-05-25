#include "serverGuessTheNumber.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdint>
#include <system_error>
#include "serverSocket.h"
#include "serverPeerSocket.h"
#include "serverClientProcessor.h"
//VER SI HAY QUE BORRAR ESTOS INCLUDE

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

#define INVALID_ARGUMENTS_TEXT "Error: argumentos inválidos\n"
#define OUT_OF_RANGE_FILE_NUMBER_TEXT "Error: archivo con números fuera de rango\n"
#define INVALID_FILE_NUMBER_TEXT "Error: formato de los números inválidos\n"

#define SOCKET_ERROR_TEXT "Error de comunicación de socket\n"
#define INVALID_FILE_TEXT "Error: archivo inexistente\n"

#define MAX_DIGIT_ASCII_CODE 48
#define MIN_DIGIT_ASCII_CODE 57

//This function returns if the string (of any size) has a repeated digit
bool ServerGuessTheNumber::_has_repeated_digits(const std::string& number_string) const{
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
    if ((number_string[i] < MIN_DIGIT_ASCII_CODE) || (number_string[i] > MAX_DIGIT_ASCII_CODE)) {
      return false;
    }
  }
  return true;
}


//AGREGAR DESCRIPCION DE LO QUE HACE LA FUNCION, AGREGAR CADA CASO DE EXCEPTION
//PORQUE SOLO SE DESCRIBE LA EXCEPTION TIRADA EN UN CASO PARTICULAR
//If a read string does not represent a number and is also of an undesired
//length, the same exception as the one with undesired length exception will
//be thrown VER SI ESTO ULTIMO HAY QUE CAMBIARLO, CREO QUE EL ENUNCIADO
//CONSIDERA EL CASO EN EL QUE EL ARCHIVO DE ENTRADA TIENE STRINGS QUE NO SON
//NUMEROS
void ServerGuessTheNumber::_load_numbers_to_guess(
                                  std::ifstream& numbers_file,
                                  std::vector<std::string>& numbers_to_guess) const{
  std::string buffer;
  std::getline(numbers_file, buffer);
  while (!numbers_file.eof()) {
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
    std::getline(numbers_file, buffer);
  }
}


///////////////////////////////PUBLIC//////////////////////////

int ServerGuessTheNumber::execute(const char** arguments, int number_of_arguments){


  //VER SI SE PASA TODO LO QUE PUEDE DEVOLVER UNA EXCEPCION A UNA FUNCION Y
  //TENER UN TRY CATCH QUE AGARRE TODAS LAS EXCEPCIONES POSIBLES E IMPRIMA UN
  //MENSAJE DE ERROR PARA CADA UNA
  if (number_of_arguments != NUMBER_OF_ARGUMENTS) {
    std::cout << INVALID_ARGUMENTS_TEXT;
    return WRONG_NUMBER_OF_ARGUMENTS;
  }

  std::ifstream numbers_file(arguments[NUMBERS_FILE_ARGUMENTS_INDEX]);

  //VER SI SE PASA TODO LO QUE PUEDE DEVOLVER UNA EXCEPCION A UNA FUNCION Y
  //TENER UN TRY CATCH QUE AGARRE TODAS LAS EXCEPCIONES POSIBLES E IMPRIMA UN
  //MENSAJE DE ERROR PARA CADA UNA
  if (!numbers_file.is_open()) {
    std::cout << INVALID_FILE_TEXT;
    return INVALID_FILE;
  }

  ServerSocket server_socket;
  PeerSocket peer_socket;
  try {
    server_socket.bind_and_listen(arguments[SERVICE_ARGUMENTS_INDEX]);
    peer_socket = std::move(server_socket.accept());
  } catch (std::system_error e){
    std::cout << SOCKET_ERROR_TEXT;
    return SOCKET_ERROR;
  }
  std::vector<std::string> numbers_to_guess;
  //bool keep_processing = true;
  try {
    _load_numbers_to_guess(numbers_file, numbers_to_guess);
  } catch(std::domain_error& e1) {
    std::cout << OUT_OF_RANGE_FILE_NUMBER_TEXT;
    return OUT_OF_RANGE_NUMBER;
  } catch(std::invalid_argument& e2) {
    std::cout << INVALID_FILE_NUMBER_TEXT;
    return INVALID_NUMBER;
  }

  //AGREGAR CODIGOS DE RETORNO DE ERROR
  //ClientProcessor processor(peer_socket, numbers_to_guess[0]);

  //processor();

  //processor.join();
  /*
  try {
    processor();
  } catch(std::system_error e) {
    std::cout << SOCKET_ERROR_TEXT;
    return SOCKET_ERROR;
  }
  */
  return SUCCESS;
}


ServerGuessTheNumber::ServerGuessTheNumber(){
}


ServerGuessTheNumber::~ServerGuessTheNumber(){
}
