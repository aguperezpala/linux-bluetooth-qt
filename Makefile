#############################################################################
# Makefile for building: bluetooth
# Generated by qmake (2.01a) (Qt 4.4.3) on: dom oct 12 02:31:13 2008
# Project:  bluetooth.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile bluetooth.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -D__DEBUG -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -m64 -pipe -O2 -D_REENTRANT -Wall -W $(DEFINES)
CXXFLAGS      = -m64 -pipe -O2 -D_REENTRANT -Wall -W $(DEFINES)
INCPATH       = -I/usr/local/Trolltech/Qt-4.4.3/mkspecs/linux-g++-64 -I. -I/usr/local/Trolltech/Qt-4.4.3/include/QtUiTools -I/usr/local/Trolltech/Qt-4.4.3/include/QtCore -I/usr/local/Trolltech/Qt-4.4.3/include/QtCore -I/usr/local/Trolltech/Qt-4.4.3/include/QtGui -I/usr/local/Trolltech/Qt-4.4.3/include/QtGui -I/usr/local/Trolltech/Qt-4.4.3/include/QtXml -I/usr/local/Trolltech/Qt-4.4.3/include/QtXml -I/usr/local/Trolltech/Qt-4.4.3/include -I. -I.
LINK          = g++
LFLAGS        = -m64 -Wl,-rpath,/usr/local/Trolltech/Qt-4.4.3/lib
LIBS          = $(SUBLIBS)  -L/usr/local/Trolltech/Qt-4.4.3/lib -L/usr/local/Trolltech/Qt-4.4.3/lib -pthread -L/usr/local/lib -pthread -L/usr/X11R6/lib64 -pthread -pthread -pthread -pthread -lQtUiTools -L/home/agustin/programacion/C++/picture_load/qt/qt-x11-opensource-src-4.4.3/lib -lQtXml -pthread -pthread -pthread -pthread -lQtGui -pthread -pthread -pthread -pthread -lfreetype -pthread -pthread -lXext -lX11 -lQtCore -pthread -lm -pthread -lgthread-2.0 -lrt -lglib-2.0 -ldl -lpthread
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = src/qparser/parser.cpp \
		src/controllerwindow.cpp \
		src/showwindow.cpp \
		src/main.cpp \
		src/imagewindow/imagewindow.cpp \
		src/textwindow/textwindow.cpp \
		src/textwindow/smstable.cpp \
		src/textwindow/maintxtwindow.cpp \
		src/filemanipulator/filemanipulator.cpp \
		src/mainwidget.cpp moc_controllerwindow.cpp \
		moc_showwindow.cpp \
		moc_smstable.cpp \
		moc_maintxtwindow.cpp \
		moc_mainwidget.cpp
OBJECTS       = parser.o \
		controllerwindow.o \
		showwindow.o \
		main.o \
		imagewindow.o \
		textwindow.o \
		smstable.o \
		maintxtwindow.o \
		filemanipulator.o \
		mainwidget.o \
		moc_controllerwindow.o \
		moc_showwindow.o \
		moc_smstable.o \
		moc_maintxtwindow.o \
		moc_mainwidget.o
DIST          = /usr/local/Trolltech/Qt-4.4.3/mkspecs/common/g++.conf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/common/unix.conf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/common/linux.conf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/qconfig.pri \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/qt_functions.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/qt_config.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/exclusive_builds.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/default_pre.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/release.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/default_post.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/qt.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/uitools.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/unix/thread.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/moc.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/warn_on.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/resources.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/uic.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/yacc.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/lex.prf \
		bluetooth.pro
QMAKE_TARGET  = bluetooth
DESTDIR       = 
TARGET        = bluetooth

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_maintxtwindow.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: bluetooth.pro  /usr/local/Trolltech/Qt-4.4.3/mkspecs/linux-g++-64/qmake.conf /usr/local/Trolltech/Qt-4.4.3/mkspecs/common/g++.conf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/common/unix.conf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/common/linux.conf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/qconfig.pri \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/qt_functions.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/qt_config.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/exclusive_builds.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/default_pre.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/release.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/default_post.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/qt.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/uitools.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/unix/thread.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/moc.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/warn_on.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/resources.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/uic.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/yacc.prf \
		/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/lex.prf \
		/usr/local/Trolltech/Qt-4.4.3/lib/libQtXml.prl \
		/usr/local/Trolltech/Qt-4.4.3/lib/libQtCore.prl \
		/usr/local/Trolltech/Qt-4.4.3/lib/libQtGui.prl \
		/usr/local/Trolltech/Qt-4.4.3/lib/libQtUiTools.prl
	$(QMAKE) -unix -o Makefile bluetooth.pro
