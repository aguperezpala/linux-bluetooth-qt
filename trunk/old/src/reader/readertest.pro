CONFIG += uitools
DEFINES += __DEBUG


FORMS += ../mainwidget.ui \
         ../textwindow/maintxtwindow.ui 
HEADERS += ../controllerwindow.h \
           ../debug.h \
           ../mainwidget.h \
           ../showwindow.h \
           ../filemanipulator/filemanipulator.h \
           ../imagewindow/imagewindow.h \
           ../qparser/parser.h \
           ../textwindow/maintxtwindow.h \
           ../textwindow/smsobject.h \
           ../textwindow/smstable.h \
           ../textwindow/textwindow.h \
           ../textwindow/marquesinobj.h \
           ../userspp/userobject.h \
           ../net/userserver.h \
           ../reader/smsreader.h \
           ../userspp/userlist.h
SOURCES += ../controllerwindow.cpp \
           main.cpp \
           ../mainwidget.cpp \
           ../showwindow.cpp \
           ../filemanipulator/filemanipulator.cpp \
           ../imagewindow/imagewindow.cpp \
           ../qparser/parser.cpp \
           ../textwindow/maintxtwindow.cpp \
           ../textwindow/smsobject.cpp \
           ../textwindow/smstable.cpp \
           ../textwindow/textwindow.cpp \
           ../textwindow/marquesinobj.cpp \
           ../userspp/userobject.cpp \
           ../net/userserver.cpp \
           ../reader/smsreader.cpp \
           ../userspp/userlist.cpp