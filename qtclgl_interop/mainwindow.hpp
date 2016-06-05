#ifndef MAINWINDOW_HPP_
#define MAINWINDOW_HPP_

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *i_pParent = NULL);
    ~MainWindow();

private:
    Ui::MainWindow *m_pUi;
};

#endif // MAINWINDOW_HPP_
