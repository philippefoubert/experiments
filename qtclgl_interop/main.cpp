// https://petoknm.wordpress.com/2015/06/17/qt-opengl-opencl-interoperability/
// https://github.com/petoknm/QtOpenCLGLInterop

#include <QApplication>
#include "mainwindow.hpp"

int main(int i_argc, char *i_argv[])
{
    QApplication a(i_argc, i_argv);
    MainWindow w;
    w.show();

    return a.exec();
}
