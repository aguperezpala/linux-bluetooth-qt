#include "cuser.h"


CUser::CUser(QString * nick, QString * mac)
{
	ASSERT (mac != NULL);
	ASSERT (mac->size() == MAC_ADDR_SIZE);
	
	this->nick = QString::QString (*nick);
	this->MAC = QString::QString (*mac);
	
}

CUser * CUser::copy(void)
{
	CUser * result = NULL;
	
	result = new CUser(&(this->nick), &(this->MAC));
	return result;
}

bool CUser::operator==(const CUser& other)
{	
	return (this->MAC == other.MAC);	
}

bool CUser::operator!=(const CUser& other)
{	
	return (this->MAC != other.MAC);
}

CUser::~CUser()
{
	/* No hay nada que liberar, es memoria estatica */
}

#ifdef __DEBUG
void CUser::printUser()
{
	
	printf ("Nick: %s\tMAC: %s\n",
		 qstrtochar(this->nick), qstrtochar(this->MAC));
}
#endif

