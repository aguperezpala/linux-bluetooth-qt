#include "userserver.h"#include "userserver.h"


QString* UserServer::parseVar (QString& src, const char *var)
{
	QString *result = NULL;
	QString aux = "";
	int pos = -1;
	int pos2 = -1;
	int eqpos = -1;		/*donde vamos almacenar la posicion del =*/
	int endpos = -1;	/*\n*/
	
	/*simplificamos el src (sacamos espacios y eso)*/
	src.simplified();
	
	if (var != NULL) {
		aux.append ("$");			/*agregamos el parametro de comparacion*/
		aux.append (QString (var)); /*sagregamos la variable a buscar*/
		aux.append ("=");			/*agregamos el =*/
		/*ahora buscamos*/
		pos = src.indexOf(QString(var),0, Qt::CaseInsensitive);
		if (pos >= 0) {
			/*entonces encontramos alguna cadena*/
			/*ahora buscamos hasta la proxima $ o \n*/
			pos2 = src.indexOf(QChar('$'), pos+1 ,Qt::CaseInsensitive);
			eqpos = src.indexOf(QChar('='), pos+1 ,Qt::CaseInsensitive);
			endpos = src.indexOf(QChar(';'), pos+1 ,Qt::CaseInsensitive);
			
			if (pos2 < 0 || pos2 < pos || eqpos > pos2 || endpos < pos2) {
				dprintf ("error parseVar, no se encontro la cadena \n");
				return result;
			}
			/*ahora sabemos que tenemos que tomar los datos que estan entre
			eqpos+1 y pos2-1*/
			result = new QString ("");
			if (result != NULL) {
				(*result).append (src.mid (eqpos + 1, pos2 - eqpos - 2));
				dprintf ("parseVar: %s\n",(*result).toStdString().c_str());
			}
			
		}
	}
	
	return result;
}


UserServer::UserServer(UserList *l)
{
	/*inicializamos el servidor*/
	this->status = false;
	assert (l != NULL);
	
	this->userlist = l;
	
}


bool UserServer::startListen()
{
	this->sock = 0;
	int i = 0;
	
	/*creamos el socket asi nomas*/
	this->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sock < 0)
	{
		dprintf ("Socket");
		this->status = false;
		return false;
	}
	
	this->self.sin_family = AF_INET;
	this->self.sin_addr.s_addr = INADDR_ANY;
	/*ahora intentamos conectarnos*/
	for (i = US_MINOR_PORT_RANGE; i < US_MAYOR_PORT_RANGE; i++) {
		this->self.sin_port = htons(i);
		if ( bind(this->sock, (struct sockaddr*)&(this->self), sizeof(this->self)) != 0 )
		{
			dprintf("no se pudo conectar al puerto %d\n",i);
		} else {
			/*si nos pudimos conectar entonces tenemos que salir del ciclo*/
			printf ("UserServer: Escuchando en el puerto %d\n", i);
			this->status = true;
			i = US_MAYOR_PORT_RANGE;	/*por las dudas*/
			break;
		}
	}
	/*escuchamos*/
	if ( listen(this->sock, 20) != 0 )
	{
		dprintf("error listen\n");
		this->status = false;
		return(false);
	}
	
	return true;

}

bool UserServer::startListenOn(int port)
{
	bool result = true;
	
	this->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sock < 0)
	{
		dprintf ("Socket");
		this->status = false;
		return false;
	}
	
	this->self.sin_family = AF_INET;
	this->self.sin_addr.s_addr = INADDR_ANY;
	this->self.sin_port = htons(port);
	/*ahora intentamos conectarnos*/
	if ( bind(this->sock, (struct sockaddr*)&(this->self), sizeof(this->self)) != 0 ) {
			dprintf("no se pudo conectar al puerto %d\n",port);
			this->status = false;
			return false;
		}
	
	/*escuchamos*/
	if ( listen(this->sock, 20) != 0 )
	{
		dprintf("error listen\n");
		this->status = false;
		return(false);
	}
	
	this->status = true;
	
	return result;
}
	

void UserServer::start()
{
	int clientfd = 0;
	struct sockaddr_in client_addr;
	socklen_t addrlen = sizeof(client_addr);
	ssize_t sizeGet = 0;
	UserObject *usr = NULL;
	QString *number = NULL;
	QString *aux = NULL;
	
	while (this->status)
	{	
		number = NULL;
		usr = NULL;
		clientfd = 0;
		sizeGet = 0;
		aux = NULL;
		
		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept((this->sock), (struct sockaddr*)&client_addr, &addrlen);
		/*send(clientfd, buffer, recv(clientfd, buffer, MAXBUF, 0), 0);*/
		
		if (clientfd) {
			/*limpiamos el buffer*/
			memset (buffer, 0, US_MAX_BUFFER_SIZE *sizeof (char));
			/*recibimos los datos*/
			sizeGet = recv(clientfd,(void*) this->buffer,US_MAX_BUFFER_SIZE, 0);
			/*los parseamos*/
			if (sizeGet > 0) {
				/*aca deberiamos parsear todas las variables*/
				aux = new QString (buffer);
				if (aux != NULL) {
					number = parseVar (*aux, US_VAR_NUMBER);
					if (number != NULL) {
						/*ahora lo que debemos hacer es agregar el numero a la lista*/
						/*creamos el usuario*/
						usr = new UserObject();
						if (usr != NULL) {
							usr->setNumber (*number);
							this->userlist->insertUser (usr);
							printf ("UserServer: Numero recibido %s\n",
									 (*number).toStdString().c_str());
							/*!le devolvemos un "OK"*/
							send (clientfd, US_OK_RESPONSE, strlen (US_OK_RESPONSE), 0);
						}
						delete number;	/*eliminamos el nombre ya que user COPIA todo*/
					} else {
						/*!ahora devolvemos un "ERROR"*/
						send (clientfd, US_ERROR_RESPONSE, strlen (US_ERROR_RESPONSE), 0);
						dprintf ("UserServer: no se parseo nada\n");
					}
					delete aux;
					
				}
			}
				
			
			/*cerramos la coneccion*/
			close(clientfd);
		}
	}
}


bool UserServer::isActive()
{
	return this->status;
}
	
int UserServer::getPort()
{
	return (int)(this->self.sin_port);
}

void UserServer::closeServer()
{
	if (this->sock != 0) {
		this->status = false;	/*para terminarlo del listen*/
		close(this->sock);
	}
}

UserServer::~UserServer()
{
	if (this->sock != 0)
		close(this->sock);
}
