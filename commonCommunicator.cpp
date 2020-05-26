#include "commonCommunicator.h"

#include <cstddef>
#include <string>

void Communicator::open_communication_channel(){
  if (host == "") {
    allow_communication(service.data(), NULL);
  } else {
    allow_communication(service.data(), host.data());
  }
}


Communicator::Communicator(const std::string service,
                           const std::string host/* = ""*/) noexcept{
  this->service = service;
  this->host = host;
}
Communicator::~Communicator(){
}
