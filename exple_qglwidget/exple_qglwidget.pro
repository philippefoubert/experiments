#-------------------------------------------------
#
# OpenGL + QT : QGLWidget
#
#-------------------------------------------------

QT      += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = exple_qglwidget
TEMPLATE = app


SOURCES += main.cpp\
           window.cpp \
           glwidget.cpp

HEADERS += window.hpp \
           glwidget.hpp

FORMS   += window.ui
