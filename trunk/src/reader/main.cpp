#include <stdio.h>
#include <stdlib.h>
#include <QString>

#include "smsreader.h"
#include "../textwindow/maintxtwindow.h"



int main (int argc, char *argv[]) {
	QApplication app(argc, argv);
	MainTxtWindow *mtw = new MainTxtWindow(0);
	SmsReader *reader = new SmsReader(mtw);
	
	reader->setFile ("../../fifo");
	reader->startReading();
	
	delete reader;
	delete mtw;
	return 0;
}

