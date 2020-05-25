#ifndef _SERVER_SOCKET_H_
#define  _SERVER_SOCKET_H_

#include <string>
#include "serverPeerSocket.h"

class ServerSocket {
private:
  int socket_fd;
private:
  bool _process_info_to_bind(struct addrinfo* info, int& socket_fd);
  bool _could_bind(struct addrinfo* info, int& socket_fd);
  void _set_hints(struct addrinfo* hints);
public:
  //BORRAR CONSTRUCTOR POR COPIA Y OPERADOR ASIGNACION POR COPIA


  ServerSocket();

  ~ServerSocket();

  void bind_and_listen(const std::string& service);

  PeerSocket accept();

  void disconnect();
};

#endif
