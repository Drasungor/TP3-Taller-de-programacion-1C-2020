#ifndef _COMMON_COMMUNICATION_SOCKET_H_
#define  _COMMON_COMMUNICATION_SOCKET_H_


#include <cstddef>

//This is an abstract class that implements the methods necessary for the
//communication between a client socket and a peer socket
//Any failure will cause a ios_base::failure exception to be thrown due to the
//stream and io managing nature of the class
class CommunicationSocket {
private:
  int socket_fd;
private:
public:

  //ESTE CONSTRUCTOR ES SOLO PARA PROBAR LA LOGICA DEL JUEGO
  CommunicationSocket() noexcept;

  /*
  CommunicationSocket(CommunicationSocket&& other) noexcept;

  CommunicationSocket(const CommunicationSocket& other) = delete;

  CommunicationSocket& operator=(const CommunicationSocket& other) = delete;
  */

  virtual ~CommunicationSocket();

  void receive(void* buffer, size_t buffer_len) const;

  void send(const void* buffer, size_t buffer_len) const;

  void set_fd(int fd);

  //VER COMO CAMBIAR ESTO POR OTRA COSA QUE NO SEA UN GET
  int get_fd();
};

#endif
