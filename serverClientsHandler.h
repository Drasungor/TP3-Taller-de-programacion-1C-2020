#ifndef _SERVER_CLIENTS_HANDLER_H_
#define  _SERVER_CLIENTS_HANDLER_H_


#include <list>
#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include <string>
#include "serverSocket.h"
#include "serverClientProcessor.h"


class ClientsHandler {
private:
  std::atomic<bool> keep_running;
  ServerSocket server_socket;
  std::atomic<size_t> winners;
  std::atomic<size_t> losers;
  std::thread thrd;

private:
  static bool _should_be_removed(
                              const std::shared_ptr<ClientProcessor>& client);
  void _close_clients(std::list<std::shared_ptr<ClientProcessor>>& clients);
  void _erase_dead_clients(
                        std::list<std::shared_ptr<ClientProcessor>>& clients);
  void _run_program(const std::string& service,
                    const std::vector<std::string>& numbers_to_guess);

public:
  ClientsHandler(const std::string& service,
                 const std::vector<std::string>& numbers_to_guess);

  ~ClientsHandler();

  //Shuts down the server and waits for the remaining clients to finish
  //executing their game session
  void operator()(size_t& winners, size_t& losers);
};

#endif
