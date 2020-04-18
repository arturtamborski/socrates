QT += \
    core \
    gui \
    widgets \
    network \
    multimedia \
    multimediawidgets \

CONFIG += c++11

DEFINES += \
    QT_DEPRECATED_WARNINGS \
    FPS=24 \

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    server.cpp \
    thread.cpp \
    transcoder.cpp \
    worker.cpp

HEADERS += \
    mainwindow.h \
    server.h \
    thread.h \
    transcoder.h \
    worker.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
