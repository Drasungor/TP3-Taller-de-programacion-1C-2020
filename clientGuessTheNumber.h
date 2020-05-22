#ifndef _CLIENT_GUESS_THE_NUMBER_H_
#define  _CLIENT_GUESS_THE_NUMBER_H_


class ClientGuessTheNumber {


public:
  ClientGuessTheNumber();

  ~ClientGuessTheNumber();

  int execute(const char** arguments, int number_of_arguments);
};

#endif
