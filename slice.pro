TEMPLATE = app
TARGET = slice_test
DEPENDPATH += . src
INCLUDEPATH += . src
CONFIG += \
	release \
	warn_on \
	console
QT += \
	opengl
HEADERS += \
	src/qSlice.h
SOURCES += \
	src/qSlice.cpp \
	src/main.cpp
DESTDIR =		bin
MOC_DIR =		build/moc
UI_DIR =		build/uic
OBJECTS_DIR =	build/obj
RCC_DIR =		build/rcc
