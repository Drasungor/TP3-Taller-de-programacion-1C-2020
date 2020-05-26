#ifndef _CLIENT_SOCKET_H_
#define  _CLIENT_SOCKET_H_

#include <string>
#include "commonCourierSocket.h"
#include "commonCommunicator.h"

//This class represents a client socket, it is used to establish connection
//with an existing server socket and exchange messagges with it
class ClientSocket: public Communicator{
private:
  CourierSocket c_socket;

private:
  bool _could_connect(struct addrinfo* info, int& socket_fd);
  void _obtain_addrinfo(const char* host, const char* service,
                        struct addrinfo* hints, struct addrinfo** result);
  bool _process_info_to_connect(struct addrinfo* info, int& socket_fd);
  void _set_hints(struct addrinfo* hints);
  void _connect(const char* host, const char* service);

  //////////////////////////INHERITED//////////////////////////
  void allow_communication(const char* service, const char* host) override;

public:
  //If an empty host string is received, the host will be localhost
  ClientSocket(const std::string& service, const std::string& host);

  ClientSocket(const ClientSocket& other) = delete;

  ClientSocket& operator=(const ClientSocket& other) = delete;

  //VER SI SE PUEDE USAR CONSTRUCTOR POR MOVIMIENTO PARA LA LISTA DE CLIENTES
  /*
  ClientSocket(ClientSocket&& other) noexcept;
  */

   ~ClientSocket();

   //Stores buffer_len bytes in buffer that were sent by other socket, if the
   //received bytes represent an element with endianness then a ntoh function
   //should be called
   void receive(void* buffer, size_t buffer_len) const;

   //Sends to the host the message of buffer_len bytes stored in buffer
   void send(const void* buffer, size_t buffer_len) const;
};

#endif
