#include "serverShouldBeRemoved.h"



bool operator()(const ClientProcessor& client){
  if (client.has_ended()) {
    if (client.join()) {
      winners++;
    } else {
      losers++;
    }
    return true;
  }
  return false;
}


ShouldBeRemoved::ShouldBeRemoved(int& winners, int& losers){
  this->winners = winners;
  this->losers = losers;
}


ShouldBeRemoved::~ShouldBeRemoved(){
}
