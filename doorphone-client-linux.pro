TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    audio.c \
    clientudp.c

HEADERS += \
    audio.h \
    main.h \
    clientudp.h

LIBS += -lpthread

