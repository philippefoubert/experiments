#ifndef GLWIDGET_HPP_
#define GLWIDGET_HPP_

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <opencv2/core/opengl.hpp>
#include <opencv2/videoio.hpp>

class QTimer;

class IGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit IGLWidget(QWidget *i_pParent = NULL);
    ~IGLWidget();

protected:
    void  initializeGL(void);
    void  paintGL(void);
    void  resizeGL(int i_width, int i_height);

    QSize minimumSizeHint(void) const;
    void  mousePressEvent(QMouseEvent *i_pEvent);
    void  mouseMoveEvent (QMouseEvent *i_pEvent);
    QSize sizeHint       (void) const;

public slots:
    // slots for xyz-rotation slider
    void setXRotation(int i_angle);
    void setYRotation(int i_angle);
    void setZRotation(int i_angle);

    int  onTimer(void);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int i_angle);
    void yRotationChanged(int i_angle);
    void zRotationChanged(int i_angle);

private:
    int  getFrame    (cv::UMat &o_frame);
    int  init        (void);
    void printDetails(void);

    int    m_xRot;
    int    m_yRot;
    int    m_zRot;
    QPoint m_lastPos;

    cv::ogl::Texture2D m_texture;

    int                m_width;
    int                m_height;
    cv::VideoCapture   m_cap;
    QTimer            *m_pTimer;
};

#endif // GLWIDGET_HPP_
