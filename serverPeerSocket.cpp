#include "serverPeerSocket.h"


///////////////////////////////PUBLIC//////////////////////////

void PeerSocket::receive(void* buffer, size_t buffer_len) const{
  c_socket.receive(buffer, buffer_len);
}

void PeerSocket::send(const void* buffer, size_t buffer_len) const{
  c_socket.send(buffer, buffer_len);
}

//VER SI ESTE CONSTRUCTOR NO HACE FALTA
PeerSocket::PeerSocket(){
}


PeerSocket::PeerSocket(int fd): c_socket(){
  c_socket.set_fd(fd);
}


PeerSocket::PeerSocket(PeerSocket&& other) noexcept{
  c_socket.set_fd(other.c_socket.get_fd());
  other.c_socket.set_fd(-1);
}

PeerSocket& PeerSocket::operator=(PeerSocket&& other) noexcept{
  c_socket.set_fd(other.c_socket.get_fd());
  other.c_socket.set_fd(-1);
  return *this;
}

PeerSocket::~PeerSocket(){
  //DO NOTHING
}
