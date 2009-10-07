#include <stdio.h>
#include <QString>
#include "configmanipulator.h"
#include "../tester.h"

QString confile = "prueba.conf";

int main (void)
{
	ConfigManipulator * cm = new ConfigManipulator (confile);
	QString * value = NULL;
	QString var = "AGUSTIN";
	QString data = "123456789";
	
	value = cm->getValue (var);
	fail_if (value == NULL);
	printf ("Value: %s\n", qstrtochar((*value)));
	fail_if (*value != "esto es una prueba");
	delete value;
	
	var = "PEPE";
	value = cm->getValue (var);
	fail_if (value == NULL);
	fail_if (*value != "125654");
	delete value;
	
	/*var = "NUEVA1";
	value = cm->getValue (var);
	fail_if (value != NULL);
	cm->setValue(var,data);*/
	var = "POS_X";
	data = "estoy cambiando las cosas bolo";
	cm->setValue(var,data);
	
	var = "NUEVA1";
	value = cm->getValue (var);
	fail_if (value == NULL);
	fail_if (*value != "123456789");
	delete value;
	
	
	cm->printConfVars();
	
	delete cm;
	
	return 0;
}