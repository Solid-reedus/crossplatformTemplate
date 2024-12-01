#include "lnx_window.h"
#include "../shared/common.h"
#include <iostream>

#include <X11/Xlib.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>


LnxWindow::LnxWindow() 
{
    m_display = XOpenDisplay(nullptr);
    if (!m_display) 
    {
        std::cerr << "Failed to open X display" << std::endl;
        return;
    }

    int screen = DefaultScreen(m_display);
    Window root = RootWindow(m_display, screen);

    XSetWindowAttributes swa;
    swa.event_mask = ExposureMask | KeyPressMask;

    m_window = XCreateWindow(
        m_display, root,
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
        0, CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &swa);

    if (!m_window) 
    {
        std::cerr << "Failed to create X11 window" << std::endl;
        XCloseDisplay(m_display);
        return;
    }

    XStoreName(m_display, m_window, WINDOW_NAME);
    XMapWindow(m_display, m_window);

    if (!InitializeEGL()) 
    {
        std::cerr << "Failed to initialize EGL" << std::endl;
        Cleanup();
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void LnxWindow::Clear()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool LnxWindow::InitializeEGL() 
{
    m_eglDisplay = eglGetDisplay((EGLNativeDisplayType)m_display);
    if (m_eglDisplay == EGL_NO_DISPLAY) {
        std::cerr << "Failed to get EGL display" << std::endl;
        return false;
    }

    if (!eglInitialize(m_eglDisplay, nullptr, nullptr)) 
    {
        std::cerr << "Failed to initialize EGL" << std::endl;
        return false;
    }

    EGLint configAttributes[] = 
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(m_eglDisplay, configAttributes, &config, 1, &numConfigs)) 
    {
        std::cerr << "Failed to choose EGL config" << std::endl;
        return false;
    }

    // Create an EGL window surface
    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, (EGLNativeWindowType)m_window, nullptr);
    if (m_eglSurface == EGL_NO_SURFACE) 
    {
        std::cerr << "Failed to create EGL surface" << std::endl;
        return false;
    }

    // Create an EGL context
    EGLint contextAttributes[] = 
    {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, contextAttributes);
    if (m_eglContext == EGL_NO_CONTEXT) 
    {
        std::cerr << "Failed to create EGL context" << std::endl;
        return false;
    }

    // Make the EGL context current
    if (!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)) 
    {
        std::cerr << "Failed to make EGL context current" << std::endl;
        return false;
    }

    const GLubyte* version = glGetString(GL_VERSION);
    printf("GL Version: %s\n", version);


    return true;
}

void LnxWindow::Update() 
{
    XEvent event;
    while (XPending(m_display)) 
    {
        XNextEvent(m_display, &event);

    }

    // Swap buffers
    eglSwapBuffers(m_eglDisplay, m_eglSurface);

}

bool LnxWindow::Init()
{
    return true;
}

void LnxWindow::Cleanup() 
{
    if (m_eglDisplay != EGL_NO_DISPLAY) 
    {
        eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (m_eglContext != EGL_NO_CONTEXT) eglDestroyContext(m_eglDisplay, m_eglContext);
        if (m_eglSurface != EGL_NO_SURFACE) eglDestroySurface(m_eglDisplay, m_eglSurface);
        eglTerminate(m_eglDisplay);
    }
    if (m_window) XDestroyWindow(m_display, m_window);
    if (m_display) XCloseDisplay(m_display);
}
