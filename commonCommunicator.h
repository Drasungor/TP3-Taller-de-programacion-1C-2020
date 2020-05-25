#ifndef _COMMON_COMMUNICATOR_H_
#define  _COMMON_COMMUNICATOR_H_

#include <string>

//This is an abstract class that implements the methods necessary for the
//establishment of communication between a client socket and a peer socket
//Any failure will cause a ios_base::failure exception to be thrown due to the
//stream and io managing nature of the class
class Communicator {
private:
  /*
  const char* service;
  const char* host;
  */
  std::string service;
  std::string host;
//protected:
public:
  virtual void allow_communication(const char* service, const char* host) = 0;

  //Communicator(const char* service, const char* host = NULL) noexcept;
  Communicator(const std::string service, const std::string host = "") noexcept;

  virtual ~Communicator();

  void open_communication_channel();

};

#endif
