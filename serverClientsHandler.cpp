#include "serverClientsHandler.h"

#include <list>
#include "serverClientProcessor.h"

ClientsHandler::_run_program(){
  ServerSocket server_socket;
  std::list<ClientProcessor> clients;
  while (keep_running) {
    
    ClientProcessor aux();
    clients.emplace_back();
  }
}


///////////////////////////////PUBLIC//////////////////////////


ClientsHandler::ClientsHandler(): keep_running(true){
  //ACA SE TIRA EL THREAD EN LA INITIALIZATION LIST
}

ClientsHandler::~ClientsHandler(){
}
