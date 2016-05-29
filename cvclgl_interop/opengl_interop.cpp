// Sample demonstrating interoperability of OpenCV UMat with OpenGL texture.
// At first, the data obtained from video file or camera and placed onto
// OpenGL texture, following mapping of this OpenGL texture to OpenCV UMat
// and call cv::blur function. The result is mapped back to OpenGL texture
// and rendered through OpenGL API.

#if defined(WIN32) || defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#elif defined(__linux__)
# include <X11/X.h>
# include <X11/Xlib.h>
#endif

#include <iostream>
#include <queue>
#include <string>

#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/core/opengl.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "winapp.hpp"

#if defined(WIN32) || defined(_WIN32)
# pragma comment(lib, "opengl32.lib")
# pragma comment(lib, "glu32.lib")
#endif

class GLWinApp : public WinApp
{
public:
    enum MODE
    {
        MODE_CPU = 0,
        MODE_GPU
    };

    GLWinApp(int i_width, int i_height, const std::string &i_window_name, cv::VideoCapture &io_cap) :
        WinApp(i_width, i_height, i_window_name)
    {
        m_isShutdown   = false;
        m_isBuffer     = false;
        m_isProcessing = true;
        m_mode         = MODE_CPU;
        m_modeStr[0]   = cv::String("Processing on CPU");
        m_modeStr[1]   = cv::String("Processing on GPU");
        m_cap          = io_cap;
    }

    ~GLWinApp() {}

    virtual void cleanup(void)
    {
        m_isShutdown = true;
#if defined(__linux__)
        glXMakeCurrent(m_display, None, NULL);
        glXDestroyContext(m_display, m_glctx);
#endif
        WinApp::cleanup();
    }

#if defined(WIN32) || defined(_WIN32)
    virtual LRESULT CALLBACK WndProc(HWND i_hWnd, UINT i_message, WPARAM i_wParam, LPARAM i_lParam)
    {
        switch(i_message)
        {
        case WM_CHAR:
            if     (i_wParam == '1')       { m_mode = MODE_CPU;                return 0; }
            else if(i_wParam == '2')       { m_mode = MODE_GPU;                return 0; }
            else if(i_wParam == '9')       { m_isBuffer = !m_isBuffer;         return 0; }
            else if(i_wParam == VK_SPACE)  { m_isProcessing = !m_isProcessing; return 0; }
            else if(i_wParam == VK_ESCAPE) { cleanup();                        return 0; }
            break;

        case WM_CLOSE:
            cleanup();
            return 0;

        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        }

        return ::DefWindowProc(i_hWnd, i_message, i_wParam, i_lParam);
    }
#endif

#if defined(__linux__)
    int handle_event(XEvent &i_event)
    {
        switch(i_event.type)
        {
        case ClientMessage:
            if((Atom)i_event.xclient.data.l[0] == m_WM_DELETE_WINDOW)
            {
                m_end_loop = true;
                cleanup();
            }
            else
            {
                return 0;
            }
            break;
        case Expose:
            render();
            break;
        case KeyPress:
            switch(keycode_to_keysym(i_event.xkey.keycode))
            {
            case XK_space:
                m_demo_processing = !m_demo_processing;
                break;
            case XK_1:
                m_mode = MODE_CPU;
                break;
            case XK_2:
                m_mode = MODE_GPU;
                break;
            case XK_9:
                m_isBuffer = !m_isBuffer;
                break;
            case XK_Escape:
                m_end_loop = true;
                cleanup();
                break;
            }
            break;
        default:
            return 0;
        }
        return 1;
    }
#endif

    int init(void)
    {
#if defined(WIN32) || defined(_WIN32)
        m_hDC = GetDC(m_hWnd);

        if(setup_pixel_format() < 0)
        {
            std::cerr << "Can't setup pixel format" << std::endl;
            return -1;
        }

        m_hRC = wglCreateContext(m_hDC);
        wglMakeCurrent(m_hDC, m_hRC);
#elif defined(__linux__)
        m_glctx = glXCreateContext(m_display, m_visual_info, NULL, GL_TRUE);
        glXMakeCurrent(m_display, m_window, m_glctx);
#endif

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, m_width, m_height);

        if(cv::ocl::haveOpenCL())
        {
            (void)cv::ogl::ocl::initializeContextFromGL();
        }

        m_oclDevName = cv::ocl::useOpenCL() ?
            cv::ocl::Context::getDefault().device(0).name() :
            (char *)"No OpenCL device";

