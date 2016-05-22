//
//  Taken (and adapted) from http://perso.telecom-paristech.fr/~elc/igr201/
// 

#include <QApplication>
#include <QSurfaceFormat>
#include "mainwindow.hpp"

int main(int i_argc, char *i_argv[])
{
    QApplication app(i_argc, i_argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow mainWin;
    mainWin.setWindowTitle("OpenGL - Example with QOpenGLWidget");
    mainWin.show();
    return app.exec();
}
