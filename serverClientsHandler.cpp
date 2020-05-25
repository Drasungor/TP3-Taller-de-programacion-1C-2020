#include "serverClientsHandler.h"

#include <list>
#include <algorithm>
#include <system_error>
#include "serverSocket.h"
#include "serverClientProcessor.h"
#include "serverPeerSocket.h"
#include "serverShouldBeRemoved.h"

//void ClientsHandler::_erase_dead_clients(std::list<std::shared_ptr<ClientProcessor>>& clients, int& winners, int& losers) const{
void ClientsHandler::_erase_dead_clients(std::list<ClientProcessor*>& clients, size_t& winners, size_t& losers) const{
  ShouldBeRemoved _should_be_removed(winners, losers);
  clients.erase(std::remove_if(clients.begin(), clients.end(), _should_be_removed), clients.end());
}


void ClientsHandler::_run_program(const std::string& service, const std::vector<std::string>& numbers_to_guess){
  ServerSocket server_socket;
  std::list<ClientProcessor*> clients;
  //std::list<std::shared_ptr<ClientProcessor>> clients;
  size_t i = 0, winners = 0, losers = 0;
  try {
    server_socket.bind_and_listen(service);
  } catch(std::system_error& e) {
    //HACER ALGO CON LA EXCEPTION PARA QUE NO CRASHEE EL PROGRAMA
  }
  while (keep_running) {
    try {
      //ClientProcessor aux(std::move(server_socket.accept()), numbers_to_guess[i]);
      //clients.emplace_back(std::move(aux));
      //clients.emplace_back(new ClientProcessor(std::move(server_socket.accept()), numbers_to_guess[i]));
      clients.push_back(new ClientProcessor(std::move(server_socket.accept()), numbers_to_guess[i]));
    } catch(std::system_error& e) {
      //CHEQUEAR keep_running PARA VER SI ES UN ERROR O SI FALLO PORQUE
      //CERRARON EL SERVER
    }
    i++;
    if (i == numbers_to_guess.size()) {
      i = 0;
    }

  }

  //VER SI SE PASA AL DESTRUCTOR
  for (std::list<ClientProcessor*>::iterator it = clients.begin(); it != clients.end(); ++it) {
    delete(*it);
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
