#include <QMouseEvent>

#include "glwidget.hpp"

IGLWidget::IGLWidget(QWidget *i_pParent)
    : QOpenGLWidget(i_pParent)
{
    m_xRot = 0;
    m_yRot = 0;
    m_zRot = 0;
}

IGLWidget::~IGLWidget()
{
}

QSize IGLWidget::minimumSizeHint(void) const
{
    return QSize(50, 50);
}

QSize IGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &io_angle)
{
    while(io_angle < 0)
    {
        io_angle += 360 * 16;
    }
    while(io_angle > 360)
    {
        io_angle -= 360 * 16;
    }
}

void IGLWidget::setXRotation(int i_angle)
{
    qNormalizeAngle(i_angle);
    if(i_angle != m_xRot)
    {
        m_xRot = i_angle;
        emit xRotationChanged(i_angle);
        update();
    }
}

void IGLWidget::setYRotation(int i_angle)
{
    qNormalizeAngle(i_angle);
    if(i_angle != m_yRot)
    {
        m_yRot = i_angle;
        emit yRotationChanged(i_angle);
        update();
    }
}

void IGLWidget::setZRotation(int i_angle)
{
    qNormalizeAngle(i_angle);
    if(i_angle != m_zRot)
    {
        m_zRot = i_angle;
        emit zRotationChanged(i_angle);
        update();
    }
}

void IGLWidget::initializeGL(void)
{
    initializeOpenGLFunctions();   // Do not forget!
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void IGLWidget::paintGL(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(m_xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(m_yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(m_zRot / 16.0, 0.0, 0.0, 1.0);
    draw();
}

void IGLWidget::resizeGL(int i_width, int i_height)
{
    int side = qMin(i_width, i_height);
    glViewport((i_width - side) / 2, (i_height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void IGLWidget::mousePressEvent(QMouseEvent *i_pEvent)
{
    m_lastPos = i_pEvent->pos();
}

void IGLWidget::mouseMoveEvent(QMouseEvent *i_pEvent)
{
    int dx = i_pEvent->x() - m_lastPos.x();
    int dy = i_pEvent->y() - m_lastPos.y();

    if(i_pEvent->buttons() & Qt::LeftButton)
    {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    }
    else if(i_pEvent->buttons() & Qt::RightButton)
    {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }

    m_lastPos = i_pEvent->pos();
}

void IGLWidget::draw(void)
{
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
        glNormal3f( 0,  0, -1);
        glVertex3f(-1, -1,  0);
        glVertex3f(-1,  1,  0);
        glVertex3f( 1,  1,  0);
        glVertex3f( 1, -1,  0);
    glEnd();

    glBegin(GL_TRIANGLES);
        glNormal3f( 0, -1, 0.707);
        glVertex3f(-1, -1, 0);
        glVertex3f( 1, -1, 0);
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
