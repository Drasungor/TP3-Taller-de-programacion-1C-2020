#ifndef _SERVER_CLIENTS_HANDLER_H_
#define  _SERVER_CLIENTS_HANDLER_H_

#include <thread>
#include <atomic>

class ClientsHandler {
private:
  std::atomic<bool> keep_running;
  std::thread thrd;
public:
  ClientsHandler();

  ~ClientsHandler();

  //int execute(const char** arguments, int number_of_arguments);
};

#endif
