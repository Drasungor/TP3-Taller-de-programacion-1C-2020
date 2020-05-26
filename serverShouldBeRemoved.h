#ifndef _SERVER_SHOULD_BE_REMOVED_H_
#define  _SERVER_SHOULD_BE_REMOVED_H_

#include "serverClientProcessor.h"

//Functor used to indicate remove_if if the ClientProcessor should be removed
//and update the program results in the process
class ShouldBeRemoved {
private:
  size_t &winners;
  size_t &losers;

public:
  ShouldBeRemoved(size_t& winners, size_t& losers);

  ~ShouldBeRemoved();

  //Indicates if the client received should be erased from the clients list by
  //checking if the game has finished and updates the ammount of winners or
  //losers
  //bool operator()(const ClientProcessor* client);
  bool operator()(const std::shared_ptr<ClientProcessor> client);
};

#endif
