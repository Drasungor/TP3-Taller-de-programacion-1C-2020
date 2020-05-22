#ifndef _SERVER_H_
#define  _SERVER_H_


class Server {

public:
  Server();

  ~Server();

  int execute(const char** arguments, int number_of_arguments);
};

#endif
