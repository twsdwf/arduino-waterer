#############################################################################
# Makefile for building: watering_analizer
# Generated by qmake (2.01a) (Qt 4.8.3) on: Thu Jan 2 14:28:19 2014
# Project:  watering_analizer.pro
# Template: app
# Command: /usr/bin/qmake-qt4 -spec /usr/share/qt4/mkspecs/linux-g++ -o Makefile watering_analizer.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I/usr/include/qwt -I. -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu -L/usr/lib -lqwt -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		com/qextserialport.cpp \
		com/qextserialenumerator_unix.cpp \
		com/posix_qextserialport.cpp \
		analizedialog.cpp \
		dosersetup.cpp \
		makeconfig.cpp \
		globalstatedialog.cpp moc_qextserialenumerator.cpp \
		moc_qextserialport.cpp \
		moc_analizedialog.cpp \
		moc_dosersetup.cpp \
		moc_makeconfig.cpp \
		moc_globalstatedialog.cpp \
		qrc_resources.cpp
OBJECTS       = main.o \
		qextserialport.o \
		qextserialenumerator_unix.o \
		posix_qextserialport.o \
		analizedialog.o \
		dosersetup.o \
		makeconfig.o \
		globalstatedialog.o \
		moc_qextserialenumerator.o \
		moc_qextserialport.o \
		moc_analizedialog.o \
		moc_dosersetup.o \
		moc_makeconfig.o \
		moc_globalstatedialog.o \
		qrc_resources.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_phonon.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		watering_analizer.pro
QMAKE_TARGET  = watering_analizer
DESTDIR       = 
TARGET        = watering_analizer

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

$(TARGET): ui_analizedialog.h ui_dosersetup.h ui_makeconfig.h ui_globalstatedialog.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: watering_analizer.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_phonon.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/i386-linux-gnu/libQtGui.prl \
		/usr/lib/i386-linux-gnu/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -o Makefile watering_analizer.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_phonon.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -o Makefile watering_analizer.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/watering_analizer1.0.0 || $(MKDIR) .tmp/watering_analizer1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/watering_analizer1.0.0/ && $(COPY_FILE) --parents com/qextserialenumerator.h com/qextserialport_global.h com/qextserialport.h analizedialog.h data_structs.h dosersetup.h makeconfig.h globalstatedialog.h .tmp/watering_analizer1.0.0/ && $(COPY_FILE) --parents resources.qrc .tmp/watering_analizer1.0.0/ && $(COPY_FILE) --parents main.cpp com/qextserialport.cpp com/qextserialenumerator_unix.cpp com/posix_qextserialport.cpp analizedialog.cpp dosersetup.cpp makeconfig.cpp globalstatedialog.cpp .tmp/watering_analizer1.0.0/ && $(COPY_FILE) --parents analizedialog.ui dosersetup.ui makeconfig.ui globalstatedialog.ui .tmp/watering_analizer1.0.0/ && (cd `dirname .tmp/watering_analizer1.0.0` && $(TAR) watering_analizer1.0.0.tar watering_analizer1.0.0 && $(COMPRESS) watering_analizer1.0.0.tar) && $(MOVE) `dirname .tmp/watering_analizer1.0.0`/watering_analizer1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/watering_analizer1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_qextserialenumerator.cpp moc_qextserialport.cpp moc_analizedialog.cpp moc_dosersetup.cpp moc_makeconfig.cpp moc_globalstatedialog.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_qextserialenumerator.cpp moc_qextserialport.cpp moc_analizedialog.cpp moc_dosersetup.cpp moc_makeconfig.cpp moc_globalstatedialog.cpp
moc_qextserialenumerator.cpp: com/qextserialport_global.h \
		com/qextserialenumerator.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) com/qextserialenumerator.h -o moc_qextserialenumerator.cpp

moc_qextserialport.cpp: com/qextserialport_global.h \
		com/qextserialport.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) com/qextserialport.h -o moc_qextserialport.cpp

moc_analizedialog.cpp: data_structs.h \
		analizedialog.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) analizedialog.h -o moc_analizedialog.cpp