/usr/local/Trolltech/Qt-4.4.3/mkspecs/common/g++.conf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/common/unix.conf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/common/linux.conf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/qconfig.pri:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/qt_functions.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/qt_config.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/exclusive_builds.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/default_pre.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/release.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/default_post.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/qt.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/uitools.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/unix/thread.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/moc.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/warn_on.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/resources.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/uic.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/yacc.prf:
/usr/local/Trolltech/Qt-4.4.3/mkspecs/features/lex.prf:
/usr/local/Trolltech/Qt-4.4.3/lib/libQtXml.prl:
/usr/local/Trolltech/Qt-4.4.3/lib/libQtCore.prl:
/usr/local/Trolltech/Qt-4.4.3/lib/libQtGui.prl:
/usr/local/Trolltech/Qt-4.4.3/lib/libQtUiTools.prl:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile bluetooth.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/bluetooth1.0.0 || $(MKDIR) .tmp/bluetooth1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/bluetooth1.0.0/ && $(COPY_FILE) --parents src/qparser/parser.h src/controllerwindow.h src/showwindow.h src/imagewindow/imagewindow.h src/textwindow/textwindow.h src/textwindow/smstable.h src/textwindow/maintxtwindow.h src/filemanipulator/filemanipulator.h src/mainwidget.h .tmp/bluetooth1.0.0/ && $(COPY_FILE) --parents src/qparser/parser.cpp src/controllerwindow.cpp src/showwindow.cpp src/main.cpp src/imagewindow/imagewindow.cpp src/textwindow/textwindow.cpp src/textwindow/smstable.cpp src/textwindow/maintxtwindow.cpp src/filemanipulator/filemanipulator.cpp src/mainwidget.cpp .tmp/bluetooth1.0.0/ && $(COPY_FILE) --parents src/textwindow/maintxtwindow.ui .tmp/bluetooth1.0.0/ && (cd `dirname .tmp/bluetooth1.0.0` && $(TAR) bluetooth1.0.0.tar bluetooth1.0.0 && $(COMPRESS) bluetooth1.0.0.tar) && $(MOVE) `dirname .tmp/bluetooth1.0.0`/bluetooth1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/bluetooth1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_controllerwindow.cpp moc_showwindow.cpp moc_smstable.cpp moc_maintxtwindow.cpp moc_mainwidget.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_controllerwindow.cpp moc_showwindow.cpp moc_smstable.cpp moc_maintxtwindow.cpp moc_mainwidget.cpp
moc_controllerwindow.cpp: src/imagewindow/imagewindow.h \
		src/showwindow.h \
		src/textwindow/textwindow.h \
		src/qparser/parser.h \
		src/controllerwindow.h
	/usr/local/Trolltech/Qt-4.4.3/bin/moc $(DEFINES) $(INCPATH) src/controllerwindow.h -o moc_controllerwindow.cpp

moc_showwindow.cpp: src/showwindow.h
	/usr/local/Trolltech/Qt-4.4.3/bin/moc $(DEFINES) $(INCPATH) src/showwindow.h -o moc_showwindow.cpp

moc_smstable.cpp: src/textwindow/smstable.h
	/usr/local/Trolltech/Qt-4.4.3/bin/moc $(DEFINES) $(INCPATH) src/textwindow/smstable.h -o moc_smstable.cpp

moc_maintxtwindow.cpp: ui_maintxtwindow.h \
		src/textwindow/textwindow.h \
		src/showwindow.h \
		src/filemanipulator/filemanipulator.h \
		src/qparser/parser.h \
		src/textwindow/maintxtwindow.h
	/usr/local/Trolltech/Qt-4.4.3/bin/moc $(DEFINES) $(INCPATH) src/textwindow/maintxtwindow.h -o moc_maintxtwindow.cpp

moc_mainwidget.cpp: src/mainwidget.h
	/usr/local/Trolltech/Qt-4.4.3/bin/moc $(DEFINES) $(INCPATH) src/mainwidget.h -o moc_mainwidget.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_maintxtwindow.h
compiler_uic_clean:
	-$(DEL_FILE) ui_maintxtwindow.h
ui_maintxtwindow.h: src/textwindow/maintxtwindow.ui
	/usr/local/Trolltech/Qt-4.4.3/bin/uic src/textwindow/maintxtwindow.ui -o ui_maintxtwindow.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

parser.o: src/qparser/parser.cpp src/qparser/parser.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o parser.o src/qparser/parser.cpp

controllerwindow.o: src/controllerwindow.cpp src/controllerwindow.h \
		src/imagewindow/imagewindow.h \
		src/showwindow.h \
		src/textwindow/textwindow.h \
		src/qparser/parser.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o controllerwindow.o src/controllerwindow.cpp

showwindow.o: src/showwindow.cpp src/showwindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o showwindow.o src/showwindow.cpp

main.o: src/main.cpp src/controllerwindow.h \
		src/imagewindow/imagewindow.h \
		src/showwindow.h \
		src/textwindow/textwindow.h \
		src/qparser/parser.h \
		src/mainwidget.h \
		src/textwindow/maintxtwindow.h \
		ui_maintxtwindow.h \
		src/filemanipulator/filemanipulator.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o src/main.cpp

imagewindow.o: src/imagewindow/imagewindow.cpp src/imagewindow/imagewindow.h \
		src/showwindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o imagewindow.o src/imagewindow/imagewindow.cpp

textwindow.o: src/textwindow/textwindow.cpp src/textwindow/textwindow.h \
		src/showwindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o textwindow.o src/textwindow/textwindow.cpp

smstable.o: src/textwindow/smstable.cpp src/textwindow/smstable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o smstable.o src/textwindow/smstable.cpp

maintxtwindow.o: src/textwindow/maintxtwindow.cpp src/textwindow/maintxtwindow.h \
		ui_maintxtwindow.h \
		src/textwindow/textwindow.h \
		src/showwindow.h \
		src/filemanipulator/filemanipulator.h \
		src/qparser/parser.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o maintxtwindow.o src/textwindow/maintxtwindow.cpp

filemanipulator.o: src/filemanipulator/filemanipulator.cpp src/filemanipulator/filemanipulator.h \
		src/qparser/parser.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o filemanipulator.o src/filemanipulator/filemanipulator.cpp

mainwidget.o: src/mainwidget.cpp src/mainwidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwidget.o src/mainwidget.cpp

moc_controllerwindow.o: moc_controllerwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_controllerwindow.o moc_controllerwindow.cpp

moc_showwindow.o: moc_showwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_showwindow.o moc_showwindow.cpp

moc_smstable.o: moc_smstable.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_smstable.o moc_smstable.cpp

moc_maintxtwindow.o: moc_maintxtwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_maintxtwindow.o moc_maintxtwindow.cpp

moc_mainwidget.o: moc_mainwidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwidget.o moc_mainwidget.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

