#ifndef _SERVER_PEERK_SOCKET_H_
#define  _SERVER_PEERK_SOCKET_H_

#include "commonCommunicationSocket.h"


//VER SI HAY QUE BORRAR ESTA HERENCIA PORQUE NO USA POLIMORFISMO

//An instance of this class should only be used when obtained from a
//ServerSocket accept method
class PeerSocket: public CommunicationSocket{
private:
  //int socket_fd;

  //BORRAR ESTO, ES PARA PROBAR
  /*
  char message[100];
  int length;
  int beginning;
  */
private:
public:

  //ESTE CONSTRUCTOR ES SOLO PARA PROBAR LA LOGICA DEL JUEGO
  //PeerSocket(const void* message, int length);


  //VER SI NO HACE FALTA ESTE CONSTRUCTOR NI LA ASIGNACION POR MOVIMIENTO
  //This constructor should only be used if another PeerSocket will be moved
  //into the one constructed by this function, otherwise, the socket is invalid
  PeerSocket();

  //This constructor should only be used by the ServerSocket
  PeerSocket(int fd);

  PeerSocket(PeerSocket&& other) noexcept;

  PeerSocket& operator=(PeerSocket&& other) noexcept;

  PeerSocket(const PeerSocket& other) = delete;

  PeerSocket& operator=(const PeerSocket& other) = delete;

  ~PeerSocket();

  //BORRAR ESTA FUNCION, ESTA PARA PROBAR LA LOGICA DEL JUEGO
  //void add_message(const void* a, int len);

  //void receive(void* buffer, int buffer_len);

  //void send(const void* buffer, int buffer_len) const;
};

#endif
