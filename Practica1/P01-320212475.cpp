#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>

const int WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;
GLint uniformColor;

// Vertex Shader
static const char* vShader = "                      \n\
#version 330 core                                   \n\
layout (location = 0) in vec3 pos;                  \n\
void main()                                         \n\
{                                                   \n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);  \n\
}";

// Fragment Shader
static const char* fShader = "                      \n\
#version 330 core                                   \n\
out vec4 color;                                     \n\
uniform vec4 uColor;                                \n\
void main()                                         \n\
{                                                   \n\
    color = uColor;                                 \n\
}";

void CrearLetras()
{
    // Arreglo explícito de coordenadas para b, J y S
    // Cada bloque rectangular está descompuesto en 2 triángulos (6 vértices)
    GLfloat vertices[] = {
        // LETRA 'b' MINÚSCULA (Izquierda: X en [-0.80, -0.40])
        // 1. Barra vertical completo (alto)
        -0.80f,  0.50f, 0.0f,   
        -0.80f, -0.50f, 0.0f,  
        -0.68f,  0.50f, 0.0f,
        -0.68f,  0.50f, 0.0f,   
        -0.80f, -0.50f, 0.0f,   
        -0.68f, -0.50f, 0.0f,
        // 2. Barra superior de la panza (media altura)
        -0.68f,  0.05f, 0.0f,   
        -0.68f, -0.07f, 0.0f,  
        -0.40f,  0.05f, 0.0f,
        -0.40f,  0.05f, 0.0f,   
        -0.68f, -0.07f, 0.0f,   
        -0.40f, -0.07f, 0.0f,
        // 3. Lateral derecho de la panza
        -0.52f,  0.05f, 0.0f,   
        -0.52f, -0.50f, 0.0f,   
        -0.40f,  0.05f, 0.0f,
        -0.40f,  0.05f, 0.0f,   
        -0.52f, -0.50f, 0.0f,   
        -0.40f, -0.50f, 0.0f,
        // 4. Barra inferior de la panza (base)
        -0.68f, -0.38f, 0.0f,   
        -0.68f, -0.50f, 0.0f,   
        -0.40f, -0.38f, 0.0f,
        -0.40f, -0.38f, 0.0f,   
        -0.68f, -0.50f, 0.0f,  
        -0.40f, -0.50f, 0.0f,

        // LETRA 'J' MAYÚSCULA (Centro: X en [-0.25, 0.25])
        // 1. Barra superior
        -0.25f,  0.50f, 0.0f,   
        -0.25f,  0.38f, 0.0f,    
         0.25f,  0.50f, 0.0f,
         0.25f,  0.50f, 0.0f,   
        -0.25f,  0.38f, 0.0f,    
         0.25f,  0.38f, 0.0f,
         // 2. Barra vertical derecho
         0.0f,  0.38f, 0.0f,    
         0.0f, -0.38f, 0.0f,    
         0.13f, 0.38f, 0.0f,
         0.13f, 0.38f, 0.0f,    
         0.0f, -0.38f, 0.0f,    
         0.13f, -0.38f, 0.0f,
          // 3. Base horizontal curva
        -0.25f, -0.38f, 0.0f,   
        -0.25f, -0.50f, 0.0f,    
         0.13f, -0.38f, 0.0f,
         0.13f, -0.38f, 0.0f,   
        -0.25f, -0.50f, 0.0f,    
         0.13f, -0.50f, 0.0f,

           // LETRA 'S' MAYÚSCULA (Derecha: X en [0.40, 0.80])
           // 1. Barra horizontal superior
         0.40f,  0.50f, 0.0f,    
         0.40f,  0.38f, 0.0f,    
         0.80f,  0.50f, 0.0f,
         0.80f,  0.50f, 0.0f,    
         0.40f,  0.38f, 0.0f,    
         0.80f,  0.38f, 0.0f,
            // 2. Tramo vertical superior izquierdo
         0.40f,  0.38f, 0.0f,    
         0.40f,  0.06f, 0.0f,    
         0.52f,  0.38f, 0.0f,
         0.52f,  0.38f, 0.0f,    
         0.40f,  0.06f, 0.0f,   
         0.52f,  0.06f, 0.0f,
             // 3. Barra horizontal media
         0.40f,  0.06f, 0.0f,    
         0.40f, -0.06f, 0.0f,    
         0.80f,  0.06f, 0.0f,
         0.80f,  0.06f, 0.0f,    
         0.40f, -0.06f, 0.0f,    
         0.80f, -0.06f, 0.0f,
              // 4. Tramo vertical inferior derecho
         0.68f, -0.06f, 0.0f,   
         0.68f, -0.38f, 0.0f,    
         0.80f, -0.06f, 0.0f,
         0.80f, -0.06f, 0.0f,    
         0.68f, -0.38f, 0.0f,    
         0.80f, -0.38f, 0.0f,
               // 5. Barra horizontal inferior
         0.40f, -0.38f, 0.0f,    
         0.40f, -0.50f, 0.0f,   
         0.80f, -0.38f, 0.0f,
         0.80f, -0.38f, 0.0f,    
         0.40f, -0.50f, 0.0f,    
         0.80f, -0.50f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    GLint codeLength[1];
    codeLength[0] = (GLint)strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    glAttachShader(theProgram, theShader);
    glDeleteShader(theShader);
}

void CompileShaders()
{
    shader = glCreateProgram();
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    glLinkProgram(shader);
    uniformColor = glGetUniformLocation(shader, "uColor");
}

int main()
{
    if (!glfwInit()) return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Iniciales bJS", NULL, NULL);
    if (!mainWindow)
    {
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    CrearLetras();
    CompileShaders();

    srand((unsigned int)time(NULL));

    double tiempoUltimoCambio = 0.0;
    float r = 0.1f, g = 0.1f, b = 0.1f;

    while (!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();
        double tiempoActual = glfwGetTime();

        // Cambio de color de fondo aleatorio cada 2 segundos
        if (tiempoActual - tiempoUltimoCambio >= 2.0)
        {
            r = (float)rand() / (float)RAND_MAX;
            g = (float)rand() / (float)RAND_MAX;
            b = (float)rand() / (float)RAND_MAX;
            tiempoUltimoCambio = tiempoActual;
        }

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        // Color uniforme para todas las iniciales (Negro)
        glUniform4f(uniformColor, 0.0f, 0.0f, 0.0f, 1.0f);
        glBindVertexArray(VAO);
        // 13 rectángulos en total * 2 triángulos = 26 triángulos (78 vértices)
        glDrawArrays(GL_TRIANGLES, 0, 78);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}