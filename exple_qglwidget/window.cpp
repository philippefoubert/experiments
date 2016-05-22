// window.cpp

#include <QKeyEvent>

#include "window.hpp"
#include "ui_window.h"

#include "myglwidget.hpp"

Window::Window(QWidget *i_pParent) :
    QWidget(i_pParent),
    m_pUi(new Ui::Window)
{
    m_pUi->setupUi(this);

    connect(m_pUi->myGLWidget, SIGNAL(xRotationChanged(int)), m_pUi->rotXSlider, SLOT(setValue(int)));
    connect(m_pUi->myGLWidget, SIGNAL(yRotationChanged(int)), m_pUi->rotYSlider, SLOT(setValue(int)));
    connect(m_pUi->myGLWidget, SIGNAL(zRotationChanged(int)), m_pUi->rotZSlider, SLOT(setValue(int)));
}

Window::~Window()
{
    delete m_pUi;
}

void Window::keyPressEvent(QKeyEvent *i_pEvent)
{
    if(Qt::Key_Escape == i_pEvent->key())
    {
        close();
    }
    else
    {
        QWidget::keyPressEvent(i_pEvent);
    }
}