        return 0;
    } // init()

    int get_frame(cv::ogl::Texture2D &o_texture, cv::ogl::Buffer &o_buffer)
    {
        int res(0);

        if(!m_cap.read(m_frame_bgr))
        {
            m_cap.set(cv::CAP_PROP_POS_AVI_RATIO, 0); // Restart
            if(!m_cap.read(m_frame_bgr))
            {
                res = -1;
            }
        }

        if(res >= 0)
        {
            cv::cvtColor(m_frame_bgr, m_frame_rgba, CV_RGB2RGBA);

            if(m_isBuffer) { o_buffer.copyFrom(m_frame_rgba, cv::ogl::Buffer::PIXEL_UNPACK_BUFFER, true); }
            else           { o_texture.copyFrom(m_frame_rgba, true);                                      }
        }
        return res;
    }

    void print_info(float i_time, const cv::String &i_oclDevName)
    {
#if defined(WIN32) || defined(_WIN32)
        HDC   hDC      = m_hDC;
        HFONT hFont    = (HFONT)::GetStockObject(SYSTEM_FONT);
        HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);
        if(hOldFont)
        {
            TEXTMETRIC tm;
            ::GetTextMetrics(hDC, &tm);

            char buf[256];
            int  y = 0;

            snprintf(buf, 255, "Mode: %s%s", m_modeStr[m_mode].c_str(), (MODE_GPU == m_mode) ? (m_isBuffer ? " OpenGL buffer" : " OpenGL texture") : "");
            ::TextOut(hDC, 0, y, buf, (int)strlen(buf));

            y += tm.tmHeight;
            snprintf(buf, 255, "Time, msec: %2.1f", i_time);
            ::TextOut(hDC, 0, y, buf, (int)strlen(buf));

            y += tm.tmHeight;
            snprintf(buf, 255, "OpenCL device: %s", i_oclDevName.c_str());
            ::TextOut(hDC, 0, y, buf, (int)strlen(buf));

            ::SelectObject(hDC, hOldFont);
        }
#elif defined(__linux__)
        char buf[256];
        snprintf(buf, 255, "Time, msec: %2.1f, Mode: %s OpenGL %s, Device: %s", i_time, m_modeStr[m_mode].c_str(), m_isBuffer ? "buffer" : "texture", i_oclDevName.c_str());
        XStoreName(m_display, m_window, buf);
#endif
    }

    void idle(void)
    {
        render();
    }

    int render(void)
    {
        try
        {
            if(m_isShutdown)
                return 0;

            int res;
            cv::ogl::Texture2D texture;
            cv::ogl::Buffer    buffer;

            texture.setAutoRelease(true);
            buffer.setAutoRelease(true);

            res = get_frame(texture, buffer);
            if(res != 0)
            {
                return -1;
            }

            switch(m_mode)
            {
                case MODE_CPU: // process frame on CPU
                    processFrameCPU(texture, buffer);
                    break;

                case MODE_GPU: // process frame on GPU
                    processFrameGPU(texture, buffer);
                    break;
            } // switch

            if(m_isBuffer) // buffer -> texture
            {
                cv::Mat mat(m_height, m_width, CV_8UC4);
                buffer.copyTo(mat);
                texture.copyFrom(mat, true);
            }

#if defined(__linux__)
            XWindowAttributes window_attributes;
            XGetWindowAttributes(m_display, m_window, &window_attributes);
            glViewport(0, 0, window_attributes.width, window_attributes.height);
#endif

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadIdentity();
            glEnable(GL_TEXTURE_2D);

            texture.bind();

            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, 0.1f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.1f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.1f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, 0.1f);
            glEnd();

#if defined(WIN32) || defined(_WIN32)
            SwapBuffers(m_hDC);
#elif defined(__linux__)
            glXSwapBuffers(m_display, m_window);
#endif

            print_info(m_timer.time(Timer::MSEC), m_oclDevName);
        }
        catch(cv::Exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
            return 10;
        }

        return 0;
    }

protected:

    void processFrameCPU(cv::ogl::Texture2D &io_texture, cv::ogl::Buffer &io_buffer)
    {
        cv::Mat mat(m_height, m_width, CV_8UC4);

        m_timer.start();

        if(m_isBuffer) { io_buffer.copyTo(mat);  }
        else           { io_texture.copyTo(mat); }

        // blur texture image with OpenCV on CPU
        if(m_isProcessing) { cv::blur(mat, mat, cv::Size(15, 15), cv::Point(-7, -7)); }

        if(m_isBuffer) { io_buffer.copyFrom(mat, cv::ogl::Buffer::PIXEL_UNPACK_BUFFER, true); }
        else           { io_texture.copyFrom(mat, true);                                      }

        m_timer.stop();
    }

    void processFrameGPU(cv::ogl::Texture2D &io_texture, cv::ogl::Buffer &io_buffer)
    {
        cv::UMat umat;

        m_timer.start();

        if(m_isBuffer) { umat = cv::ogl::mapGLBuffer(io_buffer);            }
        else           { cv::ogl::convertFromGLTexture2D(io_texture, umat); }

        // blur texture image with OpenCV on GPU with OpenCL
        if(m_isProcessing) { cv::blur(umat, umat, cv::Size(15, 15), cv::Point(-7, -7)); }

        if(m_isBuffer) { cv::ogl::unmapGLBuffer(umat);                    }
        else           { cv::ogl::convertToGLTexture2D(umat, io_texture); }

        m_timer.stop();
    }

