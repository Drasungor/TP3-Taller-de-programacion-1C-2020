# TP3-Taller-de-programacion-1C-2020

Nombre: Agustín Cambiano

Padrón: 102291

URL: https://github.com/Drasungor/TP3-Taller-de-programacion-1C-2020.git


## Trabajo práctico No 3: Adivina el número

&nbsp;&nbsp;&nbsp;&nbsp;El presente trabajo tiene el objetivo de describir el
funcionamiento de una sección del programa realizado para cumlpir con la
consigna de desarrollar un ejecutable que
genera un servidor que acepta clientes que de deben adivinar un número
particular para ganar.
&nbsp;&nbsp;&nbsp;&nbsp;Fue consigna del trabajo utilizar polimorfismo y el
operador (). El polimorfismo fue aplicado para ServerSocket y ClientSocket
para que estos tuvieran el mismo mensaje de conexión. El operador () fue
implementado para el functor ShouldBeRemoved, para pasarle a remove_if
al momento de hacer un erase de los clientes que terminaron.
&nbsp;&nbsp;&nbsp;&nbsp;La sección a desarrollar es por un lado aquella que
se encarga de ejecutar un comando del cliente, y por otro lado las que se
encargan de procesar un comando en el servidor. Para esto se crearon las
clases (entre otras): ClientGuessNumber, ServerGuessNumber, ClientSocket,
ServerSocket, ClientsHandler, ClientProcessor.

### <ins>Clases</ins>

#### ClientGuessNumber
&nbsp;&nbsp;&nbsp;&nbsp;Es la clase que se encarga de la ejecución del
programa del cliente, lee los comandos de la entrada estándar, los procesa
y los evía al servidor usando el ClientSocket, luego utiliza la misma clase
para obtener la respuesta del servidor e imprimirla por la salida estándar

#### ServerGuessNumber
&nbsp;&nbsp;&nbsp;&nbsp;Es la clase que se encarga de manejar el servidor,
ejecuta un thread que interactúa con los clientes, mientras su thread principal
espera el comando para ser apagado. Ejecuta el thread principal del servidor.

#### ClientSocket
&nbsp;&nbsp;&nbsp;&nbsp;Es una de las clases involucradas en la comunicación
entre el servidor y el cliente. Envía y recibe mensajes del servidor, que hace
lo mismo mediante el uso de PeerSocket.

#### ServerSocket
&nbsp;&nbsp;&nbsp;&nbsp;Habilita la comunicación y la conexión de clientes.
Para comunicarse con ellos genera PeerSocket, que son los que se encargan de
intercambiar mensajes con los clientes.

#### ClientsHandler
&nbsp;&nbsp;&nbsp;&nbsp;Esta clase maneja los clientes conectados. Se encarga
de aceptar nuevos clientes y liberarlos cuando terminan su trabajo. Estas
operaciones son ejecutadas en un thread del servidor a parte del principal.

#### ClientProcessor
&nbsp;&nbsp;&nbsp;&nbsp;Esta clase se encarga de ejecutar el juego para un
cliente. Recibe comandos a ejecutar y con estos arma el mensaje que enviará
como respuesta al cliente. Estas operaciones son ejecutadas en un thread del
servidor a parte del principal.

### <ins>Diagramas</ins>

&nbsp;&nbsp;&nbsp;&nbsp;A continuación se presentan gráficos que representan
distintas partes del trabajo. Estos son:

1. Diagrama de clases del cliente.
2. Diagrama de clases del servidor.
3. Diagrama de secuencia simple la comunicación entre el cliente y el servidor
   al ejecutarse un comando.
4. Diagrama de secuencia de las operaciones realizadas por un ClientProcessor
   al recibir un número.

![Diagrama_de_clases_de_cliente](/imgs/Diagrama_de_clases_cliente.png)

![Diagrama_de_clases_de_servidor](/imgs/Diagrama_de_clases_servidor.png)

![Diagrama_de_secuencia_comando_general](/imgs/Envio_general_de_comando.png)

![Diagrama_de_secuencia_procesamiendo_de_numero](/imgs/Procesamiento_de_un_numero.png)

### <ins>Aclaraciones</ins>

&nbsp;&nbsp;&nbsp;&nbsp;En los diagramas UML se puso que el scope de los
constructores de que tiran threads se encuentran en ejecución durante toda
la ejecución de la llamada a la función que ejecuta el thread. Esto es
erróneo pero fue dejado debido a limitaciones del programa utilizado
para realizar los diagramas de secuencia. Los constructores terminan de
ejecutarse antes de que termine de ejecutarse la función que se le deja al
thread.
&nbsp;&nbsp;&nbsp;&nbsp;En el segundo diagrama de secuencia se ve que todos
los métodos se ejecutan dentro de receive. Sin embargo, se ejecuta primero
receive y luego el resto de los mensajes. Se hizo de esta forma ya que
sino se tendrían dos mensajes encontados, lo cual puede ser confuso.

### <ins>Cambios en reentrega</ins>

1. Se eliminaron las funciones set_fd y get_fd del CourierSocket y se remplazaron
   por el uso de un nuevo constructor por movimiento y operador asignacion por
   movimiento. De esta forma deja de romperse el encapsulamiento.
2. Se redujo el tamaño de las dos funciones marcadas (ServerGuessTheNumber::execute
   y ClientGuessTheNumber::execute).
3. Se cambiaron los sizeof de variables de tamaños constantes y conocidos por valores
   literales.
4. Se pasó la actualización de la cantidad de ganadores y perdedores a cada cliente
   para que lo modifique él mismo. Esto genero que la clase ShouldBeRemoved fuera
   obsoleta, por lo que fue eliminada. Se pasó la implementación del operador ()
   a ClientsHandler, para remplazar la ejecución de ClientsHandler::shutdown y
   ClientsHandler::wait_for_results.
