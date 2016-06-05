#include <fstream>
#include <iostream>
#include <QMouseEvent>

#include "myglwidget.hpp"
#include "ocl_log.hpp"

/// A callback for any OpenCL errors
static void CL_CALLBACK error_notify(const char *i_errorInfo,
                                     const void */*i_privateInfoSize*/,
                                     ::size_t    /*i_cb*/,
                                     void       */*io_pUserData*/)
{
    std::cerr << "OpenCL error notify: " << i_errorInfo << std::endl;
    exit(1);
}

// Sets up the OpenGL resources and state. Gets called once before the first
// time resizeGL() or paintGL() is called.
void MyGLWidget::initializeGL(void)
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D); // Turn on the 2D texture store
    initializeCL();
}

int MyGLWidget::initializeCL(void)
{
    int res(0);

    try
    {
        QOpenGLContext *pCtx = QOpenGLContext::currentContext();
        if(NULL == pCtx)
        {
            qWarning("Attempted CL-GL interop without a current OpenGL context");
            return -1;
        }

        // Get available platforms
        std::vector<cl::Platform> platforms;
        std::vector<cl::Device>   devices;
        cl::Platform::get(&platforms);
        if(0 == platforms.size())
        {
            qWarning("No CL platform available");
            return -1;
        }

        // Select the default platform and create a context using this platform and the GPU
        cl_context_properties cps[] =
        {
//PF        CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(),
            CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[1])(),
#if defined(WIN32) || defined(_WIN32)
            CL_GL_CONTEXT_KHR,   (cl_context_properties)wglGetCurrentContext(),
            CL_WGL_HDC_KHR,      (cl_context_properties)wglGetCurrentDC(),
#elif defined(ANDROID)
            CL_GL_CONTEXT_KHR,   (cl_context_properties)eglGetCurrentContext(),
            CL_EGL_DISPLAY_KHR,  (cl_context_properties)eglGetCurrentDisplay(),
#elif defined(__linux__)
            CL_GL_CONTEXT_KHR,   (cl_context_properties)glXGetCurrentContext(),
            CL_GLX_DISPLAY_KHR,  (cl_context_properties)glXGetCurrentDisplay(),
#endif
            0
        };

        cl_int cl_res(0);
        m_contextCL = cl::Context(CL_DEVICE_TYPE_GPU, cps, error_notify, NULL, &cl_res);
        if(0 != cl_res)
        {
            LOG_CL_ERROR(cl_res, "creating OpenCL context");
            return -1;
        }

        // Get a list of devices on this platform
        devices = m_contextCL.getInfo<CL_CONTEXT_DEVICES>();
        if(0 == devices.size())
        {
            qWarning("No CL device available for the selected platform");
            return -1;
        }

        std::cout << "OpenGL Device:"
                  << "\n  Vendor: "         << reinterpret_cast<const char *>(glGetString(GL_VENDOR))
                  << "\n  OpenGL version: " << reinterpret_cast<const char *>(glGetString(GL_VERSION))
                  << "\n  Renderer: "       << reinterpret_cast<const char *>(glGetString(GL_RENDERER))
                  << std::endl;
        logCLDeviceInfo(devices.at(0));

        // Create a command queue and use the first device
        m_queue = cl::CommandQueue(m_contextCL, devices[0]);

        // Read source file
        std::ifstream sourceFile("metaballs.cl", std::ios::in);
        if(!sourceFile)
        {
            std::cerr << __LINE__ << " - Error opening OpenCL kernel file" << std::endl;
            return -1;
        }

        std::string sourceCode(std::istreambuf_iterator<char>(sourceFile),
                               (std::istreambuf_iterator<char>()));
        cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length() + 1));

        // Make program of the source code in the context
        m_program = cl::Program(m_contextCL, source);

        // Build program for these specific devices
        m_program.build(devices);

        // Make kernel
        m_kernel = cl::Kernel(m_program, "metaballs");
        m_isCLready = true;
    }
    catch(const cl::Error &error)
    {
        LOG_CL_ERROR(error, __FUNCTION__);
        throw error;
    }

    // Fill the sources vector with a grid pattern
    m_sources.clear();
    for(int i = 0; i <= m_width; i += 100)
    {
        for(int j = 0; j <= m_height; j += 100)
        {
            m_sources.push_back(QPoint(i, j));
        }
    }

    // One last point that will be used as the mouse pointer
    m_sources.push_back(QPoint());

    // Create an OpenGL texture
    glGenTextures  (1,            &m_texture);
    glBindTexture  (GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    return res;
}

// Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget
// has been resized (and also when it is shown for the first time because all
// newly created widgets get a resize event automatically).
void MyGLWidget::resizeGL(int i_width, int i_height)
{
    glViewport(0, 0, i_width, i_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::setCharge(int i_charge)
{
    m_charge = map(i_charge, 0.f, 100.f, 1.f, 100.f);
    update();
}

void MyGLWidget::setLowerLimit(int i_limit)
{
    m_limit = map(i_limit, 0.f, 100.f, 0.001f, 0.5f);
    update();
}

void MyGLWidget::setLimiting(bool i_isLimit)
{
    m_applyLimit = i_isLimit;
    update();
}

MyGLWidget::MyGLWidget(QWidget *i_pParent)
    : QOpenGLWidget(i_pParent),
      m_isCLready(false),
      m_width    (800),
      m_height   (600)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    setFormat(format);

    setMouseTracking(true);
}

int MyGLWidget::calculateTexture(void)
{
    int res(0);

    try
    {
        size_t num = m_sources.size();

        // Create a memory buffer on the GPU
        cl::Buffer bufferSources = cl::Buffer(m_contextCL, CL_MEM_READ_ONLY, num * sizeof(cl_int2));
        // Create a memory buffer on the CPU
        cl_int2 *sourcePoints = new cl_int2[num];
        // Fill the buffer with the coordinates of the source points
        for(size_t i = 0; i < num; i++)
        {
            sourcePoints[i].s[0] = m_sources.at(i).x();
            sourcePoints[i].s[1] = m_sources.at(i).y();
        }

        cl_int cl_res(0);
        // Create an OpenCL handle from an OpenGL texture
        m_img = clCreateFromGLTexture(m_contextCL(), CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, m_texture, &cl_res);
        if(0 != cl_res)
        {
            LOG_CL_ERROR(cl_res, "clCreateFromGLTexture");
            return -1;
        }

        // Create an image from the texture
        cl::Image2D output(m_img);

        //-----------------------------------------------------------------------------
        //  Lock texture
        //-----------------------------------------------------------------------------

        // Tell OpenGL to finish so that we can get to its texture
        glFinish(); // Prior to calling clEnqueueAcquireGLObjects, the application must ensure that any
                    // pending GL operations which access the objects specified in mem_objects have completed.

        std::vector<cl::Memory> objs;      // Create vector of GL objects to lock
        objs.push_back(cl::Memory(m_img)); // Add created CL texture buffer

        // Ask OpenGL to release the object contaning our image
        m_queue.enqueueAcquireGLObjects(&objs);

        //-----------------------------------------------------------------------------
        //  Run queue
        //-----------------------------------------------------------------------------

        // Copy the buffer with the source points
        cl_res = m_queue.enqueueWriteBuffer(bufferSources, CL_TRUE, 0, num * sizeof(cl_int2), sourcePoints);
        // Now we can delete the buffer from the CPU side
        delete[] sourcePoints;
        if(0 != cl_res)
        {
            LOG_CL_ERROR(cl_res, "enqueueWriteBuffer");
            return -1;
        }

        // Set arguments to kernel
        cl_res += m_kernel.setArg(0, bufferSources);
        cl_res += m_kernel.setArg(1, cl_int(num));
        cl_res += m_kernel.setArg(2, cl_float(m_charge));
        cl_res += m_kernel.setArg(3, cl_float(m_limit));
        cl_res += m_kernel.setArg(4, cl_int(m_applyLimit));
        cl_res += m_kernel.setArg(5, output);
        // Run the kernel on specific ND range
        cl::NDRange global(m_width, m_height);
        m_queue.enqueueNDRangeKernel(m_kernel, cl::NullRange, global, cl::NullRange);

        //-----------------------------------------------------------------------------
        //  Unlock
        //-----------------------------------------------------------------------------

        // Ask OpenCL to release the objects
//FIXME
//      cl::Event releaseEvent; // The application is responsible for ensuring that any pending OpenCL operations
//                              // which access the objects specified in mem_objects have completed prior to
//                              // executing subsequent GL commands which reference these objects.
//      m_queue.enqueueReleaseGLObjects(&objs, NULL, &releaseEvent);
//        objs.clear();
//        m_queue.flush();
//        m_queue.finish();
//        releaseEvent.wait();
//        if(0 != cl_res)
//        {
//            LOG_CL_ERROR(cl_res, "enqueueReleaseGLObjects");
//            // return -1;
//        }

        // Bind the texture so that it will be used in OpenGL
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }
    catch(const cl::Error &error)
    {
        LOG_CL_ERROR(error, __FUNCTION__);
        throw error;
    }

    return res;
}

// Renders the OpenGL scene. Gets called whenever the widget needs to be updated.
void MyGLWidget::paintGL(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();        // Clear the transformation matrix

    if(m_isCLready)
    {
        calculateTexture();
    }

    // Map the texture to a surface
    glBegin(GL_QUADS);
        // Texture coordinates -> Surface coodinates
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 1.0f);
    glEnd();
}

float MyGLWidget::map(float i_x, float i_xmin, float i_xmax, float i_ymin, float i_ymax) const
{
    return i_ymin + (i_x - i_xmin) * (i_ymax - i_ymin) / (i_xmax - i_xmin);
}

QPoint MyGLWidget::map(QPoint i_x, QRect i_input, QRect i_output) const
{
    QPoint pt;
    pt.setX(map(i_x.x(), i_input.left(),   i_input.right(), i_output.left(),   i_output.right()));
    pt.setY(map(i_x.y(), i_input.bottom(), i_input.top(),   i_output.bottom(), i_output.top()));
    return pt;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *i_pEvent)
{
    m_sources.back() = map(i_pEvent->pos(), QRect(0, 0, size().width(), size().height()),
                                            QRect(0, 0, m_width,        m_height));

    // Schedule a paint event
    update();
}
