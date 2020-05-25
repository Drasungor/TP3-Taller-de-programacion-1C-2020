#ifndef _COMMON_COMMUNICATION_SOCKET_H_
#define  _COMMON_COMMUNICATION_SOCKET_H_


#include <cstddef>

//This is an abstract class that implements the methods necessary for the
//communication between a client socket and a peer socket
//Any failure will cause a ios_base::failure exception to be thrown due to the
//stream and io managing nature of the class
class ShouldBeRemoved {
private:
  int &winners, &losers;
public:

  ShouldBeRemoved(int& winners, int& losers);

  ~ShouldBeRemoved();
};

#endif
