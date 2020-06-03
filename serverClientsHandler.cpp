#include "serverClientsHandler.h"

#include <list>
#include <algorithm>
#include <system_error>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include "serverSocket.h"
#include "serverClientProcessor.h"
#include "serverPeerSocket.h"
//#include "serverShouldBeRemoved.h"

//Indicates if the client received should be erased from the clients list by
//checking if the game has finished
bool ClientsHandler::_should_be_removed(
                              const std::shared_ptr<ClientProcessor>& client){
  return client->has_ended();
}

void ClientsHandler::_erase_dead_clients(
                        std::list<std::shared_ptr<ClientProcessor>>& clients){
  //ShouldBeRemoved _should_be_removed/*(this->winners, this->losers)*/;
  clients.erase(std::remove_if(clients.begin(), clients.end(),
                _should_be_removed), clients.end());
}


//CAMBIAR NOMBRE DE LA FUNCION POR _close_clients o algo asi
void ClientsHandler::_close_clients(
                        std::list<std::shared_ptr<ClientProcessor>>& clients){
  for (std::list<std::shared_ptr<ClientProcessor>>::iterator it =
       clients.begin(); it != clients.end(); ++it) {
    (*it)->join();
    /*
    if ((*it)->did_client_win()) {
      winners++;
    } else {
      losers++;
    }
    */
  }
}

void ClientsHandler::_run_program(
                            const std::string& service,
                            const std::vector<std::string>& numbers_to_guess){
  std::list<std::shared_ptr<ClientProcessor>> clients;
  size_t i = 0;
  try {
    server_socket.open_communication_channel();
  } catch(std::system_error& e) {
    //If the server crashes then the clients have to force quit the program
    return;
  }
  while (keep_running) {
    try {
      clients.emplace_back(new ClientProcessor(
                                          std::move(server_socket.accept()),
                                          numbers_to_guess[i], winners,
                                          losers));
      _erase_dead_clients(clients);
    } catch(std::system_error& e) {
      keep_running = false;
    }
    i++;
    if (i == numbers_to_guess.size()) {
      i = 0;
    }
  }
  //_count_winners_and_losers(clients);
  _close_clients(clients);
}


///////////////////////////////PUBLIC//////////////////////////

/*
void ClientsHandler::shutdown(){
  server_socket.disconnect();
}

void ClientsHandler::wait_for_results(size_t& winners, size_t& losers){
  thrd.join();
  winners = this->winners;
  losers = this->losers;
}
*/

void ClientsHandler::operator()(size_t& winners, size_t& losers){
  server_socket.disconnect();
  thrd.join();
  winners = this->winners;
  losers = this->losers;
}




ClientsHandler::ClientsHandler(
        const std::string& service,
        const std::vector<std::string>& numbers_to_guess):
        keep_running(true),
        server_socket(service),
        winners(0), losers(0),
        thrd(&ClientsHandler::_run_program, this, service, numbers_to_guess){
  //winners = 0;
  //losers = 0;
}

ClientsHandler::~ClientsHandler(){
}
