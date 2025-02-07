#ifndef _COMMON_COMMUNICATOR_H_
#define  _COMMON_COMMUNICATOR_H_

#include <string>

//This is an abstract class that implements the methods necessary for the
//establishment of communication between a client socket and a server socket
//Any failure will cause a system_error exception to be thrown due to the
//usage of sockets
class Communicator {
private:
  std::string service;
  std::string host;

public:
  //The implemented function should establish the desired type of communication
  //(as client or as server)
  virtual void allow_communication(const char* service, const char* host) = 0;

  //An empty string host indicates localhost is used
   explicit Communicator(const std::string service,
                         const std::string host = "") noexcept;

  virtual ~Communicator();

  //Establishes the communication implemented by allow_communication
  void open_communication_channel();
};

#endif
