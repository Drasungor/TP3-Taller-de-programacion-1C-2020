#include "commonCommunicator.h"

#include <cstddef>

void Communicator::open_communication_channel(){
  if (host == "") {
    allow_communication(service.data(), NULL);
  } else {
    allow_communication(service.data(), host.data());
  }
}

/*
Communicator::Communicator(const char* service, const char* host = NULL) noexcept:
                          service(service), host(host){
  this->service = service;
  this->host = host;
}
*/
Communicator::Communicator(const std::string service, const std::string host/* = ""*/) noexcept{
  this->service = service;
  this->host = host;
}
Communicator::~Communicator(){
}
