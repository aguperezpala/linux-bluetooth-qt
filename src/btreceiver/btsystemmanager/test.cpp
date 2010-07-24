#include <iostream>
#include <assert.h>
#include <string>
#include "btsystemmanager.h"
#include "dispobject.h"


UDataBase *db;
BTSystemManager *btSM;

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
	bool finish = false;
	DispObject *obj = NULL;
	const CUser *user = NULL;
	string auxStr = "";
	crate_db();
	btSM = new BTSystemManager(db);
	cout << "initializeDongles: \n";
	/*if (btSM->initializeDongles() < 0){
		cout << "Error: " << btSM->getReport().toStdString() << endl;
		cout << "repairing: " << btSM->tryToRepairDongles() << endl;
	}*/
	
	/* seteamos el tiempo maximo que queremos para cada conexion:
	 * 3 segundos: */
	btSM->setMaxConnTime(3000000);
	
	/* comenzamos a recibir */
	cout << "comenzando a recibir objetos\n";
	btSM->startToReceive();
	
	
	
	while (!finish) {
		cout << "*************************\nReport: " << 
		btSM->getReport().toStdString() << "**************************\n";
		
		obj = btSM->getDispObject();
		if(obj == NULL) {
			cerr << "Error, tenemos un objeto VACIO!!!\n";
			continue;
		}
		user = obj->getUser();
		auxStr = obj->getData().toStdString();
		cout << "\nObjeto recibido: \n" <<
		"Obj->data: " << obj->getData().toStdString()<< endl <<
		"Obj->Kind: " << obj->kind << endl <<
		"Obj->user: ";
		user->printUser();
		cout << endl << "fin objeto recibido\n" << endl;
		
		/*! work obj here */
		delete obj;
		if((int) auxStr.find("fail") >= 0)
			break;
		
	}
	
	delete btSM;
	delete db;
	
	return 0;
}
