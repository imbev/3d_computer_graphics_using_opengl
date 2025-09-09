#include <me_gl_window.h>

void me_gl_window_init_gl()
{
    glewInit();

    GLfloat verts[] = {
        +0.0f, +0.0f,        // 0
        +1.0f, +0.0f, +0.0f, //
        +1.0f, +1.0f,        // 1
        +1.0f, +0.0f, +0.0f, //
        -1.0f, +1.0f,        // 2
        +1.0f, +0.0f, +0.0f, //
        -1.0f, -1.0f,        // 3
        +1.0f, +0.0f, +0.0f, //
        +1.0f, -1.0f,        // 4
        +1.0f, +0.0f, +0.0f, //
    };

    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (char *)(sizeof(GL_FLOAT) * 2));

    GLushort indices[] = {
        0, 1, 2, //
        0, 3, 4  //
    };
    GLuint indexBufferId;
    glGenBuffers(1, &indexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void me_gl_window_paint_gl(GLFWwindow *window)
{
    glClear(GL_COLOR_BUFFER_BIT);

    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}
