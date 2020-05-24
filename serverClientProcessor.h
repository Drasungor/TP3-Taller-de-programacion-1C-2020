#ifndef _SERVER_CLIENT_PROCESSOR_H_
#define  _SERVER_CLIENT_PROCESSOR_H_

#include <thread>
#include <cstdint>
#include <string>
#include "serverPeerSocket.h"

class ClientProcessor {
private:
  const std::string number_to_guess;
  PeerSocket client;
  std::thread thrd;
  //int current_number_of_guesses;
private:
  void _run_game();
  //DESPUES DE HACER EL SOCKET EN SERIO AGREGAR EL CONST DEVUELTA
  bool _execute_command(char command_indicator,
                        int& current_number_of_guesses);
  //DESPUES DE HACER EL SOCKET EN SERIO AGREGAR EL CONST DEVUELTA
  bool _execute_number(int& current_number_of_guesses);
  //DESPUES DE HACER EL SOCKET EN SERIO AGREGAR EL CONST DEVUELTA
  void _execute_give_up();
  //DESPUES DE HACER EL SOCKET EN SERIO AGREGAR EL CONST DEVUELTA
  void _execute_help();
  bool _process_guessed_number(std::string& message_to_send
                               const std::string& number_to_guess,
                               uint16_t guessed_number,
                               int& taken_turns) const;
  bool _has_repeated_digits(const std::string& number_string) const;
public:
  //VER SI SE PASA UNA REFERENCIA O SI SE GUARDA EL INT EN EL ClientProcessor
  //ClientProcessor(PeerSocket& client, uint16_t& number_to_guess);

  ClientProcessor(PeerSocket& client, const std::string& number_to_guess);

  ClientProcessor(ClientProcessor&& other) noexcept;

  ClientProcessor(const ClientProcessor& other) = delete;

  ClientProcessor& operator=(const ClientProcessor& other) = delete;

  //ClientProcessor& operator=(ClientProcessor&& other) noexcept;

  ~ClientProcessor();

  //This operator starts the execution of the client's game, this allows the
  //user to start the execution at any moment and not instead of its
  //constrcutor
  void operator()();
};

#endif
