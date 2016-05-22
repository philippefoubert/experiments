#include <QDebug>
#include "glwidget.hpp"

GLWidget::GLWidget(QWidget *i_pParent)
    : QOpenGLWidget(i_pParent)
{
    m_red = m_green = m_blue = 0;
    m_xRot = m_yRot = m_zRot = 0;
}

GLWidget::~GLWidget()
{
}

void GLWidget::getColor(GLfloat &o_red, GLfloat &o_green, GLfloat &o_blue)
{
    o_red = m_red; o_green = m_green; o_blue = m_blue;
}

void GLWidget::setColor(GLfloat i_red, GLfloat i_green, GLfloat i_blue)
{
    if(i_red != m_red || i_green != m_green || i_blue != m_blue)
    {
        m_red = i_red; m_green = i_green; m_blue = i_blue;

        makeCurrent();
        GLfloat lightColor[4] = {m_red, m_green, m_blue, 1};
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);

        emit colorChanged(m_red, m_green, m_blue);
        update();
    }
}

void GLWidget::setXRot(GLfloat i_angle)
{
    if(i_angle != m_xRot)
    {
        m_xRot = i_angle;
        emit xRotChanged(i_angle);
        update();
    }
}

void GLWidget::setYRot(GLfloat i_angle)
{
    if(i_angle != m_yRot)
    {
        m_yRot = i_angle;
        emit yRotChanged(i_angle);
        update();
    }
}

void GLWidget::setZRot(GLfloat i_angle)
{
    if(i_angle != m_zRot)
    {
        m_zRot = i_angle;
        emit zRotChanged(i_angle);
        update();
    }
}

void GLWidget::initializeGL(void)
{
    initializeOpenGLFunctions();   // ne pas oublier !
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightPosition[4] = {0, 0, 10, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    GLfloat lightColor[4] = {m_red, m_green, m_blue, 1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);
}

void GLWidget::paintGL(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(m_xRot, 1.0, 0.0, 0.0);
    glRotatef(m_yRot, 0.0, 1.0, 0.0);
    glRotatef(m_zRot, 0.0, 0.0, 1.0);
    draw();
}

void GLWidget::resizeGL(int i_w, int i_h)
{
    int side = qMin(i_w, i_h);
    glViewport((i_w - side) / 2, (i_h - side) / 2, side, side);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::draw(void)
{
    glBegin(GL_QUADS);
    glNormal3f( 0,  0, -1);
    glVertex3f(-1, -1,  0);
    glVertex3f(-1,  1,  0);
    glVertex3f( 1,  1,  0);
    glVertex3f( 1, -1,  0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glNormal3f( 0, -1, 0.707); glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1, -1, 0);     glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f( 1, -1, 0);     glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f( 0,  0, 1.2);
    glEnd();

    glBegin(GL_TRIANGLES);
    glNormal3f( 1,  0, 0.707);
    glVertex3f( 1, -1, 0);
    glVertex3f( 1,  1, 0);
    glVertex3f( 0,  0, 1.2);
    glEnd();

    glBegin(GL_TRIANGLES);
    glNormal3f( 0,  1, 0.707);
    glVertex3f( 1,  1, 0);
    glVertex3f(-1,  1, 0);
    glVertex3f( 0,  0, 1.2);
    glEnd();

    glBegin(GL_TRIANGLES);
    glNormal3f(-1,  0, 0.707);
    glVertex3f(-1,  1, 0);
    glVertex3f(-1, -1, 0);
    glVertex3f( 0,  0, 1.2);
    glEnd();
}
