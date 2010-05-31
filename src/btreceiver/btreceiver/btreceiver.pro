INCLUDEPATH +=  ../.. ../../udatabase ../../cuser ../btprotocol ../btpaket . \
../btcodegenerator/

DEFINES += __DEBUG

HEADERS += ../../udatabase/udatabase.h ../../debug.h ../../tester.h ../btpaket/btpaket.h \
../btprotocol/btprotocol.h ../btcodegenerator/btcodegenerator.h \
../../cuser/cuser.h ../../consts.h btreceiver.h

SOURCES += ../../cuser/cuser.cpp ../../udatabase/udatabase.cpp \
../btcodegenerator/btcodegenerator.cpp \
../btpaket/btpaket.cpp ../btprotocol/btprotocol.cpp btreceiver.cpp test.cpp 

LIBS += -lbluetooth -lbt-manager

