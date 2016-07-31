TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    audio.c \
    clientudp.c \
    clienttcp.c \
    servicetransmit.c

HEADERS += \
    audio.h \
    main.h \
    clientudp.h \
    clienttcp.h \
    servicetransmit.h

LIBS += -lpthread

DISTFILES +=

