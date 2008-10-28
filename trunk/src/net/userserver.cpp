#include "userserver.h"


QString& UserServer::parseVar (QString& src, const char *var)
{
	QString result = "";
	QString aux = "";
	int pos = -1;
	int pos2 = -1;
	
	if (var != NULL) {
		aux.append ("$");			/*agregamos el parametro de comparacion*/
		aux.append (QString (var)); /*sagregamos la variable a buscar*/
		aux.append ("=");			/*agregamos el =*/
		/*ahora buscamos*/
		pos = src.indexOf(QString(var),0, Qt::CaseInsensitive);
		if (pos >= 0) {
			/*entonces encontramos alguna cadena*/
			/*ahora buscamos hasta la proxima $ o \n*/
			pos2 = src.indexOf(QString("$"), pos ,Qt::CaseInsensitive);
			
			if (pos2 < 0 || pos2 < pos)
				return result;
			
			/*por seguridad vamos a hacer una busqueda mas*/
			
		}
	
	
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
			dprintf("no se pudo conectar al puerto %d\n",i);
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
	
	return result;
}
	

void UserServer::start()
{
	while (this->status)
	{	
		int clientfd = 0;
		struct sockaddr_in client_addr;
		socklen_t addrlen = sizeof(client_addr);
		ssize_t sizeGet = 0;
		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept((this->sock), (struct sockaddr*)&client_addr, &addrlen);
		/*send(clientfd, buffer, recv(clientfd, buffer, MAXBUF, 0), 0);*/
		
		if (clientfd) {
			
			sizeGet = recv(clientfd,(void*) this->buffer,US_MAX_BUFFER_SIZE, 0);
		}
		
	
		close(clientfd);
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
