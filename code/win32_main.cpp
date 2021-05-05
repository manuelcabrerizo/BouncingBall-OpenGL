#include <windows.h>
#include <glad/glad.h>
#include <GL/wglext.h>
#include <stdio.h>

// my stuff...
#include "math.h"
#include "shader.h"
#include "game.h"

#define global_variable static
#define WNDWIDTH 800
#define WNDHEIGHT 600

global_variable bool is_runnig;

LRESULT CALLBACK WindowProc(
        HWND   hwnd,
        UINT   uMsg,
        WPARAM wParam,
        LPARAM lParam)
{
    LRESULT result;
    switch(uMsg)
    {
        case WM_CLOSE:
        {
            is_runnig = false; 
        }break;
        case WM_DESTROY:
        {
            is_runnig = false; 
        }break;
        default:
        {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }break;
    }
    return result;
}


// function to initialize OpengGL in windows.h
bool InitOpengGLContext(HWND* hWnd, HDC* handle_device_context)
{
     PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,     // flags
        PFD_TYPE_RGBA,     // color frame buffer pixel format
        32,                // color frame buffer depht format 
 		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,   
        24,                // number of bits for the depht buffer
        8,                 // number of bits for the stancil buffer
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    }; 

    *handle_device_context = GetDC(*hWnd);
    int wnd_pixel_format = ChoosePixelFormat(*handle_device_context, &pfd);
    SetPixelFormat(*handle_device_context, wnd_pixel_format, &pfd);

    HGLRC opengl_render_context_temp = wglCreateContext(*handle_device_context);
    wglMakeCurrent(*handle_device_context, opengl_render_context_temp);
    
    int attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    HGLRC opengl_render_context;
    if(wglCreateContextAttribsARB)
    {
        opengl_render_context = wglCreateContextAttribsARB(*handle_device_context, NULL, attributes);
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(opengl_render_context_temp);
        wglMakeCurrent(*handle_device_context, opengl_render_context);

        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 
            (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        if(wglSwapIntervalEXT)
        {
            wglSwapIntervalEXT(1);
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    if(!gladLoadGL())
    {
        OutputDebugString("ERROR::INITIALIZING::GLAD\n");
        return false;
    }
    
    // set up initial configuration for open GL...
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glViewport(0, 0, WNDWIDTH, WNDHEIGHT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST); 

    return true; 
}


// this is the entry point of our application...
int WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR     lpCmdLine,
        int       nShowCmd)
{
    // register wndClass to decribe the type of window
    // we want to have... 
    WNDCLASS wc      = { };
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance; 
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "BouncingBalls3D";
    if(RegisterClass(&wc) == 0)
    {
        OutputDebugString("FAILED::REGISTERING::CLASS\n");
        return 1;
    }
    
    // funtion that create the window whit the wndClass and a position and a dimention
    HWND hWnd = CreateWindow(
                    "BouncingBalls3D",
                    "BouncingBalls3D",
                    WS_OVERLAPPEDWINDOW,
                    0, 0,
                    WNDWIDTH,
                    WNDHEIGHT,
                    NULL,
                    NULL,
                    hInstance,
                    NULL);
    if(hWnd == 0)
    {
        OutputDebugString("FAILED::CREATING::WINDOW\n");
        return 1;
    }
    
    // TODO: Initialize OpenGL from windows.h library... 
    HDC handle_device_context;
    if(!InitOpengGLContext(&hWnd, &handle_device_context))
    {
        OutputDebugString("FAILED::INITIALIZING::OPENGL\n");
        return 1;
    }

    // TODO: Initialize the Game...
    is_runnig = true;
    ShowWindow(hWnd, SW_SHOW);
    CoInitialize(nullptr);

    MainGame game;
    GameInit(&game);
    
    while(is_runnig == true)
    {
        MSG message;
        if(PeekMessage(&message, hWnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message); 
        }
        else
        {
            // TODO: Update and Render the Game...
            glClearColor(0.2f, 0.0f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // render game...
            GameUnpdateAndRender(&game, 1.0f);

            SwapBuffers(handle_device_context);

        }
    }

    // TODO: Clear and Colse the Game...
    ReleaseDC(hWnd, handle_device_context); 

    return 0;
}

