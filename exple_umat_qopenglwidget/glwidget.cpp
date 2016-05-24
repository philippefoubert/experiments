#include <iostream>

#include <QDebug>
#include <QMouseEvent>
#include <QTimer>

#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>

#include "glwidget.hpp"

IGLWidget::IGLWidget(QWidget *i_pParent)
    : QOpenGLWidget(i_pParent),
      m_width(400),
      m_height(400),
      m_pTimer(NULL)
{
    init();
    m_xRot = 0;
    m_yRot = 0;
    m_zRot = 0;
}

IGLWidget::~IGLWidget()
{
    if(m_pTimer)
    {
        m_pTimer->stop();
        m_pTimer->deleteLater();
    }
}

QSize IGLWidget::minimumSizeHint(void) const
{
    return QSize(50, 50);
}

QSize IGLWidget::sizeHint() const
{
    return QSize(m_width, m_height);
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

int IGLWidget::init(void)
{
    int res(0);

    if(NULL == m_pTimer)
    {
        m_pTimer = new QTimer(this);
        m_pTimer->setObjectName(QString("TimerVideo"));
        connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
    }

    const char video_file[] = "choux.mp4";
    m_cap.open(video_file);
    if(!m_cap.isOpened())
    {
        std::cerr << "Can not video file \'" << video_file << "\'\n" << std::flush;
        res = -1;
    }
    else
    {
        m_width  = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_WIDTH));
        m_height = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));

        const int period_ms = 20;
        m_pTimer->start(period_ms);
    }

    return res;
}

void IGLWidget::initializeGL(void)
{
    initializeOpenGLFunctions(); // Do not forget!

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_TEXTURE_2D);
//  glEnable(GL_DEPTH_TEST);

    if(cv::ocl::haveOpenCL())
    {
        (void)cv::ogl::ocl::initializeContextFromGL();
    }

    printDetails();
}

void IGLWidget::paintGL(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); // Clear the transformation matrix
    glEnable(GL_TEXTURE_2D);

//  glTranslatef(0.0f, 0.0f, -10.0f);
    glRotatef(m_xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(m_yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(m_zRot / 16.0f, 0.0f, 0.0f, 1.0f);

    draw();
}

void IGLWidget::resizeGL(int i_width, int i_height)
{
    int side = qMin(i_width, i_height);
    glViewport((i_width - side) / 2, (i_height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//#ifdef QT_OPENGL_ES_1
//    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
//#else
//    glOrtho (-2, +2, -2, +2, 1.0, 15.0);
//#endif
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
    m_texture.bind();

    // Map the texture to a surface
    glBegin(GL_QUADS);
        // Texture coordinates -> Surface coodinates
        // glTexCoord2f(s, t)  -> glVertex3f(    x,    y,     z);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, 0.1f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, 0.1f);
    glEnd();
}

int IGLWidget::getFrame(cv::UMat &o_frame)
{
    int res(0);

    cv::UMat frame_bgr;
    if(!m_cap.read(frame_bgr))
    {
        m_cap.set(cv::CAP_PROP_POS_AVI_RATIO, 0); // Restart
        if(!m_cap.read(frame_bgr))
        {
            res = -1;
        }
    }

    if(res >= 0)
    {
        cv::cvtColor(frame_bgr, o_frame, CV_RGB2RGBA);
    }
    return res;
}

int IGLWidget::onTimer(void)
{
    int res(0);

    try
    {
        cv::UMat frame;
        if(getFrame(frame) < 0)
        {
            res = -1;
        }
        else
        {
            cv::ogl::Buffer buffer;
            buffer.create(frame.rows, frame.cols, CV_8UC4,
                          cv::ogl::Buffer::PIXEL_UNPACK_BUFFER, true);

            cv::UMat result = cv::ogl::mapGLBuffer(buffer, cv::ACCESS_WRITE);
            cv::blur(frame, result, cv::Size(15, 15), cv::Point(-7, -7));
            cv::ogl::unmapGLBuffer(result);

            // buffer -> texture
            m_texture.copyFrom(buffer, true);

            update();
        }
    }
    catch(cv::Exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 10;
    }

    return res;
}

void IGLWidget::printDetails(void)
{
    std::cout << "OpenGL version : " << reinterpret_cast<const char *>(glGetString(GL_VERSION))  << '\n';
    std::cout << "OpenGL vendor  : " << reinterpret_cast<const char *>(glGetString(GL_VENDOR))   << '\n';
    std::cout << "OpenGL renderer: " << reinterpret_cast<const char *>(glGetString(GL_RENDERER)) << '\n';
    std::cout << "OpenCL device  : ";
    if(cv::ocl::useOpenCL())
    {
        std::cout << cv::ocl::Context::getDefault().device(0).name() << '\n';
    }
    else
    {
        std::cout << "No OpenCL device\n";
    }
    std::cout << std::flush;
}
