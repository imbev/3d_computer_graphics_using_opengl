#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "me_gl_window.h"

int main(int argc, char *argv[])
{
    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    me_gl_window_init_gl();

    while (!glfwWindowShouldClose(window))
    {
        me_gl_window_paint_gl(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
