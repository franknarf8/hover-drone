CONFIG += c++11
CONFIG += console

QT += core

TARGET = testComm

SOURCES += main.cpp\
        CommSNES.cpp \
    testreceiver.cpp

HEADERS  += CommSNES.h \
    testreceiver.h

INCLUDEPATH += $$PWD/includes

win32 {
    LIBS += -L$$PWD/libs/ -lhidapi
} else {
    #LIBS += -L$$PWD/libs/ -lhidapi
}

DEPENDPATH += $$PWD/includes
