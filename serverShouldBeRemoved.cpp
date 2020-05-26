#include "serverShouldBeRemoved.h"

#include <memory>
#include "serverClientProcessor.h"


bool ShouldBeRemoved::operator()(const std::shared_ptr<ClientProcessor> client){
  if (client->has_ended()) {
    if (client->did_client_win()) {
      winners++;
    } else {
      losers++;
    }
    return true;
  }
  return false;
}


ShouldBeRemoved::ShouldBeRemoved(size_t& winners, size_t& losers):
                                 winners(winners), losers(losers){
}


ShouldBeRemoved::~ShouldBeRemoved(){
}
