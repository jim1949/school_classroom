#-------------------------------------------------
#
# Project created by QtCreator 2015-11-17T22:22:59
#
#-------------------------------------------------

QT       += core gui network sql
RC_FILE += :/new/prefix1/image_resource/applicatoin.ICON
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = database
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    image_frame.cpp \
    classroom_treeview.cpp \
    classroom_database.cpp \
    database.cpp \
    login_dialog.cpp \
    adminstor_add.cpp \
    global_file.cpp \
    classroom_change.cpp

HEADERS  += widget.h \
    image_frame.h \
    classroom_treeview.h \
    classroom_database.h \
    database.h \
    login_dialog.h \
    adminstora_dd.h \
    global_file.h \
    classroom_change.h

FORMS    += widget.ui \
    classroom_database.ui \
    login_dialog.ui \
    adminstor_add.ui \
    classroom_change.ui

LIBS += /usr/lib/x86_64-linux-gnu/libopencv_highgui.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_core.so
RESOURCES += \
    image_resource.qrc

DISTFILES += \
    image_resource/applicatoin.ico
