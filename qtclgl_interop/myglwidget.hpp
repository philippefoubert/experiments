#ifndef MYGLWIDGET_HPP_
#define MYGLWIDGET_HPP_

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include <vector>
#include <QPoint>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MyGLWidget(QWidget *i_pParent = NULL);
    ~MyGLWidget() {}

public slots:
    void setCharge    (int  i_charge);
    void setLimiting  (bool i_isLimit);
    void setLowerLimit(int  i_limit);

protected:
    void initializeGL(void);
    void mouseMoveEvent(QMouseEvent *i_pEvent);
    void paintGL(void);
    void resizeGL(int i_width, int i_height);

private:
    // OpenCL
    cl::Context         m_contextCL;
    cl::CommandQueue    m_queue;
    cl::Program         m_program;
    cl::Kernel          m_kernel;
    std::vector<QPoint> m_sources;
    cl_mem              m_img;
    bool                m_isCLready;

    // OpenGL
    GLuint              m_texture;

    // Common
    int                 m_width;
    int                 m_height;

    // Algorithm parameters
    float               m_limit;
    bool                m_applyLimit;
    float               m_charge;

    int    calculateTexture(void);
    int    initializeCL(void);
    float  map(float  i_x, float i_xmin, float i_xmax, float i_ymin, float i_ymax) const;
    QPoint map(QPoint i_x, QRect i_input, QRect i_output) const;
};

#endif // MYGLWIDGET_HPP_
