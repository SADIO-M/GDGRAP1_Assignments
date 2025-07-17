#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>

using namespace std;
using namespace glm;

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit()) return -1;

    window = glfwCreateWindow(800, 800, "Megan Florence Sophia Sadio", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    float windowWidth = 800;
    float windowHeight = 800;

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, 800, 800);

    mat4 projection = ortho(
        -800.0f,
         800.0f,
        -800.0f,
         800.0f,
        -100.0f,
         100.0f
    );

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProg = glCreateProgram();

    // Setting up the vertex shader
    fstream vertexSrc("Shaders/Line.vert");
    stringstream vertexBuffer;
    vertexBuffer << vertexSrc.rdbuf();
    string vertexString = vertexBuffer.str();
    const char* vert = vertexString.c_str();

    // Setting up the fragment shader
    fstream fragmentSrc("Shaders/Line.frag");
    stringstream fragmentBuffer;
    fragmentBuffer << fragmentSrc.rdbuf();
    string fragmentString = fragmentBuffer.str();
    const char* frag = fragmentString.c_str();

    // Creation of the vertex shaders
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vert, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &frag, NULL);
    glCompileShader(fragmentShader);

    // Attaching shaders and program linking
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragmentShader);
    glLinkProgram(shaderProg);

    //Clean-up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Array of vertices of the Octagon
    GLfloat verticesArray[]{
                    /* x,  y,   z */
        /*Vertex 1*/ 0.0f, 100.0f, 0.f,
        /*Vertex 2*/ 0.0f, -750.0f, 0.f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(verticesArray),
        verticesArray,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(GLfloat),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProg);

        glBindVertexArray(VAO);
        glLineWidth(1);
        glDrawArrays(GL_LINES, 0, 2);

        unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProg);

    glfwTerminate();
    return 0;
}