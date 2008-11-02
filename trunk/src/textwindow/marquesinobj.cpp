#include "marquesinobj.h"


MarquesinObj::MarquesinObj(QString* str, int showPos)
{
	if (str == NULL || showPos < 0)
		return;
	this->data = str;
	this->showPos = showPos;
	this->addSomeOne = false;
}


int MarquesinObj::getDataSize ()
{
	int result = 0;
	
	if (this->data != NULL)
		result = this->data->length();
	
	return result;
}



MarquesinObj::~MarquesinObj()
{
	this->data = NULL;
}
