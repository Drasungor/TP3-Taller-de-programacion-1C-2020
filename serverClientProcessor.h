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
  std::atomic<size_t>& winners;
  std::atomic<size_t>& losers;
  //std::atomic<bool> has_player_won;
  std::thread thrd;
  
private:
  void _run_game();
  void _build_non_winning_message(std::string& message_to_send,
                                  int correct_digits, int regular_digits);
  void _send_built_message(const std::string&& message) const;
  bool _execute_command(char command_indicator,
                        int& current_number_of_guesses);
  bool _execute_number(int& current_number_of_guesses);
  void _execute_give_up() const;
  void _execute_help() const;
  bool _store_invalid_number_answer_message(
                                          std::string& message_to_send,
                                          int current_number_of_guesses) const;
  bool _store_normal_answer_message(
                                std::string& message_to_send,
                                int current_number_of_guesses,
                                int correct_digits, int regular_digits);
  void _calculate_score(const std::string guessed_number_string,
                        int& correct_digits, int& regular_digits) const;
  bool _process_guessed_number(std::string& message_to_send,
                               const std::string& number_to_guess,
                               uint16_t guessed_number, int& taken_turns);
  bool _has_repeated_digits(const std::string& number_string) const;

public:
  //Stores a copy of the received string and moves the received client so that
  //this ClientProcessor is now the owner
  //ClientProcessor(PeerSocket&& client, const std::string& number_to_guess);
  ClientProcessor(PeerSocket&& client, const std::string& number_to_guess,
                  std::atomic<size_t>& winners, std::atomic<size_t>& losers);

  ClientProcessor(const ClientProcessor& other) = delete;

  ClientProcessor& operator=(const ClientProcessor& other) = delete;

  ~ClientProcessor();

  //Waits for the program to end
  void join();

  //The value of this function is only valid if the process has ended
  //Returns true if the client won, otherwise returns false
  //bool did_client_win() const;

  //Returns true if the thread has finished executing the received function,
  //otherwise returns false
  bool has_ended() const;
};

#endif
