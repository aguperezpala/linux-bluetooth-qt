#include <stdio.h>
#include <QString>
#include "udbprotocol.h"
#include "../../tester.h"

QString req;
QString resp;

static void check_validReq(void)
{
	/*<SSDBP>*/
	req = "<SSD";
	fail_if (udbs_isValidRequest (req) != UDBS_INCOMPLETE);
	req = "<SSDsad";
	fail_if (udbs_isValidRequest (req) != UDBS_NO_PROTOCOL);
	req = "<SSDBP>";
	fail_if (udbs_isValidRequest (req) != UDBS_INCOMPLETE);
	req = "<SSDBP>asd<SSDBP>a";
	fail_if (udbs_isValidRequest (req) != UDBS_NO_PROTOCOL);
	req = "<SSDBP>asd<SSDB";
	fail_if (udbs_isValidRequest (req) != UDBS_INCOMPLETE);
	req = "<SSDBP>asd<SSDBP>";
	fail_if (udbs_isValidRequest (req) != UDBS_NO_ERROR);
	req = "<SSDBP>asd<SSDB<SSDBP>asd<SSDB<SSDBP>asd<SSDB<SSDBP>asd<SSDB"
	"<SSDBP>asd<SSDB<SSDBP>asd<SSDB<SSDBP>asd<SSDB<SSDBP>asd<SSDB"
	"<SSDBP>asd<SSDB<SSDBP>asd<SSDB<SSDBP>asd<SSDB<SSDBP>asd<SSDB"
	"<SSDBP>asd<SSDB<SSDBP>asd<SSDB<SSDBP>asd<SSDB<SSDBP>asd<SSDB"
	"<SSDBP>asd<SSDB<SSDBP>asd<SSDB<SSDBP>asd<SSDB<SSDBP>asd<SSDB"
	"<SSDBP>asd<SSDB<SSDBP>asd<SSDB<SSDBP>asd<SSDB<SSDBP>asd<SSDB";
	fail_if (udbs_isValidRequest (req) != UDBS_OVERFLOW);
}

static void check_createResp (void)
{
	resp = "asd";
	
	udbs_generateResponse(resp);
	printf ("\n%s\n", qstrtochar(resp));
	fail_if (resp != "<SSDBP>asd<SSDBP>");
}

static void check_parseResq (void)
{
	QString * result = NULL;
	
	req = "<SSDBP>asd<SSDBP>";
	
	result = udbs_parseRequest(req);
	fail_if (*result != "asd");
}
	
	
	
	

int main (void)
{
	check_validReq();
	check_createResp();
	check_parseResq();
	
	return 0;
}