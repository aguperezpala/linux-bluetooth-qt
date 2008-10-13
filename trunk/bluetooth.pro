# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: .
# El destino es una aplicaci??n: 

CONFIG      += uitools
DEFINES	    += __DEBUG
FORMS       += src/textwindow/maintxtwindow.ui


#HEADERS += $(shell ls src/*.h)
#SOURCES += $(shell ls src/*.cpp)
HEADERS += src/textwindow/smstable.h \
	   src/qparser/parser.h \
	   src/filemanipulator/filemanipulator.h \
           src/showwindow.h \
           src/imagewindow/imagewindow.h \
           src/textwindow/textwindow.h \
           src/textwindow/maintxtwindow.h \
           src/controllerwindow.h \
	   src/mainwidget.h 
           
SOURCES += src/textwindow/smstable.cpp \
	   src/qparser/parser.cpp \
	   src/filemanipulator/filemanipulator.cpp \
           src/showwindow.cpp \
           src/imagewindow/imagewindow.cpp \
           src/textwindow/textwindow.cpp \
           src/textwindow/maintxtwindow.cpp \
           src/controllerwindow.cpp \
	   src/mainwidget.cpp \
           src/main.cpp 
