#ifndef __VAPP_H__
#define __VAPP_H__

#ifndef WIN32

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#define GlutMainLoopEvent glutCheckLoop

#else

#include "vgl.h"
#define GlutMainLoopEvent glutMainLoopEvent

#endif

class VermillionApplication
{
protected:
    inline VermillionApplication(void) {}
    virtual ~VermillionApplication(void) {}

    static VermillionApplication * s_app;

    static void DisplayFunc(void);
    static void ReshapeFunc(int width, int height);

#ifdef _DEBUG
    static void APIENTRY DebugOutputCallback(GLenum source,
                                             GLenum type,
                                             GLuint id,
                                             GLenum severity,
                                             GLsizei length,
                                             const GLchar* message,
                                             GLvoid* userParam);
#endif

public:
    virtual void MainLoop(void);

    virtual void Initialize(const char * title = 0)
    {
        int one = 1;
        char * name = "name";

#ifdef _DEBUG
        glutInitContextFlags(GLUT_DEBUG);
#endif
        // glutInitContextProfile(GLUT_CORE_PROFILE);
        // glutInitContextVersion(4, 3);

        glutInitWindowSize(480, 480);
        glutInitWindowPosition (140, 140);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
        glutInit(&one, &name);

        glutCreateWindow( title ? title : "OpenGL Application" );
        glutDisplayFunc(DisplayFunc);
        glutReshapeFunc(ReshapeFunc);
#ifdef WIN32
#ifdef USE_GL3W
        gl3wInit();
#else
        glewInit();
#endif
#endif

#ifdef _DEBUG
        if (glDebugMessageCallbackARB != NULL)
            glDebugMessageCallbackARB(DebugOutputCallback, this);
#endif
    }

    virtual void Display(bool auto_redraw = true)
    {
        glutSwapBuffers();
        if (auto_redraw)
        {
            glutPostRedisplay();
        }
    }

    virtual void Finalize(void)
    {

    }

    virtual void Reshape(int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};

#define BEGIN_APP_DECLARATION(appclass,title)               \
class appclass : public VermillionApplication               \
{                                                           \
public:                                                     \
    typedef class VermillionApplication base;               \
    static VermillionApplication * Create(void)             \
    {                                                       \
        return (s_app = new appclass);                      \
    }                                                       \
	static int run(){                                       \
		VermillionApplication * app = appclass::Create();   \
		app->Initialize(title);                             \
		app->MainLoop();                                    \
		app->Finalize();                                    \
		return 0;											\
	}														\

#define END_APP_DECLARATION()                               \
};

#ifdef _DEBUG
#define DEBUG_OUTPUT_CALLBACK                                                   \
void APIENTRY VermillionApplication::DebugOutputCallback(GLenum source,         \
                                                         GLenum type,           \
                                                         GLuint id,             \
                                                         GLenum severity,       \
                                                         GLsizei length,        \
                                                         const GLchar* message, \
                                                         GLvoid* userParam)     \
{                                                                               \
    OutputDebugStringA(message);                                                \
    OutputDebugStringA("\n");                                                   \
}
#else
#define DEBUG_OUTPUT_CALLBACK
#endif

#endif /* __VAPP_H__ */
