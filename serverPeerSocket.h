#ifndef _SERVER_PEERK_SOCKET_H_
#define  _SERVER_PEERK_SOCKET_H_

#include "commonCommunicationSocket.h"


class PeerSocket: public CommunicationSocket{
private:
  int socket_fd;

  //BORRAR ESTO, ES PARA PROBAR
  char message[100];
  int length;
  int beginning;
private:
public:

  //ESTE CONSTRUCTOR ES SOLO PARA PROBAR LA LOGICA DEL JUEGO
  PeerSocket(const void* message, int length);

  PeerSocket(int fd);

  PeerSocket(PeerSocket&& other) noexcept;

  PeerSocket(const PeerSocket& other) = delete;

  PeerSocket& operator=(const PeerSocket& other) = delete;

  ~PeerSocket();

  //BORRAR ESTA FUNCION, ESTA PARA PROBAR LA LOGICA DEL JUEGO
  void add_message(const void* a, int len);

  void receive(void* buffer, int buffer_len);

  void send(const void* buffer, int buffer_len) const;
};

#endif
