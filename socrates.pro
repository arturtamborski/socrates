#############################
QT +=                       \
    core                    \
    gui                     \
    network                 \
    widgets                 \
    multimedia              \
    multimediawidgets       \

#############################
CONFIG +=                   \
    c++11                   \

#############################
DEFINES +=                  \
    QT_DEPRECATED_WARNINGS  \
    FPS=24                  \
    PORT=2564               \

#############################
SOURCES +=                  \
    main.cpp                \
    mainwindow.cpp          \
    server.cpp              \
    thread.cpp              \
    transcoder.cpp          \
    worker.cpp              \

#############################
HEADERS +=                  \
    mainwindow.h            \
    server.h                \
    strings.h               \
    thread.h                \
    transcoder.h            \
    worker.h                \


INCLUDEPATH +=              \
    /usr/local/include/opencv4/

#INCLUDEPATH += 				\
#    $$(OPENCV_SDK_DIR)/include/opencv4/

#    -L$$(OPENCV_SDK_DIR)/x86/mingw/lib \
LIBS +=                  	\
    -lopencv_core           \
    -lopencv_dnn            \
    -lopencv_highgui        \
    -lopencv_imgproc        \

#############################
FORMS +=                    \
    mainwindow.ui           \

#############################
INSTALLS +=                 \
    target                  \
