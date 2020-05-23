#include "serverClientProcessor.h"

#include <cstdint>
#include <string>
#include "serverPeerSocket.h"

#define HELP_MESSAGE_PART_1 "Comandos válidos:\n\tAYUDA: despliega la lista "
#define HELP_MESSAGE_PART_2 "de comandos válidos\n\tRENDIRSE: pierde el juego "
#define HELP_MESSAGE_PART_3 "automáticamente\n\tXXX: número de 3 cifras a ser "
#define HELP_MESSAGE_PART_4 "enviado al servidor para adivinar el número "
#define HELP_MESSAGE_PART_5 "secreto"

#define LOSE_MESSAGE "Perdiste"
#define WIN_MESSAGE "Ganaste"

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
  std::string number_string = std::to_string(guessed_number);
  if ((number_string.length() != NUMBERS_DIGITS_AMMOUNT) ||
      (_has_repeated_digits(number_string))) {
    //message_to_send = INVALID_NUMBER_MESSAGE;
    return INVALID_NUMBER_MESSAGE;
  }
  std::string message_to_send;
  int correct_digits = 0, regular_digits = 0;
  //PASAR ESTE DOBLE FOR A UNA FUNCION A APRTE
  for (size_t i = 0; i < NUMBERS_DIGITS_AMMOUNT; i++) {
    for (size_t j = 0; j < NUMBERS_DIGITS_AMMOUNT; j++) {
      if (number_to_guess[i] == guessed_number[j]) {
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
  current_turn++;
  return message_to_send;
}

void ClientProcessor::_execute_help() const{
  client.send(HELP_MESSAGE);
}


void ClientProcessor::_execute_give_up() const{
  client.send(LOOSE_MESSAGE);
}

//ESTO ES PARA PROBAR POR AHORA EL PROGRAMA, VER SI TIENE QUE DEVOLVER VOID
//Returns if the program should end
bool ClientProcessor::_execute_number(int& current_number_of_guesses) const{
  std::string message_to_send;
  uint16_t guessed_number;
  client.receive(&guessed_number, sizeof(uint16_t));

  //DESCOMENTAR ESTO CUANDO SE EMPIECE A USAR EL SOCKET
  //guessed_number = ntohs(guessed_number);

  //AGREGAR CHEQUEO PARA VER SI EL NUMERO ESTA EN EL RANGO APROPIADO
  message_to_send = _process_guessed_number(number_to_guess, guessed_number,
                                            current_number_of_guesses);
  client.send(message_to_send);
  //AGREGAR CODIGO PARA TERMINAR EL JUEGO CUANDO EL CLIENTE GANE

  //BORRAR ESTO, ES SOLO PARA VER SI ANDA
  if (message_to_send == WIN_MESSAGE) {
    return true;
  }
  return false;
}


//Returns if the program should end
void ClientProcessor::_execute_command(char command_indicator,
                                       int& current_number_of_guesses) const{
  switch (command_indicator) {
    case COMMAND_INDICATOR_HELP:
      return false;
    case COMMAND_INDICATOR_GIVE_UP:
      return true;
    case COMMAND_INDICATOR_NUMBER:
      return _execute_number(current_number_of_guesses);
    default:
      //MANDAR MENSAJE DE ERROR
      return false;
  }
}


//void ClientProcessor::_run_game(uint16_t& number_to_guess){
void ClientProcessor::_run_game() const{
  int current_number_of_guesses = 0;
  bool keep_running = true;
  char command;
  client.receive(&command, sizeof(char));
  while (keep_running) {
    keep_running = _execute_command(command_indicator,
                                    current_number_of_guesses);
  }
}


///////////////////////////////PUBLIC//////////////////////////

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
                                 client(peer_socket){
  //AGREGAR MOVE DEL PeerSocket
  //current_number_of_guesses = 0;
}

ClientProcessor::ClientProcessor(ClientProcessor&& other) noexcept:
                            number_to_guess(std::move(other.numbers_to_guess)),
                            client(std::move(other.client)){
  //const std::string& number_to_guess;
  //const PeerSocket client;
  std::thread thrd = std::move(other.thrd);
}

ClientProcessor::~ClientProcessor(){
}
