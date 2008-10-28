#include "userserver.h"

UserServer::UserServer(UserList *list)
{
	/*inicializamos el servidor*/
	this->status = false;
	assert (list != NULL);
	
	this->userlist = list;
	
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
	

void start()
{
	while (this->status)
	{	
		int clientfd;
		struct sockaddr_in client_addr;
		int addrlen = sizeof(client_addr);
		ssize_t sizeGet = 0;
		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
		/*send(clientfd, buffer, recv(clientfd, buffer, MAXBUF, 0), 0);*/
		
		if (clientfd) {
			
		sizeGet = recv(clientfd,(void*) this->buffer,US_MAX_BUFFER_SIZE, 0);
			
		if (buf != NULL)
			printf ("el buffer %s\ntamaño recibido: %d",(char *)buf,(int)tam);
		else
			printf ("buf = NULL\n");
	
		/*---Echo back anything sent---*/
			
	
		/*---Close data connection---*/
		close(clientfd);
	}
}


bool UserServer::isActive()
{
	return this->status;
}
	
int UserServer::getPort()
{
	return inet_ntoa(this->self.sin_port);
}

void UserServer::closeServer()
{
	if (this->sock != 0) {
		this->status = false;	/*para terminarlo del listen*/
		close(this->sock);
}

UserServer::~UserServer()
{
	if (this->sock != 0)
		close(this->sock);
}
