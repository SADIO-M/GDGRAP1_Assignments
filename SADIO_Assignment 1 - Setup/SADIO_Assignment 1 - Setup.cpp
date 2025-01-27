/*
    Assignment 01 - Setup

    Created by: SADIO (GDGRAP1-S22)
    Latest revision: January 27, 2025, 8:15 PM

    Submitted on: January 27, 2025
*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit()) return -1;

    window = glfwCreateWindow(800, 800, "Megan Florence Sophia Sadio", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    //Array of vertices of the Octagon
    GLfloat verticesArray[]{
                    /* x,  y,   z */
        /*Vertex 1*/ -0.1f, 1.f, 0.f,
        /*Vertex 2*/ 0.1f, 1.f, 0.f,
        /*Vertex 3*/ 0.f, 1.f, 0.f,
        /*Vertex 4*/ 0.2f, 0.5f, 0.f,
        /*Vertex 5*/ 0.2f, 0.5f, 0.f,
        /*Vertex 6*/ 0.5f, 0.f, 0.f,
        /*Vertex 7*/ -0.6f, 0.1f, 0.f,
        /*Vertex 8*/ -0.6f, 0.1f, 0.f,
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

        glBindVertexArray(VAO);
        glDrawArrays(GL_POLYGON, 0, 8);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}