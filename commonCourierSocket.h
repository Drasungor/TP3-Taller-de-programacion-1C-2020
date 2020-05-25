#ifndef _COMMON_COURIER_SOCKET_H_
#define  _COMMON_COURIER_SOCKET_H_


#include <cstddef>

//This is an abstract class that implements the methods necessary for the
//communication between a client socket and a peer socket
//Any failure will cause a ios_base::failure exception to be thrown due to the
//stream and io managing nature of the class
class CourierSocket {
private:
  int socket_fd;
private:
public:

  CourierSocket() noexcept;

  /*
  CommunicationSocket(CommunicationSocket&& other) noexcept;

  CommunicationSocket(const CommunicationSocket& other) = delete;

  CommunicationSocket& operator=(const CommunicationSocket& other) = delete;
  */

  ~CourierSocket();

  void receive(void* buffer, size_t buffer_len) const;

  void send(const void* buffer, size_t buffer_len) const;

  void set_fd(int fd);

  //VER COMO CAMBIAR ESTO POR OTRA COSA QUE NO SEA UN GET
  int get_fd();
};

#endif
