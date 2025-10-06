#include <me_gl_window.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void send_data_to_opengl()
{
    const float RED_TRIANGLE_Z = +0.5f;
    const float BLUE_TRIANGLE_Z = -0.5f;

    GLfloat verts[] = {
        -1.0f, -1.0f, RED_TRIANGLE_Z, //
        +1.0f, +0.0f, +0.0f, //
        +0.0f, +1.0f, -1.0f, //
        +0.0f, +0.0f, +1.0f, //
        +1.0f, -1.0f, RED_TRIANGLE_Z, //
        +1.0f, +0.0f, +0.0f, //

        +0.0f, +1.0f,  BLUE_TRIANGLE_Z, //
        +0.0f, +0.0f, +1.0f, //
        +0.0f, -1.0f,  BLUE_TRIANGLE_Z, //
        +0.0f, +0.0f, +1.0f, //
        +1.0f, +1.0f, BLUE_TRIANGLE_Z, //
        +0.0f, +0.0f, +1.0f, //
    };

    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (char *)(sizeof(GL_FLOAT) * 3));

    GLushort indices[] = {
        0, 1, 2, //
        3, 4, 5, //
    };
    GLuint indexBufferId;
    glGenBuffers(1, &indexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

bool check_status(
    GLuint object_id, 
    PFNGLGETSHADERIVPROC object_property_getter,
    PFNGLGETSHADERINFOLOGPROC get_info_log_func,
    GLenum status_type
) {
    GLint status;
    object_property_getter(object_id, status_type, &status);
    if (status != GL_TRUE) {
        GLint info_log_length;
        object_property_getter(object_id, GL_INFO_LOG_LENGTH, &info_log_length);
        GLchar *buffer = malloc(sizeof(GLchar) * info_log_length);

        GLsizei buffer_size;
        get_info_log_func(object_id, info_log_length, &buffer_size, buffer);
        printf("%s\n", buffer);

        free(buffer);

        return false;
    }

    return true;
}

bool check_shader_status(GLuint shader_id) {
    return check_status(shader_id, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool check_program_status(GLuint program_id) {
    if (!check_status(program_id, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS)) {
        return false;
    }
    GLint attached_count;
    glGetProgramiv(program_id, GL_ATTACHED_SHADERS, &attached_count);
    if (attached_count == 0) {
        printf("No shader objects attached.\n");
        return false;
    }
    return true;
}

char *read_shader_code(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", file_name);
        exit(1);
    }

    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char *buffer = malloc(file_size);
    if (buffer == NULL) {
        printf("Failed to load file: %s\n", file_name);
        exit(1);
    }
    fread(buffer, sizeof(char), file_size, file);

    fclose(file);

    return buffer;
}

void install_shaders()
{
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar *adapter[1];

    adapter[0] = read_shader_code("./vertex_shader_code.glsl");
    glShaderSource(vertex_shader_id, 1, adapter, 0);
    free((void *)adapter[0]);

    adapter[0] = read_shader_code("./fragment_shader_code.glsl");
    glShaderSource(fragment_shader_id, 1, adapter, 0);
    free((void *)adapter[0]);

    glCompileShader(vertex_shader_id);
    glCompileShader(fragment_shader_id);

    if (!check_shader_status(vertex_shader_id) || !check_shader_status(fragment_shader_id)) {
        return;
    }

    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    if (!check_program_status(program_id)) {
        return;
    }

    glUseProgram(program_id);
}

void me_gl_window_init_gl()
{
    glewInit();
    glEnable(GL_DEPTH_TEST);
    send_data_to_opengl();
    install_shaders();
}

void me_gl_window_paint_gl(GLFWwindow *window)
{
    glClear(GL_DEPTH_BUFFER_BIT);

    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}
