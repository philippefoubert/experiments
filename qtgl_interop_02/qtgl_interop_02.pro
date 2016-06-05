#-------------------------------------------------
#
# OpenGL + QT : QOpenGLWidget
#
#-------------------------------------------------

QT      += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = qtgl_interop_02
TEMPLATE = app

SOURCES += main.cpp\
           window.cpp \
           glwidget.cpp

HEADERS += window.hpp \
           glwidget.hpp

FORMS   += window.ui
