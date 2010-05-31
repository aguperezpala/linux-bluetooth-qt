#include "btpaket.h"

/* constructor */
BTPaket::BTPaket(void)
{
	this->msg.clear();
	this->origination = -1;
	this->cmdType = -1;
}

/* Funcion que llena un pakete segun el protocolo determinado
* RETURNS:
* 	< 0	if error (no respeta el protocolo)
*	0	on success
*/
int BTPaket::pktFromRcvString(string &data)
{
	int size = 0;
	string auxMsg = "";
	
	if (data.size() < BT_CMD_STR[0].size())
		return -1;
	
	/* ahora buscamos el cmd correspondiente */
	if (!btproto_parse_data(data, this->cmdType, size, this->msg))
		return -1;
	
	
	/* el origen del paquete es del recibido */
	this->origination = BT_PKT_RCV;
	
	/* liberamos memoria */
	
	return 0;
}

/* Funcion que setea el origen del paquete 
* {BT_PKT_RCV | BT_PKT_SND}
*/
void BTPaket::setOrigination(int o)
{
	assert(o == BT_PKT_RCV || o == BT_PKT_SND);
	
	this->origination = o;
}

/* Funcion que setea el tipo de commando commando */
void BTPaket::setCmdType(int ct)
{
	this->cmdType = ct;
}

/* Funcion que setea los datos del pakete */
void BTPaket::setMsg(string &msg)
{
	this->msg = msg;
}

/* funcion que llena los datos segun un commando y un mensaje
* RETURNS:
* 	< 0	on error
* 	== 0	if success
*/
int BTPaket::pktFromCmdAndMsg(string &cmd, string &msg)
{
	int i = 0;
	
	this->cmdType = -1;
	while (BT_CMD_STR[i].size() != 0) {
		if (BT_CMD_STR[i].compare(cmd) == 0) {
			this->cmdType = i;
			break;
		}
		i++;
	}
	if (this->cmdType < 0)
		return -1;
	
	this->msg = msg;
	
	return 0;
}

/* Funcion que convierte el pakete en una cadena de caracteres
* listo para enviarse.
* RETURNS:
* 	NULL	if error
* 	strPkt	on success
* NOTE: Genera memoria
*/
string *BTPaket::toString(void)
{
	string *result = NULL;
	char aux[10] = {0};
	
	if (this->cmdType < 0)
		return NULL;
	
	result = new string("");
	if (!result)
		return NULL;
	
	/* agregamos el commando */
	result->append(BT_CMD_STR[this->cmdType]);
	result->append(":");
	/* convertimos */
	if (sprintf(aux, "%d", (int) this->msg.size()) < 0) {
		/* wtf! error */
		assert(false);
	}
	result->append(aux);
	result->append(":");
	result->append(this->msg);
	
	return result;
}

/* destructor */
BTPaket::~BTPaket(void)
{
	this->msg.clear();
}
