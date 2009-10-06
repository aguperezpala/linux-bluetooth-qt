DEFINES += __DEBUG
HEADERS += ../../tester.h \
	udbserver.h \
	../udatabase.h \	
	udbprotocol.h \
	../../debug.h \
	../../consts.h \
	../../simple_server/sclient.h \
	../../cuser/cuser.h \
	../../simple_server/sserver.h \
	
SOURCES += udbserver.cpp \
	../udatabase.cpp \
	udbprotocol.cpp \
	../../simple_server/sclient.cpp \
	../../cuser/cuser.cpp \
	../../simple_server/sserver.cpp \
	test_server.cpp 
