#include "serverClientProcessor.h"

#include <cstdint>
#include <string>
#include <cstring>
#include "serverPeerSocket.h"
#include "CommunicationConstants.h"


//BORRAR ESTE INCLUDE
#include <iostream>

#define HELP_MESSAGE_PART_1 "Comandos válidos:\n\tAYUDA: despliega la lista "
#define HELP_MESSAGE_PART_2 "de comandos válidos\n\tRENDIRSE: pierde el juego "
#define HELP_MESSAGE_PART_3 "automáticamente\n\tXXX: número de 3 cifras a ser "
#define HELP_MESSAGE_PART_4 "enviado al servidor para adivinar el número "
#define HELP_MESSAGE_PART_5 "secreto\n\n" //<-- BORRAR ESTOS \n

#define INVALID_COMMAND_MESSAGE_PART_1 "Error: comando inválido. Escriba "
#define INVALID_COMMAND_MESSAGE_PART_2 "AYUDA para obtener ayuda"

#define INVALID_NUMBER_MESSAGE_PART_1 "Número inválido. Debe ser de 3 cifras "
#define INVALID_NUMBER_MESSAGE_PART_2 "no repetidas"


#define GOOD_GUESS_MESSAGE_PART " bien"
#define REGULAR_GUESS_MESSAGE_PART " regular"
#define BAD_GUESS_MESSAGE_PART " mal"

#define MAX_NUMBER_OF_TRIES 10

//This function returns if the string (of any size) has a repeated digit
bool ClientProcessor::_has_repeated_digits(const std::string& number_string) const{
  /*
  for (size_t i = 0; i < number_string.length(); i++) {
    for (size_t j = 0; j < number_string.length(); j++) {
      if (i != j) {
        if (number_string[i] == number_string[j]) {
          return true;
        }
      }
    }
  }
  */
  for (size_t i = 0; i < number_string.length() - 1; i++) {
    for (size_t j = i + 1; j < number_string.length(); j++) {
      if (number_string[i] == number_string[j]) {
        return true;
      }
    }
  }
  return false;
}

//VER SI SE DEVUELVE VOID Y SE MANDA DIRECTAMENTE ACA EL MENSAJE
std::string ClientProcessor::_process_guessed_number(
                                            const std::string& number_to_guess,
                                            uint16_t guessed_number,
                                            int& taken_turns) const{
  taken_turns++;
  std::string message_to_send = "";
  std::string guessed_number_string = std::to_string(guessed_number);
  if ((guessed_number_string.length() != NUMBERS_DIGITS_AMMOUNT) ||
      (_has_repeated_digits(guessed_number_string))) {
    message_to_send += INVALID_NUMBER_MESSAGE_PART_1;
    message_to_send += INVALID_NUMBER_MESSAGE_PART_2;
    return message_to_send;
  }
  int correct_digits = 0, regular_digits = 0;

  //PASAR ESTE DOBLE FOR A UNA FUNCION A APRTE, CHEQUEA LA CANTIAD DE
  //CARACTERES BUENOS Y REGULARES
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
  if (correct_digits == NUMBERS_DIGITS_AMMOUNT) {
    message_to_send = WIN_MESSAGE;
  } else {
    //VER SI LA LOGICA DE LOSE_MESSAGE SE SACA DE ESTA FUNCION Y SE
    //PASA A UN FOR AFUERA
    if (taken_turns == MAX_NUMBER_OF_TRIES) {
      message_to_send = LOSE_MESSAGE;
    } else {
      if ((correct_digits == 0) && (regular_digits == 0)) {
        message_to_send = std::to_string(NUMBERS_DIGITS_AMMOUNT) + BAD_GUESS_MESSAGE_PART;
      } else {
        message_to_send = std::to_string(correct_digits) + GOOD_GUESS_MESSAGE_PART
                          + " " + std::to_string(regular_digits) +
                          REGULAR_GUESS_MESSAGE_PART;
      }
    }
  }
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
  return message_to_send;
}

