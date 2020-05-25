#include "serverShouldBeRemoved.h"

#include "serverClientProcessor.h"

bool ShouldBeRemoved::operator()(const ClientProcessor* client){
  if (client->has_ended()) {
    if (client->join()) {
      winners++;
    } else {
      losers++;
    }
    return true;
  }
  return false;
}


ShouldBeRemoved::ShouldBeRemoved(size_t& winners, size_t& losers){
  this->winners = winners;
  this->losers = losers;
}


ShouldBeRemoved::~ShouldBeRemoved(){
}