moc_dosersetup.cpp: com/qextserialport.h \
		com/qextserialport_global.h \
		dosersetup.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) dosersetup.h -o moc_dosersetup.cpp

moc_makeconfig.cpp: com/qextserialport.h \
		com/qextserialport_global.h \
		makeconfig.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) makeconfig.h -o moc_makeconfig.cpp

moc_globalstatedialog.cpp: com/qextserialport.h \
		com/qextserialport_global.h \
		globalstatedialog.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) globalstatedialog.h -o moc_globalstatedialog.cpp

compiler_rcc_make_all: qrc_resources.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_resources.cpp
qrc_resources.cpp: resources.qrc \
		res/search.png \
		res/find.png \
		res/connect.png \
		res/save.png \
		res/download.png \
		res/disconnect.png \
		res/transmit.png \
		res/apply.png \
		res/open.png
	/usr/bin/rcc -name resources resources.qrc -o qrc_resources.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_analizedialog.h ui_dosersetup.h ui_makeconfig.h ui_globalstatedialog.h
compiler_uic_clean:
	-$(DEL_FILE) ui_analizedialog.h ui_dosersetup.h ui_makeconfig.h ui_globalstatedialog.h
ui_analizedialog.h: analizedialog.ui
	/usr/bin/uic-qt4 analizedialog.ui -o ui_analizedialog.h

ui_dosersetup.h: dosersetup.ui
	/usr/bin/uic-qt4 dosersetup.ui -o ui_dosersetup.h

ui_makeconfig.h: makeconfig.ui
	/usr/bin/uic-qt4 makeconfig.ui -o ui_makeconfig.h

ui_globalstatedialog.h: globalstatedialog.ui
	/usr/bin/uic-qt4 globalstatedialog.ui -o ui_globalstatedialog.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

main.o: main.cpp makeconfig.h \
		com/qextserialport.h \
		com/qextserialport_global.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

qextserialport.o: com/qextserialport.cpp com/qextserialport.h \
		com/qextserialport_global.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qextserialport.o com/qextserialport.cpp

qextserialenumerator_unix.o: com/qextserialenumerator_unix.cpp com/qextserialenumerator.h \
		com/qextserialport_global.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qextserialenumerator_unix.o com/qextserialenumerator_unix.cpp

posix_qextserialport.o: com/posix_qextserialport.cpp com/qextserialport.h \
		com/qextserialport_global.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o posix_qextserialport.o com/posix_qextserialport.cpp

analizedialog.o: analizedialog.cpp analizedialog.h \
		data_structs.h \
		ui_analizedialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o analizedialog.o analizedialog.cpp

dosersetup.o: dosersetup.cpp dosersetup.h \
		com/qextserialport.h \
		com/qextserialport_global.h \
		ui_dosersetup.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o dosersetup.o dosersetup.cpp

makeconfig.o: makeconfig.cpp makeconfig.h \
		com/qextserialport.h \
		com/qextserialport_global.h \
		ui_makeconfig.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o makeconfig.o makeconfig.cpp

globalstatedialog.o: globalstatedialog.cpp globalstatedialog.h \
		com/qextserialport.h \
		com/qextserialport_global.h \
		ui_globalstatedialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o globalstatedialog.o globalstatedialog.cpp

moc_qextserialenumerator.o: moc_qextserialenumerator.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_qextserialenumerator.o moc_qextserialenumerator.cpp

moc_qextserialport.o: moc_qextserialport.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_qextserialport.o moc_qextserialport.cpp

moc_analizedialog.o: moc_analizedialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_analizedialog.o moc_analizedialog.cpp

moc_dosersetup.o: moc_dosersetup.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_dosersetup.o moc_dosersetup.cpp

moc_makeconfig.o: moc_makeconfig.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_makeconfig.o moc_makeconfig.cpp

moc_globalstatedialog.o: moc_globalstatedialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_globalstatedialog.o moc_globalstatedialog.cpp

qrc_resources.o: qrc_resources.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_resources.o qrc_resources.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

