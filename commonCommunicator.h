#ifndef _COMMON_COMMUNICATOR_H_
#define  _COMMON_COMMUNICATOR_H_


//This is an abstract class that implements the methods necessary for the
//establishment of communication between a client socket and a peer socket
//Any failure will cause a ios_base::failure exception to be thrown due to the
//stream and io managing nature of the class
class Communicator {
private:
  int socket_fd;
private:
public:

  //ESTE CONSTRUCTOR ES SOLO PARA PROBAR LA LOGICA DEL JUEGO
  Communicator() noexcept;

  virtual ~CommunicationSocket();

  void set_fd(int fd);

  //VER COMO CAMBIAR ESTO POR OTRA COSA QUE NO SEA UN GET
  int get_fd();

  void open_communication_channel() = 0;
};

#endif
