DEFINES += __DEBUG
HEADERS += ../tester.h \
../consts.h \
../debug.h \
../simple_server/sserver.h \
../simple_server/sclient.h \
../dispobject/dispobject.h \
../udatabase/udatabase.h \
../cuser/cuser.h \
obexparser/obexparser.h


SOURCES += ../simple_server/sserver.cpp \
../simple_server/sclient.cpp \
../dispobject/dispobject.cpp \
../udatabase/udatabase.cpp \
../cuser/cuser.cpp \
obexreceiver.cpp \
obexparser/obexparser.cpp \
test.cpp        
