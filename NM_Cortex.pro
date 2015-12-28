TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = Cortex.cpp

SOURCES +=  Cortex_mex.cpp \
	    Cortex.cpp \
	    Cortical_Column.cpp

HEADERS +=  Cortical_Column.h \
            Random_Stream.h \
	    saves.h \
	    Stimulation.h

QMAKE_CXXFLAGS += -std=c++11 -O3

SOURCES -= Cortex_mex.cpp
