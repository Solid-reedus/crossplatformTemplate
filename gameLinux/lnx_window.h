#pragma once

#include "../shared/baseWindow.h"
#include <X11/Xlib.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

class LnxWindow : public BaseWindow 
{
    public:
    LnxWindow();
    void Update() override;
    bool Init() override;


    bool ShouldClose() const { return m_shouldClose; };

    private:
    Display* m_display = nullptr;
    Window m_window = 0;
    EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
    EGLContext m_eglContext = EGL_NO_CONTEXT;
    EGLSurface m_eglSurface = EGL_NO_SURFACE;

    bool m_shouldClose = false;

    bool InitializeEGL();
    void Cleanup();
};
