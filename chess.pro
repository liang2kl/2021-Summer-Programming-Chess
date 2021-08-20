QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chess.cpp \
    chessboardscene.cpp \
    chessboardview.cpp \
    chessgame.cpp \
    chessgamenetworkbase.cpp \
    chessgamenetworkclient.cpp \
    chessgamenetworkserver.cpp \
    chessgamepanelview.cpp \
    chessgameview.cpp \
    chessgraphicsitem.cpp \
    chesspoint.cpp \
    main.cpp

HEADERS += \
    chess.h \
    chessboardscene.h \
    chessboardview.h \
    chessgame.h \
    chessgamenetworkbase.h \
    chessgamenetworkclient.h \
    chessgamenetworkserver.h \
    chessgamepanelview.h \
    chessgameview.h \
    chessgraphicsitem.h \
    chesspoint.h \
    constants.h \
    dropshadoweffect.h \
    graph.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
