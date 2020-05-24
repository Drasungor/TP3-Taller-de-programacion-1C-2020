#ifndef _COMMON_COMMUNICATION_SOCKET_H_
#define  _COMMON_COMMUNICATION_SOCKET_H_


#include <cstddef>

//This is an abstract class that implements the methods necessary for the
//communication between a client socket and a peer socket
class CommunicationSocket {
private:
  int socket_fd;
private:
public:

  //ESTE CONSTRUCTOR ES SOLO PARA PROBAR LA LOGICA DEL JUEGO
  CommunicationSocket();

  /*
  CommunicationSocket(CommunicationSocket&& other) noexcept;

  CommunicationSocket(const CommunicationSocket& other) = delete;

  CommunicationSocket& operator=(const CommunicationSocket& other) = delete;
  */

  virtual ~CommunicationSocket();

  void receive(void* buffer, size_t buffer_len) const;

  void send(const void* buffer, size_t buffer_len) const;

  void set_fd(int fd);
};

#endif
