TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    audio.c \
    sockudp.c

HEADERS += \
    audio.h \
    sockudp.h \
    main.h

