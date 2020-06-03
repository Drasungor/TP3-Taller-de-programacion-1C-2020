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
  //CourierSocket() noexcept;
  explicit CourierSocket(int fd = -1) noexcept;

  CourierSocket(const CourierSocket& other) = delete;

  CourierSocket& operator=(const CourierSocket& other) = delete;

  CourierSocket(CourierSocket&& other) noexcept;

  CourierSocket& operator=(CourierSocket&& other) noexcept;

  ~CourierSocket();

  void receive(void* buffer, size_t buffer_len) const;

  void send(const void* buffer, size_t buffer_len) const;

  /*
  void set_fd(int fd);

  int get_fd() const;
  */
};

#endif
