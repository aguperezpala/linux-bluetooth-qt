# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: .
# El destino es una aplicaci??n: 


MAKEFILE = src/users/Makefile


CONFIG += uitools

FORMS += src/mainwidget.ui \
         src/textwindow/maintxtwindow.ui 
HEADERS += src/controllerwindow.h \
           src/debug.h \
           src/mainwidget.h \
           src/showwindow.h \
           src/filemanipulator/filemanipulator.h \
           src/imagewindow/imagewindow.h \
           src/qparser/parser.h \
           src/textwindow/maintxtwindow.h \
           src/textwindow/smsobject.h \
           src/textwindow/smstable.h \
           src/textwindow/textwindow.h 
SOURCES += src/controllerwindow.cpp \
           src/main.cpp \
           src/mainwidget.cpp \
           src/showwindow.cpp \
           src/filemanipulator/filemanipulator.cpp \
           src/imagewindow/imagewindow.cpp \
           src/qparser/parser.cpp \
           src/textwindow/maintxtwindow.cpp \
           src/textwindow/smsobject.cpp \
           src/textwindow/smstable.cpp \
           src/textwindow/textwindow.cpp 
DEPENDPATH = . 
INCLUDEPATH += . \
               src \
               src/imagewindow \
               src/textwindow \
               src/qparser \
               src/filemanipulator \
               src/users \
               pruebas/wiggly 
CONFIG += release \
          warn_on 
TEMPLATE = app 
