#ifndef GLWIDGET_HPP_
#define GLWIDGET_HPP_

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *i_pParent);
    ~GLWidget();

    void getColor(GLfloat &o_red, GLfloat &o_green, GLfloat &o_blue);
    GLfloat getXRot(void) const {return m_xRot;}
    GLfloat getYRot(void) const {return m_yRot;}
    GLfloat getZRot(void) const {return m_zRot;}

public slots:
    void setColor(GLfloat i_red, GLfloat i_green, GLfloat i_blue);
    void setXRot(GLfloat i_angle);
    void setYRot(GLfloat i_angle);
    void setZRot(GLfloat i_angle);

signals:
    void colorChanged(GLfloat i_red, GLfloat i_green, GLfloat i_blue);
    void xRotChanged(GLfloat i_angle);
    void yRotChanged(GLfloat i_angle);
    void zRotChanged(GLfloat i_angle);

private:
    GLfloat m_red,  m_green, m_blue;
    GLfloat m_xRot, m_yRot,  m_zRot;

    void initializeGL(void);
    void paintGL(void);
    void resizeGL(int i_w, int i_h);
    void draw(void);
};

#endif // GLWIDGET_HPP_
