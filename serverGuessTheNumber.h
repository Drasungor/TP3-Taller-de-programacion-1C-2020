#ifndef _SERVER_GUESS_THE_NUMBER_H_
#define  _SERVER_GUESS_THE_NUMBER_H_


class ServerGuessTheNumber {

public:
  ServerGuessTheNumber();

  ~ServerGuessTheNumber();

  int execute(const char** arguments, int number_of_arguments);
};

#endif
