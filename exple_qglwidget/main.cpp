//
//  Taken (and adapted) from http://www.bogotobogo.com/Qt/Qt5_OpenGL_QGLWidget.php
//  Note: it is using QGLWidget which is marked as deprecated... and is replaced by QOpenGLWidget
//
#include <QApplication>
#include <QDesktopWidget>

#include "window.hpp"

int main(int i_argc, char *i_argv[])
{
    QApplication app(i_argc, i_argv);
    IWindow mainWin;
    mainWin.resize(mainWin.sizeHint());
    int desktopArea = QApplication::desktop()->width() *
                      QApplication::desktop()->height();
    int widgetArea  = mainWin.width() * mainWin.height();

    mainWin.setWindowTitle("OpenGL + QT - Example with QGLWidget (deprecated)");

    if(((float)widgetArea / (float)desktopArea) < 0.75f)
    {
        mainWin.show();
    }
    else
    {
        mainWin.showMaximized();
    }

    return app.exec();
}
