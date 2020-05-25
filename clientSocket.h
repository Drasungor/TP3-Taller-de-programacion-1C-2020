#ifndef _CLIENT_SOCKET_H_
#define  _CLIENT_SOCKET_H_

#include <string>
#include "commonCourierSocket.h"

class ClientSocket {
private:
  CourierSocket c_socket;
private:
  void _obtain_addrinfo(const char* host, const char* service,
                        struct addrinfo* hints, struct addrinfo** result);
  bool _process_info_to_connect(struct addrinfo* info, int& socket_fd);
  void _set_hints(struct addrinfo* hints);
public:
  ClientSocket();

  //BORRAR CONSTRUCTORES POR COPIA

  /*
  CommunicationSocket(CommunicationSocket&& other) noexcept;

  CommunicationSocket(const CommunicationSocket& other) = delete;

  CommunicationSocket& operator=(const CommunicationSocket& other) = delete;
  */

   ~ClientSocket();


   //AGREGAR MAS COMENTARIOS
   //If an empty host string is received, the host will be localhost
   void connect(const std::string& host, const std::string& service);

   void receive(void* buffer, size_t buffer_len) const;

   void send(const void* buffer, size_t buffer_len) const;
};

#endif
