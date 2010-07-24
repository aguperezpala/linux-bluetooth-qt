INCLUDEPATH +=  ../.. ../../udatabase ../../cuser ../btprotocol ../btpaket . \
../btcodeadmin/ ../btreceiver/ ../btobjgenerator ../../dispobject

DEFINES += __DEBUG

HEADERS += ../../udatabase/udatabase.h ../../debug.h ../../tester.h ../btpaket/btpaket.h \
../btprotocol/btprotocol.h ../btcodeadmin/btcodeadmin.h \
../../cuser/cuser.h ../../consts.h ../btreceiver/btreceiver.h \
../btobjgenerator/btobjgenerator.h ../../dispobject/dispobject.h btsystemmanager.h

SOURCES += ../../cuser/cuser.cpp ../../udatabase/udatabase.cpp \
../btcodeadmin/btcodeadmin.cpp \
../btpaket/btpaket.cpp ../btprotocol/btprotocol.cpp btsystemmanager.cpp\
../btreceiver/btreceiver.cpp ../btobjgenerator/btobjgenerator.cpp ../../dispobject/dispobject.cpp \
test.cpp 

LIBS += -lbluetooth -lbt-manager
QMAKE_CXXFLAGS += -g
CXXFLAGS += -g

