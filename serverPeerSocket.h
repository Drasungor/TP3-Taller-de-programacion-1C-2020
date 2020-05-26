#ifndef _SERVER_PEERK_SOCKET_H_
#define  _SERVER_PEERK_SOCKET_H_

#include "commonCourierSocket.h"


//VER SI HAY QUE BORRAR ESTA HERENCIA PORQUE NO USA POLIMORFISMO

//An instance of this class should only be used when obtained from a
//ServerSocket accept method
class PeerSocket{
private:
  CourierSocket c_socket;

public:
  //This constructor should only be used by the ServerSocket
  explicit PeerSocket(int fd);

  PeerSocket(PeerSocket&& other) noexcept;

  PeerSocket& operator=(PeerSocket&& other) noexcept;

  PeerSocket(const PeerSocket& other) = delete;

  PeerSocket& operator=(const PeerSocket& other) = delete;

  ~PeerSocket();

  void receive(void* buffer, size_t buffer_len) const;

  void send(const void* buffer, size_t buffer_len) const;
};

#endif
