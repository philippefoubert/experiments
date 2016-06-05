#-------------------------------------------------
#
# Project created by QtCreator 2015-06-14T14:08:58
#
#-------------------------------------------------

QT          += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11
TARGET   = qtclgl_interop
TEMPLATE = app

# Since nVidia doesn't support 1.2 yet
# DEFINES += CL_USE_DEPRECATED_OPENCL_1_1_APIS

# MinGW wants to compile directly against the DLL
#LIBS       += -LC:/Windows/System32/ -lOpenCL
LIBS        += C:/Windows/System32/OpenCL.dll
INCLUDEPATH += "D:\FOUBERT\projets\opensource_workspace\opencv\3rdparty\include\opencl\1.2"

SOURCES     += main.cpp       \
               mainwindow.cpp \
               myglwidget.cpp \
               ocl_log.cpp

HEADERS     += mainwindow.hpp \
               myglwidget.hpp \
               ocl_log.hpp

FORMS       += mainwindow.ui

DISTFILES   += metaballs.cl
