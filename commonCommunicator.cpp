#include "commonCommunicator.h"

#include <cstddef>

void Communicator::open_communication_channel(){
  allow_communication(service, host);
}


Communicator::Communicator(const char* service, const char* host/* = NULL*/) noexcept:
                          service(service), host(host){
/*
  this->service = service;
  this->host = host;
*/
}

Communicator::~Communicator(){
}
