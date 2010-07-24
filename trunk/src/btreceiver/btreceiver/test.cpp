#include <iostream>
#include <assert.h>
#include <string>
#include <bluetooth/bluetooth.h>
#include "btprotocol.h"
#include "btreceiver.h"
#include "btpaket.h"


UDataBase *db;

static void crate_db(void)
{
	
	CUser * user = NULL;
	
	QString nick = QString ("MAMAAAAAA");
	QString MAC = QString("00:24:90:BF:72:F2");
	
	user = new CUser(&nick, &MAC);
	
	
	db = new UDataBase("prueba.txt");
	db->addUser(user);
	
	db->saveToFile();
	
	
	
}

int main(void)
{
	BTReceiver *btr = NULL;
	bool finish = false;
	BTPaket pkt;
	bdaddr_t mac;
	int result = 0;
	char strMac[20] = {0};
	
	
	crate_db();
	btr = new BTReceiver(db);
	if (btr->startListen() < 0) {
		cout << "Error al hacer listen\n";
		exit(1);
	}
	
	while (!finish) {
		result = btr->getReceivedObject(pkt, &mac);
		if (result < 0) {
			/* error */
			cout << "error al getReceivedObject\n";
		} else if (result == 0) {
			cout << "recibimos correctamente algo getReceivedObject\n";
			ba2str(&mac, strMac);
			cout << "Addr: " << strMac << "\ndata: " << pkt.getMsg();
			if ((int)pkt.getMsg().find("fail") >= 0)
				finish = true;
		}
		cout << endl;
		//cout.flush();
	}
	
	delete btr;
	delete db;
	
	return 0;
}
