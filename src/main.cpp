//
//  Main File
//  main.cpp
//
//  Author: Kirill "ori0n" Kravinsky 2015
//

//#define  GLFW_INCLUDE_GLU


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Main window handle
GLFWwindow* wnd;

void display(GLuint &vao)
{
    // Clear buffer
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 12);

    // Swap buffers (using double buffering)
    glfwSwapBuffers(wnd);
}

void wnd_resized_callback(GLFWwindow* window, int width, int height)
{
    glClearColor(1, 0, 0, 1);

    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(wnd);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

// Read a shader source from a file
// store the shader source in a std::vector<char>
void read_shader_src(const char *fname, std::vector<char> &buffer) {
    std::ifstream in;
    in.open(fname, std::ios::binary);

    if(in.is_open()) {
        // Get the number of bytes stored in this file
        in.seekg(0, std::ios::end);
        size_t length = (size_t)in.tellg();

        // Go to start of the file
        in.seekg(0, std::ios::beg);

        // Read the content of the file in a buffer
        buffer.resize(length + 1);
        in.read(&buffer[0], length);
        in.close();
        // Add a valid C - string end
        buffer[length] = '\0';
    }
    else {
        std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
        exit(-1);
    }
}

// Compile shader
GLuint load_and_compile_shader(const char* fname, GLenum shaderType) {
    std::vector<char> buffer;
    read_shader_src(fname, buffer);
    const char* src = &buffer[0];

    // Compile shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    // Check the result of compilation
    GLint test;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
    if (!test) {
        std::cerr << "Shader compilation failed: " << std::endl;
        std::vector<char> compilation_log(512);
        glGetShaderInfoLog(shader, (GLsizei) compilation_log.size(), NULL, &compilation_log[0]);
        std::cerr << &(compilation_log[0]) << std::endl;
        exit(-1);
    }

    return shader;
}

// Create a program from two shaders
GLuint create_program(const char* path_vert_shader, const char* path_frag_shader) {
    GLuint vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
    GLuint fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    return shaderProgram;
}

void initialize(GLuint &vao) {
    // Use a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 4 triangles to be rendered
    GLfloat vertices_pos[24] = {
            0.0f, 0.0f,
            0.5f, 0.0f,
            0.5f, 0.5f,

            0.0f, 0.0f,
            0.0f, 0.5f,
            -0.5f, 0.5f,

            0.0f, 0.0f,
            -0.5f, 0.0f,
            -0.5f, -0.5f,

            0.0f, 0.0f,
            0.0f, -0.5f,
            0.5f, -0.5f,
    };

    // Create & allocate vertex buffer (store for vertices in video mem)
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_pos), vertices_pos, GL_STATIC_DRAW);

    GLuint shaderProgram = create_program("renderprogs/vertex.vert", "renderprogs/fragment.frag");

    // Get the location of the attributes that enter the vertex shader
    GLint position_attribute = glGetAttribLocation(shaderProgram, "position");

    // Specify how the position data will be accessed
    glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Enable attribute
    glEnableVertexAttribArray(position_attribute);
}

int main(int argc, char** argv) {

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Main window handle
    wnd = glfwCreateWindow(640, 480, "GLFW Bootstrap", NULL, NULL);

    if (!wnd)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(wnd);
    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(wnd, wnd_resized_callback);
    glfwSetKeyCallback(wnd, key_callback);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Print OpenGL version
    int major = glfwGetWindowAttrib(wnd, GLFW_CONTEXT_VERSION_MAJOR);
    int minor = glfwGetWindowAttrib(wnd, GLFW_CONTEXT_VERSION_MINOR);
    int rev =   glfwGetWindowAttrib(wnd, GLFW_CONTEXT_REVISION);
    std::cout << "OpenGL - " << major << "." << minor << "." << rev << std::endl;

    // The vertex array object
    GLuint vao;

    // Initialize a vertex array orbject
    initialize(vao);

    while (!glfwWindowShouldClose(wnd))
    {
        display(vao);
        glfwPollEvents();
    }

    glfwDestroyWindow(wnd);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
