#ifndef _SERVER_CLIENTS_HANDLER_H_
#define  _SERVER_CLIENTS_HANDLER_H_


#include <list>
#include <thread>
#include <atomic>
#include <vector>
#include <string>
#include "serverSocket.h"
#include "serverClientProcessor.h"


class ClientsHandler {
private:
  std::atomic<bool> keep_running;
  ServerSocket server_socket;
  std::thread thrd;
  size_t winners;
  size_t losers;

private:
  //void _erase_dead_clients(std::list<ClientProcessor*>& clients);
  void _erase_dead_clients(std::list<std::shared_ptr<ClientProcessor>>& clients);
  void _run_program(const std::string& service,
                    const std::vector<std::string>& numbers_to_guess);

public:
  ClientsHandler(const std::string& service,
                 const std::vector<std::string>& numbers_to_guess);

  ~ClientsHandler();

  //Kills the servers program, waiting for all the clients sessions to finish
  void shutdown();

  //This function should only be called after a shutdown
  void wait_for_results(size_t& winners, size_t& losers);
};

#endif
