#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>

#include "window.hpp"

int main(int i_argc, char *i_argv[])
{
    QApplication app(i_argc, i_argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    IWindow mainWin;
    mainWin.resize(mainWin.sizeHint());
    int desktopArea = QApplication::desktop()->width() *
                      QApplication::desktop()->height();
    int widgetArea  = mainWin.width() * mainWin.height();

    mainWin.setWindowTitle("OpenGL + QT - Example with QOpenGLWidget");

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
