#ifndef _CLIENT_SOCKET_H_
#define  _CLIENT_SOCKET_H_

#include <string>
#include "commonCommunicationSocket.h"
//VER SI SE SACA ESTA HERENCIA YA QUE NO AYUDA AL POLIMORFISMO, CONVIENE
//DELEGAR
class ClientSocket: public CommunicationSocket {
private:
  bool _process_info_to_connect(struct addrinfo* info, int& socket_fd);
  void _set_hints(struct addrinfo* hints);
public:
  ClientSocket();

  /*
  CommunicationSocket(CommunicationSocket&& other) noexcept;

  CommunicationSocket(const CommunicationSocket& other) = delete;

  CommunicationSocket& operator=(const CommunicationSocket& other) = delete;
  */

   ~ClientSocket();

   void connect(const std::string& host, const std::string& service);


};

#endif
