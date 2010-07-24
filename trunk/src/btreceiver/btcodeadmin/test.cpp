#include <iostream>
#include <assert.h>
#include <string>
#include <vector>
#include "btcodeadmin.h"



int main(void)
{
	BTCodeAdmin ca;
	string code;
	string code2 = "00200108";
	vector<string> codes;
	vector<string>::iterator it;
	string fname = "codes.txt";
	
	
	ca.genCodes(codes, 300);
	for(it = codes.begin(); it != codes.end(); ++it) {
		cout << (*it) << endl;
	}
	
	ca.setFileName(fname);
	assert(ca.isCodeValid(code2));
	assert(!ca.isCodeValid(code));
	
	assert(ca.wasUsed(code2) == false);
	ca.addUsedCode(code2);
	assert(ca.wasUsed(code2) == true);
	ca.setCodeUnused(code);
	assert(ca.wasUsed(code2) == true);
	ca.setCodeUnused(code2);
	assert(ca.wasUsed(code2) == false);
	
	ca.genCodes(codes, 15);
	
	for(it = codes.begin(); it != codes.end(); ++it) {
		assert(ca.isCodeValid(*it));
		ca.addUsedCode(*it);
	}
	
	ca.toFile();
	for(it = codes.begin(); it != codes.end(); ++it) {
		assert(ca.wasUsed(*it) == true);
		ca.setCodeUnused(*it);
		assert(ca.wasUsed(*it) == false);
	}
	
	ca.fromFile();
	/*for(it = codes.begin(); it != codes.end(); ++it) {
		assert(ca.wasUsed(*it) == true);
	}*/
	
	
	return 0;
}
