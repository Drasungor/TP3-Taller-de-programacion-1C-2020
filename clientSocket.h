#ifndef _CLIENT_SOCKET_H_
#define  _CLIENT_SOCKET_H_

//This is an abstract class that implements the methods necessary for the
//communication between a client socket and a peer socket
class ClientSocket: public CommunicationSocket {
private:
public:

  //ESTE CONSTRUCTOR ES SOLO PARA PROBAR LA LOGICA DEL JUEGO
  ClientSocket();

  /*
  CommunicationSocket(CommunicationSocket&& other) noexcept;

  CommunicationSocket(const CommunicationSocket& other) = delete;

  CommunicationSocket& operator=(const CommunicationSocket& other) = delete;
  */

   ~ClientSocket();

  //void receive(void* buffer, int buffer_len) const;

  //void send(const void* buffer, int buffer_len) const;
};

#endif
