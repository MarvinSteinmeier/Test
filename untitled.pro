QT = core

CONFIG += c++17 cmdline + felgo

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        automobil_daten.cpp \
        main.cpp \
        sqlite3.c

RESOURCES +=\
            main.qml

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    automobil_daten.h \
    funktionen.h \
    sqlite3.h

DISTFILES += \
    main.qml

