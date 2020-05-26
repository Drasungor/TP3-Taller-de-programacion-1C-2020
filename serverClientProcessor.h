#ifndef _SERVER_CLIENT_PROCESSOR_H_
#define  _SERVER_CLIENT_PROCESSOR_H_

#include <thread>
#include <cstdint>
#include <string>
#include <atomic>
#include "serverPeerSocket.h"

class ClientProcessor {
private:
  const std::string number_to_guess;
  PeerSocket client;
  std::atomic<bool> has_program_ended;
  std::atomic<bool> has_player_won;
  std::thread thrd;

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
  bool _store_invalid_number_answer_message(
                                          std::string& message_to_send,
                                          int current_number_of_guesses) const;
  bool _store_normal_answer_message(std::string& message_to_send,
                                    int current_number_of_guesses,
                                    int correct_digits, int regular_digits);
  void _calculate_score(const std::string guessed_number_string,
                        int& correct_digits, int& regular_digits) const;
  bool _process_guessed_number(std::string& message_to_send,
                               const std::string& number_to_guess,
                               uint16_t guessed_number, int& taken_turns);
  bool _has_repeated_digits(const std::string& number_string) const;

public:
  //VER SI SE PASA UNA REFERENCIA O SI SE GUARDA EL INT EN EL ClientProcessor
  //ClientProcessor(PeerSocket& client, uint16_t& number_to_guess);

  //AVISAR QUE VA A HACER UN MOVE DEL PeerSocket
  ClientProcessor(PeerSocket&& client, const std::string& number_to_guess);

  ClientProcessor(ClientProcessor&& other) noexcept;

  ClientProcessor(const ClientProcessor& other) = delete;

  ClientProcessor& operator=(const ClientProcessor& other) = delete;

  //ClientProcessor& operator=(ClientProcessor&& other) noexcept;

  ~ClientProcessor();

  //This operator starts the execution of the client's game, this allows the
  //user to start the execution at any moment and not instead of its
  //constrcutor
  //void operator()();

  //VER SI HAY QUE BORRAR ESTA FUNCION Y SE LLAMA A JOIN SOLO EN EL DESTRUCTOR
  //Returns true if the player has won, false it he lost, if there is an error
  //that prevents the game from being played then it will be counted as a
  //player loss
  //bool join();
  void join();

  //The value of this function is only valid if the process has ended
  //Returns true y the client won, false otherwise
  bool did_client_win() const;

  //Returns true if the thread has finished executing the received function,
  //otherwise returns false
  bool has_ended() const;
};

#endif
