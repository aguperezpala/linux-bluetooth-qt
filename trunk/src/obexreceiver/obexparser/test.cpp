#include <stdio.h>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include "obexparser.h"
#include "../../consts.h"
#include "../../tester.h"

QByteArray buff;


static void printList (QStringList & list)
{
	QStringList::iterator i;
	
	printf ("\nLista:\n");
	for (i = list.begin(); i != list.end(); ++i)
		printf ("%s\n", qstrtochar((*i)));
}


int main (void)
{
	QStringList * list;
	
	buff = "{</home/xxxx/arch1><11:22:33:44:55:66>}";
	list = obpa_parse (buff);
	fail_if (list == NULL);
	printList (*list);
	delete list;
	
	buff = "{</home/xxxx/arch1><11:22:33:44:55:66>";
	list = obpa_parse (buff);
	fail_if (list != NULL);
	
	buff = "{</home/xxxx/arch1><11:22:33:44:55:66>}"
		"{</home/xxxx/arch1><11:22:33:44:55:66>}" ;
	list = obpa_parse (buff);
	fail_if (list == NULL);
	printList (*list);
	delete list;
	
	
	buff = "{</home/xxxx/arch1><11:22:33:44:55:66><pepe><cualquiera1>}";	
	list = obpa_parse (buff);
	fail_if (list == NULL);
	printList (*list);
	delete list;
	
	buff = "{</home/xxxx/arch1><11:22:33:44:55:66><}";	
	list = obpa_parse (buff);
	fail_if (list == NULL);
	printList (*list);
	delete list;
	
	buff = "{{}{</home/xxxx/arch1><11:22:33:44:55:66><}"
		"{</home/xxxx/arch2><11:22:33:44:55:66><}";	
	list = obpa_parse (buff);
	fail_if (list == NULL);
	printList (*list);
	delete list;
	list = obpa_parse (buff);
	fail_if (list == NULL);
	printList (*list);
	delete list;
	list = obpa_parse (buff);
	fail_if (list == NULL);
	printList (*list);
	delete list;
	
	return 0;
}