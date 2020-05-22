#ifndef _CLIENT_H_
#define  _CLIENT_H_


class Client {


public:
  Client();

  ~Client();

  int execute(const char** arguments, int number_of_arguments);
};

#endif
