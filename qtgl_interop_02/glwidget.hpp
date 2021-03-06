#ifndef GLWIDGET_HPP_
#define GLWIDGET_HPP_

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class IGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit IGLWidget(QWidget *i_pParent = NULL);
    ~IGLWidget();

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

#endif // GLWIDGET_HPP_
