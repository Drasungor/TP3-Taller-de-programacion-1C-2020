#ifndef _COMMON_COURIER_SOCKET_H_
#define  _COMMON_COURIER_SOCKET_H_


#include <cstddef>

//The purpose of this class is to store the socket id and send and receive
//messages. This class should only by stored by a socket that sends or receive
//messages so that those processes can be delegated to this class
class CourierSocket {
private:
  int socket_fd;

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
  int get_fd() const;
};

#endif
