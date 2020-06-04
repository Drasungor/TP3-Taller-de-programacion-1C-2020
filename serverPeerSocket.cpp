#include "serverPeerSocket.h"

#include <utility>

///////////////////////////////PUBLIC//////////////////////////

void PeerSocket::receive(void* buffer, size_t buffer_len) const{
  c_socket.receive(buffer, buffer_len);
}

void PeerSocket::send(const void* buffer, size_t buffer_len) const{
  c_socket.send(buffer, buffer_len);
}

PeerSocket::PeerSocket(int fd): c_socket(fd){
}


PeerSocket::PeerSocket(PeerSocket&& other) noexcept{
  c_socket = std::move(other.c_socket);
}

PeerSocket& PeerSocket::operator=(PeerSocket&& other) noexcept{
  c_socket = std::move(other.c_socket);
  return *this;
}

PeerSocket::~PeerSocket(){
  //DO NOTHING
}
