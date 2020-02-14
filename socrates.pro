QT += \
    core \
    gui \
    widgets \
    network \
    multimedia \
    multimediawidgets \

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += \
    QT_DEPRECATED_WARNINGS \
    "FPS=24"

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    streamserver.cpp \
    streamthread.cpp \
    streamtranscoder.cpp \
    streamworker.cpp

HEADERS += \
    mainwindow.h \
    streamserver.h \
    streamthread.h \
    streamtranscoder.h \
    streamworker.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