void ClientProcessor::_execute_help(){
  client.send(HELP_MESSAGE_PART_1, std::strlen(HELP_MESSAGE_PART_1));
  client.send(HELP_MESSAGE_PART_2, std::strlen(HELP_MESSAGE_PART_2));
  client.send(HELP_MESSAGE_PART_3, std::strlen(HELP_MESSAGE_PART_3));
  client.send(HELP_MESSAGE_PART_4, std::strlen(HELP_MESSAGE_PART_4));
  client.send(HELP_MESSAGE_PART_5, std::strlen(HELP_MESSAGE_PART_5));
}


void ClientProcessor::_execute_give_up(){
  client.send(LOSE_MESSAGE, std::strlen(LOSE_MESSAGE));
}

//ESTO ES PARA PROBAR POR AHORA EL PROGRAMA, VER SI TIENE QUE DEVOLVER VOID
//Returns if the program should end
//DESPUES DE HACER EL SOCKET EN SERIO AGREGAR EL CONST DEVUELTA
bool ClientProcessor::_execute_number(int& current_number_of_guesses){
  std::string message_to_send;
  uint16_t guessed_number;
  client.receive(&guessed_number, sizeof(uint16_t));

  //DESCOMENTAR ESTO CUANDO SE EMPIECE A USAR EL SOCKET
  //guessed_number = ntohs(guessed_number);

  //BORRAR PRINT
  std::cout << "ESTOY EN EXECUTE NUMBER" << std::endl;

  //AGREGAR CHEQUEO PARA VER SI EL NUMERO ESTA EN EL RANGO APROPIADO
  message_to_send = _process_guessed_number(number_to_guess, guessed_number,
                                            current_number_of_guesses);
  client.send(message_to_send.data(), message_to_send.length());

  std::cout << "CANTIDAD DE PRUEBAS ACTUALES: " << current_number_of_guesses <<"\n\n";

  //AGREGAR CODIGO PARA TERMINAR EL JUEGO CUANDO EL CLIENTE GANE

  //BORRAR ESTO, ES SOLO PARA VER SI ANDA
  if ((message_to_send == WIN_MESSAGE) || (message_to_send == LOSE_MESSAGE)) {
    return true;
  }
  return false;
}


//Returns if the program should end
bool ClientProcessor::_execute_command(char command_indicator,
                                       int& current_number_of_guesses){
  switch (command_indicator) {
    case COMMAND_INDICATOR_HELP:
      _execute_help();
      return false;
    case COMMAND_INDICATOR_GIVE_UP:
    _execute_give_up();
      return true;
    case COMMAND_INDICATOR_NUMBER:
      return _execute_number(current_number_of_guesses);
    default:
      //MANDAR MENSAJE DE ERROR
      return false;
  }
}


//void ClientProcessor::_run_game(uint16_t& number_to_guess){
//VER SI HAY QUE AGREGAR CONST
void ClientProcessor::_run_game(){
  int current_number_of_guesses = 0;
  bool should_stop = false;
  char command_indicator;
  while (!should_stop) {

    client.receive(&command_indicator, sizeof(char));

    should_stop = _execute_command(command_indicator,
                                    current_number_of_guesses);
  }
}


///////////////////////////////PUBLIC//////////////////////////

//VER SI HAY QUE AGREGAR CONST
void ClientProcessor::operator()(){
  //std::thread aux_thread(/*TIRAR DESPUES THREAD CON ARGUMENTOS*/);

  //LLAMAR A ESTA FUNCION EN EL THREAD
  _run_game();
}

/*
ClientProcessor& ClientProcessor::operator=(ClientProcessor&& other) noexcept{

}
*/

//ClientProcessor::ClientProcessor(PeerSocket& peer_socket, int& number_to_guess){
ClientProcessor::ClientProcessor(PeerSocket& peer_socket,
                                 const std::string& number_to_guess):
                                 number_to_guess(std::move(number_to_guess)),
                                 client(std::move(peer_socket)){
  //AGREGAR MOVE DEL PeerSocket
  //current_number_of_guesses = 0;
}

ClientProcessor::ClientProcessor(ClientProcessor&& other) noexcept:
                            number_to_guess(std::move(other.number_to_guess)),
                            client(std::move(other.client)){
  //const std::string& number_to_guess;
  //const PeerSocket client;
  std::thread thrd = std::move(other.thrd);
}

ClientProcessor::~ClientProcessor(){
}
