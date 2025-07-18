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

float x_mod = 0;
float y_mod = 0;
float z_mod = 0;
bool persCam = true;

GLfloat verticesArray[]{
    /* x,  y,   z */
    /*Vertex 1*/ 0.0f,   0.0f, -5.f,
    /*Vertex 2*/ 0.0f,  -10.0f, -6.f
};

GLfloat newPoints[]{
    /* x,  y,   z */
    /*Vertex 1*/ 0.0f,  0.0f, -5.f,
    /*Vertex 2*/ 5.0f,  0.0f, -6.f
};


GLuint VAO, VBO;

void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods) {

    if (key == GLFW_KEY_D && action == GLFW_REPEAT) {
        x_mod += 1.0f;
    }
    if (key == GLFW_KEY_A && action == GLFW_REPEAT) {
        x_mod -= 1.0f;
    }

    if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
        y_mod += 1.0f;
    }
    if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
        y_mod -= 1.0f;
    }

    if (key == GLFW_KEY_Q && action == GLFW_REPEAT) {
        z_mod += 1.0f;
    }
    if (key == GLFW_KEY_E && action == GLFW_REPEAT) {
        z_mod -= 1.0f;
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            sizeof(verticesArray),
            &verticesArray
        );
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            sizeof(newPoints),
            &newPoints
        );
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        persCam = !persCam;
    }
}

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
    glfwSetKeyCallback(window, Key_Callback);

    glViewport(0, 0, windowWidth, windowHeight);

    mat4 OrthoProjection = ortho(
        -800.0f,
         800.0f,
        -800.0f,
         800.0f,
        -100.0f,
         100.0f
    );

    mat4 PersProjection = perspective(
        radians(60.0f),
        windowHeight / windowWidth,
        0.1f,
        100.f
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

    vec3 cameraPos = vec3(x_mod, 0, 10.f);
    mat4 cameraPositionMatrix = translate(mat4(1.0f), cameraPos * -1.0f);
    vec3 worldUp = vec3(0, 1, 0);
    vec3 center = vec3(0, 3, 0);
    
    vec3 forward = vec3(center - cameraPos);
    forward = normalize(forward);
    
    vec3 right = normalize(cross(forward, worldUp));

    vec3 up = normalize(cross(right, forward));

    mat4 cameraOrientation = mat4(1.0f);
    cameraOrientation[0][0] = right.x;
    cameraOrientation[1][0] = right.y;
    cameraOrientation[2][0] = right.z;

    cameraOrientation[0][1] = up.x;
    cameraOrientation[1][1] = up.y;
    cameraOrientation[2][1] = up.z;

    cameraOrientation[0][2] = -forward.x;
    cameraOrientation[1][2] = -forward.y;
    cameraOrientation[2][2] = -forward.z;

    mat4 viewMatrix = cameraOrientation * cameraPositionMatrix;


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        //x_mod += 0.0001f;
        //verticesArray[0] += x_mod;
        cout << "X: " << x_mod << endl;
        cout << "Y: " << y_mod << endl;
        cout << "Z: " << z_mod << endl;

        glUseProgram(shaderProg);

        glBindVertexArray(VAO);
        glLineWidth(10);
        glDrawArrays(GL_LINES, 0, 2);

        unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
        if(persCam) glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(PersProjection));
        else glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(OrthoProjection));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(viewMatrix));

        unsigned int xLoc = glGetUniformLocation(shaderProg, "x");
        glUniform1f(xLoc, x_mod);
        
        unsigned int yLoc = glGetUniformLocation(shaderProg, "y");
        glUniform1f(yLoc, y_mod);
        
        unsigned int zLoc = glGetUniformLocation(shaderProg, "z");
        glUniform1f(zLoc, z_mod);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProg);

    glfwTerminate();
    return 0;
}