#-------------------------------------------------
#
# Project created by QtCreator 2016-04-19T16:55:08
#
#-------------------------------------------------

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
RC_FILE +=:/new/prefix1/applicatoin.ico
TARGET = client
TEMPLATE = app
CONFIG += opencv


SOURCES += main.cpp\
        widget.cpp \
        image_frame.cpp \
        videodevice.cpp \
         camera.cpp \
         image_socket.cpp \
         login_dialog.cpp

HEADERS  += widget.h \
    image_frame.h \
    videodevice.h \
    camera.h \
    image_socket.h \
    login_dialog.h \
mOpenCV.h

FORMS    += widget.ui \
    login_dialog.ui


INCLUDEPATH += /usr/include/boost

LIBS += /usr/lib/x86_64-linux-gnu/libopencv_highgui.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_core.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_contrib.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so

RESOURCES += \
    image_src.qrc
