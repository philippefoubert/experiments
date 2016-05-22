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

private slots:
    void quitApp(void);
    void on_xRotSlider_valueChanged (int i_value);
    void on_yRotSlider_valueChanged (int i_value);
    void on_zRotSlider_valueChanged (int i_value);
    void on_xRotSpin_valueChanged   (int i_value);
    void on_yRotSpin_valueChanged   (int i_value);
    void on_zRotSpin_valueChanged   (int i_value);
    void on_redSlider_valueChanged  (int i_value);
    void on_greenSlider_valueChanged(int i_value);
    void on_blueSlider_valueChanged (int i_value);
    void on_redSpin_valueChanged    (int i_value);
    void on_greenSpin_valueChanged  (int i_value);
    void on_blueSpin_valueChanged   (int i_value);

private:
    Ui::MainWindow *m_pUi;
    float m_red, m_blue, m_green;

    void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_HPP_
