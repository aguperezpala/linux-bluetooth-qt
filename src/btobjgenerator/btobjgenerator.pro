DEFINES += __DEBUG
HEADERS += ../btreceiver/btcodeadmin/btcodeadmin.h ../btreceiver/btreceiver/btreceiver.h ../btreceiver/btprotocol/btprotocol.h \
../btreceiver/btpaket/btpaket.h ../cuser/cuser.h ../udatabase/udatabase.h \
../dispobject/dispobject.h ../debug.h ../tester.h ../consts.h 

SOURCES += ../btreceiver/btreceiver/btreceiver.cpp ../btreceiver/btprotocol/btprotocol.cpp \
../btreceiver/btpaket/btpaket.cpp ../cuser/cuser.cpp ../udatabase/udatabase.cpp \
../dispobject/dispobject.cpp ../btreceiver/btcodeadmin/btcodeadmin.cpp test.cpp
INCLUDEPATH += ../btreceiver/btreceiver ../btreceiver/btcodeadmin  \
../btreceiver/btprotocol ../btreceiver/btpaket ../cuser ../udatabase ../dispobject ..

LIBS += -lbluetooth -lbt-manager
