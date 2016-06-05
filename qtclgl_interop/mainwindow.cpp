#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *i_pParent) :
    QMainWindow(i_pParent),
    m_pUi(new Ui::MainWindow)
{
    m_pUi->setupUi(this);
    m_pUi->horizontalSlider->setValue(30);
    m_pUi->horizontalSlider_2->setValue(25);
    m_pUi->checkBox->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete m_pUi;
}
