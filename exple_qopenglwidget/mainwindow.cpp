#include <QCloseEvent>
#include <QMessageBox>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "glwidget.hpp"

MainWindow::MainWindow(QWidget *i_pParent) :
    QMainWindow(i_pParent),
    m_pUi(new Ui::MainWindow)
{
    m_pUi->setupUi(this);
    m_red = m_green = m_blue = 0;
}

MainWindow::~MainWindow()
{
    delete m_pUi;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// autoconnect callback functions

void MainWindow::on_xRotSlider_valueChanged(int i_value)
{
    m_pUi->xRotSpin->setValue(i_value);
    m_pUi->glwidget->setXRot(i_value);
}

void MainWindow::on_yRotSlider_valueChanged(int i_value)
{
    m_pUi->yRotSpin->setValue(i_value);
    m_pUi->glwidget->setYRot(i_value);
}

void MainWindow::on_zRotSlider_valueChanged(int i_value)
{
    m_pUi->zRotSpin->setValue(i_value);
    m_pUi->glwidget->setZRot(i_value);
}

void MainWindow::on_xRotSpin_valueChanged(int i_value)
{
    m_pUi->xRotSlider->setValue(i_value);
    m_pUi->glwidget->setXRot(i_value);
}

void MainWindow::on_yRotSpin_valueChanged(int i_value)
{
    m_pUi->yRotSlider->setValue(i_value);
    m_pUi->glwidget->setYRot(i_value);
}

void MainWindow::on_zRotSpin_valueChanged(int i_value)
{
    m_pUi->zRotSlider->setValue(i_value);
    m_pUi->glwidget->setZRot(i_value);
}

void MainWindow::on_redSlider_valueChanged(int i_value)
{
    m_red = i_value/100.;
    m_pUi->redSpin->setValue(i_value);
    m_pUi->glwidget->setColor(m_red, m_green, m_blue);
}

void MainWindow::on_greenSlider_valueChanged(int i_value)
{
    m_green = i_value/100.;
    m_pUi->greenSpin->setValue(i_value);
    m_pUi->glwidget->setColor(m_red, m_green, m_blue);
}

void MainWindow::on_blueSlider_valueChanged(int i_value)
{
    m_blue = i_value/100.;
    m_pUi->blueSpin->setValue(i_value);
    m_pUi->glwidget->setColor(m_red, m_green, m_blue);
}

void MainWindow::on_redSpin_valueChanged(int i_value)
{
    m_red = i_value/100.;
    m_pUi->redSlider->setValue(i_value);
    m_pUi->glwidget->setColor(m_red, m_green, m_blue);
}

void MainWindow::on_greenSpin_valueChanged(int i_value)
{
    m_green = i_value/100.;
    m_pUi->greenSlider->setValue(i_value);
    m_pUi->glwidget->setColor(m_red, m_green, m_blue);
}

void MainWindow::on_blueSpin_valueChanged(int i_value)
{
    m_blue = i_value/100.;
    m_pUi->blueSlider->setValue(i_value);
    m_pUi->glwidget->setColor(m_red, m_green, m_blue);
}

void MainWindow::closeEvent(QCloseEvent *i_pEvent)
{
    i_pEvent->ignore();
    quitApp();
}

void MainWindow::quitApp(void)
{
    int ret = QMessageBox::critical(this,
                                    tr("Quit Application"),
                                    tr("Do you want to quit?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) QApplication::quit();
}
