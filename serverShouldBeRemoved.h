#ifndef _COMMON_COMMUNICATION_SOCKET_H_
#define  _COMMON_COMMUNICATION_SOCKET_H_


#include <cstddef>

//Functor used to indicate remove_if if the ClientProcessor should be removed
//and update the program results in the process
class ShouldBeRemoved {
private:
  int &winners, &losers;
public:

  ShouldBeRemoved(int& winners, int& losers);

  ~ShouldBeRemoved();

  //Indicates if the client received should be erased from the clients list by
  //checking if the game has finished and updates the ammount of winners or
  //losers
  bool operator()(const ClientProcessor& client);
};

#endif
