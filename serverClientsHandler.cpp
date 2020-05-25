#include "serverClientsHandler.h"

#include <list>
#include "serverClientProcessor.h"
#include "serverPeerSocket.h"
#include "serverShouldBeRemoved.h"

ClientsHandler::_erase_dead_clients(std::list<ClientProcessor>& clients) const{

}


ClientsHandler::_run_program(const std::string& service, const std::vector<std::string>& numbers_to_guess){
  ServerSocket server_socket;
  std::list<ClientProcessor> clients;
  int i = 0, winners = 0, losers = 0;
  try {
    server_socket.bind_and_listen(service);
  } catch(std::system_error& e) {
    //HACER ALGO CON LA EXCEPTION PARA QUE NO CRASHEE EL PROGRAMA
  }
  while (keep_running) {
    try {
      ClientProcessor aux(server_socket.accept(), numbers_to_guess[i]);
      clients.emplace_back(std::move(aux));
    } catch(std::system_error& e) {
      //CHEQUEAR keep_running PARA VER SI ES UN ERROR O SI FALLO PORQUE
      //CERRARON EL SERVER
    }
    i++;
    if (i == numbers_to_guess.size()) {
      i = 0;
    }

  }
}


///////////////////////////////PUBLIC//////////////////////////


ClientsHandler::ClientsHandler(
                            const std::string& service,
                            const std::vector<std::string>& numbers_to_guess):
                            keep_running(true){
  //ACA SE TIRA EL THREAD EN LA INITIALIZATION LIST
}

ClientsHandler::~ClientsHandler(){
}
