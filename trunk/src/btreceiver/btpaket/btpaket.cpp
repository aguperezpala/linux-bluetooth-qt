#include "btpaket.h"

/* constructor */
BtPaket::BtPaket(void)
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
int BtPaket::pktFromRcvString(string &data)
{
	int size = 0;
	string *cmd = NULL, *auxMsg = NULL;
	int i = 0;
	
	if (data.size() < BT_CMD_STR[0][0].size())
		return -1;
	
	/* ahora buscamos el cmd correspondiente */
	if (!btproto_parse_data(data, &cmd, &size, &auxMsg))
		return -1;
	
	/* tenemos los datos verdaderos ==> buscmaos el commando y asignamos
	 * la data */
	ASSERT(cmd != NULL);
	ASSERT(auxMsg != NULL);
	this->msg = *auxMsg;
	
	this->cmdType = -1;
	
	while (BT_CMD_STR[i][0] != NULL) {
		if (BT_CMD_STR[i][0] == *cmd) {
			this->cmdType = i;
			break;
		}
		i++;
	}
	/* el origen del paquete es del recibido */
	this->origination = BT_PKT_RCV;
	
	/* liberamos memoria */
	delete cmd;
	delete auxMsg;
	
	return 0;
}

/* Funcion que setea el origen del paquete 
* {BT_PKT_RCV | BT_PKT_SND}
*/
void BtPaket::setOrigination(int o)
{
	if (o != BT_PKT_RCV || o != BT_PKT_SND)
		return;
	
	this->origination = o;
}

/* Funcion que setea el tipo de commando commando */
void BtPaket::setCmdType(int ct)
{
	this->cmdType = ct;
}

/* Funcion que setea los datos del pakete */
void BtPaket::setMsg(string &msg)
{
	this->msg = msg;
}

/* funcion que llena los datos segun un commando y un mensaje
* RETURNS:
* 	< 0	on error
* 	== 0	if success
*/
int BtPaket::pktFromCmdAndMsg(string &cmd, string &msg)
{
	int i = 0;
	
	this->cmdType = -1;
	while (BT_CMD_STR[i][0] != NULL) {
		if (BT_CMD_STR[i][0] == *cmd) {
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
string *BtPaket::toString(void)
{
	string *result = NULL;
	char aux[10] = {0};
	
	if (this->cmdType < 0)
		return NULL;
	
	result = new String("");
	if (!result)
		return NULL;
	
	/* agregamos el commando */
	result->append(BT_CMD_STR[this->cmdType][0]);
	result->append(":");
	/* convertimos */
	if (sprintf(aux, "%d", this->msg.size()) < 0) {
		/* wtf! error */
		ASSERT(false);
	}
	result->append(aux);
	result->append(":");
	result->append(this->msg);
	
	return result;
}

/* destructor */
BtPaket::~BtPaket(void)
{
	this->msg.clear();
}