#if defined(WIN32) || defined(_WIN32)
    int setup_pixel_format(void)
    {
        int res(0);
        PIXELFORMATDESCRIPTOR  pfd;

        pfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion        = 1;
        pfd.dwFlags         = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL  | PFD_DOUBLEBUFFER;
        pfd.iPixelType      = PFD_TYPE_RGBA;
        pfd.cColorBits      = 24;
        pfd.cRedBits        = 8;
        pfd.cRedShift       = 0;
        pfd.cGreenBits      = 8;
        pfd.cGreenShift     = 0;
        pfd.cBlueBits       = 8;
        pfd.cBlueShift      = 0;
        pfd.cAlphaBits      = 8;
        pfd.cAlphaShift     = 0;
        pfd.cAccumBits      = 0;
        pfd.cAccumRedBits   = 0;
        pfd.cAccumGreenBits = 0;
        pfd.cAccumBlueBits  = 0;
        pfd.cAccumAlphaBits = 0;
        pfd.cDepthBits      = 24;
        pfd.cStencilBits    = 8;
        pfd.cAuxBuffers     = 0;
        pfd.iLayerType      = PFD_MAIN_PLANE;
        pfd.bReserved       = 0;
        pfd.dwLayerMask     = 0;
        pfd.dwVisibleMask   = 0;
        pfd.dwDamageMask    = 0;

        int pfmt = ChoosePixelFormat(m_hDC, &pfd);
        if(0 == pfmt)
        {
            res = -1;
        }
        else if(0 == SetPixelFormat(m_hDC, pfmt, &pfd))
        {
            res = -2;
        }

        return res;
    }
#endif

#if defined(__linux__)
    KeySym keycode_to_keysym(unsigned i_keycode)
    {   // note that XKeycodeToKeysym() is considered deprecated
        int keysyms_per_keycode_return = 0;
        KeySym *keysyms = XGetKeyboardMapping(m_display, i_keycode, 1, &keysyms_per_keycode_return);
        KeySym keysym = keysyms[0];
        XFree(keysyms);
        return keysym;
    }
#endif

private:
    bool               m_isBuffer; //<! if OpenGL, shall we use buffers?
    bool               m_isProcessing;
    bool               m_isShutdown;
    MODE               m_mode;
    cv::String         m_modeStr[2];
#if defined(WIN32) || defined(_WIN32)
    HDC                m_hDC;
    HGLRC              m_hRC;
#elif defined(__linux__)
    GLXContext         m_glctx;
#endif
    cv::VideoCapture   m_cap;
    cv::Mat            m_frame_bgr;
    cv::Mat            m_frame_rgba;
    cv::String         m_oclDevName;
};

static void help(void)
{
    printf("\nSample demonstrating interoperability of OpenGL and OpenCL with OpenCV.\n"
           "Hot keys: \n"
           "  SPACE - turn processing on/off\n"
           "    1   - process GL data through OpenCV on CPU\n"
           "    2   - process GL data through OpenCV on GPU (via OpenCL)\n"
           "    9   - toggle use of GL texture/GL buffer\n"
           "  ESC   - exit\n\n");
}

int main(int i_argc, char** i_argv)
{
    const char video_file[] = "choux.mp4";

    help();

    cv::VideoCapture cap;
    cap.open(video_file);
    if(!cap.isOpened())
    {
        printf("Can not open video file \'%s\'\n", video_file);
        return -1;
    }

    int width  = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

#if defined(WIN32) || defined(_WIN32)
    std::string wndname = "WGL Window";
#elif defined(__linux__)
    std::string wndname = "GLX Window";
#endif

    GLWinApp app(width, height, wndname, cap);
    try
    {
        app.create();
        return app.run();
    }
    catch(cv::Exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 10;
    }
    catch(...)
    {
        std::cerr << "FATAL ERROR: Unknown exception" << std::endl;
        return 11;
    }
}
