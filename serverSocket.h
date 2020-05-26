#ifndef _SERVER_SOCKET_H_
#define  _SERVER_SOCKET_H_

#include <string>
#include "serverPeerSocket.h"
#include "commonCommunicator.h"

class ServerSocket: public Communicator {
private:
  int socket_fd;

private:
  bool _process_info_to_bind(struct addrinfo* info, int& socket_fd);
  bool _could_bind(struct addrinfo* info, int& socket_fd);
  void _set_hints(struct addrinfo* hints);
  void _bind_and_listen(const char* host, const char* service);

  //////////////////////////INHERITED//////////////////////////
  void allow_communication(const char* service, const char* host) override;

public:
  explicit ServerSocket(const std::string& service);

  ServerSocket(const ServerSocket& other) = delete;

  ServerSocket& operator=(const ServerSocket& other) = delete;

  ~ServerSocket();

  //Returns a PeerSocket that allows for communication with a client
  PeerSocket accept();

  //Closes the server
  void disconnect();
};

#endif
