# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: .
# El destino es una aplicaci??n
QMAKE_CXXFLAGS+= -g
FORMS =  textwindow/txtwincontrol.ui picturewindow/picwincontrol.ui \
	mainwidget.ui
CONFIG += uitools
DEFINES += __DEBUG 
HEADERS += ../consts.h \
../debug.h \
picturewindow/picturewindow.h \
picturewindow/picwincontrol.h \
textwindow/textwindow.h \
textwindow/marquesinobj.h \
textwindow/txtwincontrol.h \
../dispobject/dispobject.h \
dispobjtable/dispobjtable.h \
../configmanipulator/configmanipulator.h \
../cuser/cuser.h \
mainwidget.h

SOURCES += picturewindow/picturewindow.cpp \
picturewindow/picwincontrol.cpp \
textwindow/textwindow.cpp \
textwindow/marquesinobj.cpp \
textwindow/txtwincontrol.cpp \
../dispobject/dispobject.cpp \
dispobjtable/dispobjtable.cpp \
../configmanipulator/configmanipulator.cpp \
../cuser/cuser.cpp \
mainwidget.cpp \
test.cpp 
