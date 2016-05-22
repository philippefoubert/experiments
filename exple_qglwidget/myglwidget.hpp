// myglwidget.hpp

#ifndef MYGLWIDGET_HPP_
#define MYGLWIDGET_HPP_

#include <QGLWidget>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit MyGLWidget(QWidget *i_pParent = NULL);
    ~MyGLWidget();

protected:
    void initializeGL(void);
    void paintGL(void);
    void resizeGL(int i_width, int i_height);

    QSize minimumSizeHint(void) const;
    QSize sizeHint(void) const;
    void mousePressEvent(QMouseEvent *i_pEvent);
    void mouseMoveEvent (QMouseEvent *i_pEvent);

public slots:
    // slots for xyz-rotation slider
    void setXRotation(int i_angle);
    void setYRotation(int i_angle);
    void setZRotation(int i_angle);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int i_angle);
    void yRotationChanged(int i_angle);
    void zRotationChanged(int i_angle);

private:
    void draw(void);

    int m_xRot;
    int m_yRot;
    int m_zRot;

    QPoint m_lastPos;
};

#endif // MYGLWIDGET_HPP_
