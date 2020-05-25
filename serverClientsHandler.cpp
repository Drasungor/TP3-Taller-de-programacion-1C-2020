#include "serverClientsHandler.h"

#include <list>
#include <algorithm>
#include <system_error>
#include "serverSocket.h"
#include "serverClientProcessor.h"
#include "serverPeerSocket.h"
#include "serverShouldBeRemoved.h"



void ClientsHandler::_erase_dead_clients(std::list<ClientProcessor*>& clients){
  ShouldBeRemoved _should_be_removed(this->winners, this->losers);
  clients.erase(std::remove_if(clients.begin(), clients.end(), _should_be_removed), clients.end());
}


void ClientsHandler::_run_program(
                            const std::string& service,
                            const std::vector<std::string>& numbers_to_guess){
  ServerSocket server_socket;
  std::list<ClientProcessor*> clients;
  size_t i = 0;
  try {
    server_socket.bind_and_listen(service);
  } catch(std::system_error& e) {
    //HACER ALGO CON LA EXCEPTION PARA QUE NO CRASHEE EL PROGRAMA
  }
  while (keep_running) {
    try {
      clients.push_back(new ClientProcessor(std::move(server_socket.accept()),
                        numbers_to_guess[i]));
    } catch(std::system_error& e) {
      //CHEQUEAR keep_running PARA VER SI ES UN ERROR O SI FALLO PORQUE
      //CERRARON EL SERVER
      keep_running = false;
    }
    i++;
    if (i == numbers_to_guess.size()) {
      i = 0;
    }
  }

  //VER SI SE PASA AL DESTRUCTOR
  for (std::list<ClientProcessor*>::iterator it = clients.begin(); it != clients.end(); ++it) {
    (*it)->join();
    if ((*it)->did_client_win()) {
      winners++;
    } else {
      losers++;
    }
    delete(*it);
  }
}


///////////////////////////////PUBLIC//////////////////////////

void ClientsHandler::shutdown(){
  keep_running = false;
}

void ClientsHandler::wait_for_results(size_t& winners, size_t& losers){
  thrd.join();
  winners = this->winners;
  losers = this->losers;
}


ClientsHandler::ClientsHandler(
        const std::string& service,
        const std::vector<std::string>& numbers_to_guess):
        keep_running(true),
        thrd(&ClientsHandler::_run_program, this, service, numbers_to_guess){
  winners = 0;
  losers = 0;
}

ClientsHandler::~ClientsHandler(){
}
