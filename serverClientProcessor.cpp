#include "serverClientProcessor.h"

#include <cstdint>

#define HELP_MESSAGE "Comandos válidos:\n\tAYUDA: despliega la lista de "
                     "comandos válidos\n\tRENDIRSE: pierde el juego "
                     "automáticamente\n\tXXX: número de 3 cifras a ser "
                     "enviado al servidor para adivinar el número secreto"
#define LOOSE_MESSAGE "Perdiste"
#define WIN_MESSAGE "Ganaste"
#define INVALID_COMMAND "Error: comando inválido. Escriba AYUDA para obtener "
                        "ayuda"
#define INVALID_NUMBER_MESSAGE "Número inválido. Debe ser de 3 cifras no "
                               "repetidas"


#define GOOD_GUESS_MESSAGE_PART " bien"
#define REGULAR_GUESS_MESSAGE_PART " regular"
#define BAD_GUESS_MESSAGE_PART " mal"

//This function returns if the string has a repeated digit
bool ClientProcessor::_has_repeated_digits(std::string& number_string){
  char current_digit;
  for (size_t i = 0; i < number_string.length(); i++) {
    for (size_t j = 0; j < number_string.length(); j++) {
      if (i != j) {
        if (number_string[i] == number_string[j]) {
          return true;
        }
      }
    }
  }
  return false;
}

void ClientProcessor::_process_guessed_number(std::string& message_to_send, uint16_t guessed_number){
  std::string number_string = std::to_string(guessed_number);
  if ((number_string.length() != NUMBERS_DIGITS_AMMOUNT) ||
       ) {
    message_to_send = ;
  }
}

void ClientProcessor::_execute_help(){
  client.send(HELP_MESSAGE);
}


void ClientProcessor::_execute_give_up(){
  client.send(LOOSE_MESSAGE);
}

void ClientProcessor::_execute_number(/*uint16_t guessed_number*/){
  std::string message_to_send;
  uint16_t guessed_number;
  client.receive(&guessed_number, sizeof(uint16_t));

  //DESCOMENTAR ESTO CUANDO SE EMPIECE A USAR EL SOCKET
  //guessed_number = ntohs(guessed_number);

  //AGREGAR CHEQUEO PARA VER SI EL NUMERO ESTA EN EL RANGO APROPIADO


  client.send(WIN_MESSAGE);
}

void ClientProcessor::_execute_command(char command_indicator){
  switch (command_indicator) {
    case COMMAND_INDICATOR_HELP:
      break;
    case COMMAND_INDICATOR_GIVE_UP:
      break;
    case COMMAND_INDICATOR_NUMBER:
      break;
    default:
      //VER SI TIRO UNA EXCEPTION
      break;
  }
}


//void ClientProcessor::_run_game(uint16_t& number_to_guess){
void ClientProcessor::_run_game(std::string& number_to_guess){

}


///////////////////////////////PUBLIC//////////////////////////

void ClientProcessor::operator()(){
  std::thread aux_thread();
}

//ClientProcessor::ClientProcessor(PeerSocket& peer_socket, int& number_to_guess){
ClientProcessor::ClientProcessor(PeerSocket& peer_socket, string& number_to_guess){
  //AGREGAR MOVE DEL PeerSocket
  current_number_of_guesses = 0;
}

ClientProcessor::~ClientProcessor(){
}
