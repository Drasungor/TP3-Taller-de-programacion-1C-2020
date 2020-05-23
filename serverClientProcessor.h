#ifndef _SERVER_CLIENT_PROCESSOR_H_
#define  _SERVER_CLIENT_PROCESSOR_H_

#include <thread>
#include <cstdint>
#include <string>
#include "serverPeerSocket.h"

class ClientProcessor {
private:
  const std::string number_to_guess;
  const PeerSocket client;
  std::thread thrd;
  //int current_number_of_guesses;
private:
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
