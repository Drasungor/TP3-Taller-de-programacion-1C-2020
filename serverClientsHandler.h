#ifndef _SERVER_CLIENTS_HANDLER_H_
#define  _SERVER_CLIENTS_HANDLER_H_

#include <thread>
#include <atomic>
#include <vector>

class ClientsHandler {
private:
  std::atomic<bool> keep_running;
  std::thread thrd;
public:
  ClientsHandler(const std::string& service, const std::vector<std::string>& numbers_to_guess);

  ~ClientsHandler();

  //CREAR FUNCION QUE RECIBE REFERENCIAS A INTS Y GUARDA LA CANTIDAD DE GANADORES Y DE PERDEDORES
};

#endif
