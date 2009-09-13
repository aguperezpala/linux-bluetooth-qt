#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include <QString>

static void printq (const char* p, QString *str)
{
	printf (p,str->toStdString().c_str());
}
	

int main (int argc, char* argv[]) {
	FILE *fp;
	QString str("");
	Parser parser;
	QString *sms = NULL;
	QString *number = NULL;
	QString *text = NULL;
	
	int c;

	if (argc > 1)
		fp = fopen (argv[1], "r");
	else
		fp = fopen ("fifo", "r");

	printf ("\n");
	while (fp) {
		c = fgetc (fp);
		if (c == EOF || fp == NULL) {
			/*lo mandamos al parser*/
			/*printf ("el source es: %s \n", str.toStdString().c_str());*/
			sms = parser.getFirstSms (str);
			if (sms != NULL) {
				/*printf ("el primer sms es: *************************\n %s \n", sms->toStdString().c_str());
				printf ("***********************************\n");*/
				while (sms != NULL && !sms->isEmpty()) {
					number = parser.getNumberField (sms);
					if (number != NULL) {
						printq ("number: %s\n",number);
						delete number; number = NULL;
					}
					text = parser.getTextField(sms);
					if (text != NULL) {
						printq ("mensaje: %s.\n",text);
						delete text; text = NULL;
					}
					delete sms; sms = NULL;
					sms = parser.getFirstSms (str);
				}
				delete sms;	sms = NULL;
			}
			/* lo volvemos a abrir*/
			fp = fopen ("fifo", "r");
		} else {
			/*agregamos un str*/
			str.append (QChar (c));
		}
		
	}

	return 0;
}

