#include "marquesinobj.h"


MarquesinObj::MarquesinObj(QString* str, int showPos)
{
	if (str == NULL || showPos < 0)
		return;
	this->data = str;
	this->showPos = showPos;
	this->addSomeOne = false;
}


void MarquesinObj::setData (QString * d)
{
	ASSERT (d != NULL);
	
	if (d == NULL)
		return;
	if (this->data != NULL)
		delete this->data;
	
	this->data = d;
}
	

int MarquesinObj::getDataSize ()
{
	int result = -1;
	
	if (this->data != NULL)
		result = this->data->length();
	
	return result;
}



MarquesinObj::~MarquesinObj()
{
	if (this->data != NULL)
		delete this->data;
	this->data = NULL;
}
