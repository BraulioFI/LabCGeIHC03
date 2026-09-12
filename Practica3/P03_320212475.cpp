/*
// Practica 3: Cohete espacial con instancias de primitivas geometricas
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>

// GLM
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\random.hpp>

// Clases del proyecto
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"

using std::vector;

const float toRadians = 3.14159265f / 180.0f;
const float PI = 3.14159265f;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Camera camera;
Window mainWindow;

vector<Mesh*> meshList;
vector<Shader> shaderList;

// Shaders
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";

// Esfera
Sphere sp = Sphere(1.0f, 20, 20);


// ============================================================
// CUBO
void CrearCubo()
{
    unsigned int cubo_indices[] = {
        // front
        0, 1, 2,
        2, 3, 0,

        // right
        1, 5, 6,
        6, 2, 1,

        // back
        7, 6, 5,
        5, 4, 7,

        // left
        4, 0, 3,
        3, 7, 4,

        // bottom
        4, 5, 1,
        1, 0, 4,

        // top
        3, 2, 6,
        6, 7, 3
    };

    GLfloat cubo_vertices[] = {
        // front
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // back
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };

    Mesh* cubo = new Mesh();
    cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
    meshList.push_back(cubo);
}


// ============================================================
// PIRAMIDE TRIANGULAR
void CrearPiramideTriangular()
{
    GLfloat vertices_piramide_triangular[] = {
        -1.0f,  1.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.45f
    };

    unsigned int indices_piramide_triangular[] = {
        0, 1, 2,
        0, 3, 1,
        1, 3, 2,
        2, 3, 0
    };

    Mesh* piramidet = new Mesh();
    piramidet->CreateMesh(
        vertices_piramide_triangular,
        indices_piramide_triangular,
        12,
        12
    );

    meshList.push_back(piramidet);
}


// ============================================================
// PIRAMIDE CUADRANGULAR
void CrearPiramideCuadrangular()
{
    unsigned int piramidecuadrangular_indices[] = {
        0, 3, 4, // frontal
        3, 2, 4, // izquierda
        2, 1, 4, // trasera
        1, 0, 4, // derecha
        0, 1, 2, // abajo 1
        0, 2, 3  // abajo 2
    };

    GLfloat piramidecuadrangular_vertices[] = {
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.0f,  0.5f,  0.0f
    };

    Mesh* piramidec = new Mesh();
    piramidec->CreateMesh(
        piramidecuadrangular_vertices,
        piramidecuadrangular_indices,
        15,
        18
    );

    meshList.push_back(piramidec);
}


// ============================================================
// CILINDRO
// Se construye con triangulos para que sea compatible con
// Mesh::RenderMesh(), igual que el cubo y las piramides.
void CrearCilindro(int res, float R)
{
    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    // Centro inferior
    vertices.push_back(0.0f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.0f);

    // Centro superior
    vertices.push_back(0.0f);
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);

    // Anillos inferior y superior.
    // Se repite el primer punto al final para cerrar el cilindro.
    for (int i = 0; i <= res; i++)
    {
        float angulo = 2.0f * PI * i / res;
        float x = R * cos(angulo);
        float z = R * sin(angulo);

        // Vertice inferior
        vertices.push_back(x);
        vertices.push_back(-0.5f);
        vertices.push_back(z);

        // Vertice superior
        vertices.push_back(x);
        vertices.push_back(0.5f);
        vertices.push_back(z);
    }

    // Indices
    for (int i = 0; i < res; i++)
    {
        unsigned int inferiorActual = 2 + i * 2;
        unsigned int superiorActual = inferiorActual + 1;

        unsigned int inferiorSiguiente = inferiorActual + 2;
        unsigned int superiorSiguiente = superiorActual + 2;

        // Pared lateral: dos triangulos
        indices.push_back(inferiorActual);
        indices.push_back(superiorActual);
        indices.push_back(inferiorSiguiente);

        indices.push_back(superiorActual);
        indices.push_back(superiorSiguiente);
        indices.push_back(inferiorSiguiente);

        // Tapa inferior
        indices.push_back(0);
        indices.push_back(inferiorSiguiente);
        indices.push_back(inferiorActual);

        // Tapa superior
        indices.push_back(1);
        indices.push_back(superiorActual);
        indices.push_back(superiorSiguiente);
    }

    Mesh* cilindro = new Mesh();

    cilindro->CreateMesh(
        vertices.data(),
        indices.data(),
        (unsigned int)vertices.size(),
        (unsigned int)indices.size()
    );

    meshList.push_back(cilindro);
}


// ============================================================
// CONO
// Tambien se construye con GL_TRIANGLES.
void CrearCono(int res, float R)
{
    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    // Punta del cono
    vertices.push_back(0.0f);
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);

    // Centro de la base
    vertices.push_back(0.0f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.0f);

    // Circunferencia de la base
    for (int i = 0; i <= res; i++)
    {
        float angulo = 2.0f * PI * i / res;
        float x = R * cos(angulo);
        float z = R * sin(angulo);

        vertices.push_back(x);
        vertices.push_back(-0.5f);
        vertices.push_back(z);
    }

    for (int i = 0; i < res; i++)
    {
        unsigned int actual = 2 + i;
        unsigned int siguiente = actual + 1;

        // Cara lateral
        indices.push_back(0);
        indices.push_back(actual);
        indices.push_back(siguiente);

        // Base
        indices.push_back(1);
        indices.push_back(siguiente);
        indices.push_back(actual);
    }

    Mesh* cono = new Mesh();

    cono->CreateMesh(
        vertices.data(),
        indices.data(),
        (unsigned int)vertices.size(),
        (unsigned int)indices.size()
    );

    meshList.push_back(cono);
}


// ============================================================
// SHADERS
void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);

    Shader* shader2 = new Shader();
    shader2->CreateFromFiles(vShaderColor, fShader);
    shaderList.push_back(*shader2);
}


// ============================================================
// DIBUJAR UNA INSTANCIA DE CUBO, PIRAMIDE, CILINDRO O CONO
void DibujarInstancia(
    int indice,
    glm::vec3 posicion,
    glm::vec3 escala,
    glm::vec3 rotacion,
    glm::vec3 color,
    GLuint uniformModel,
    GLuint uniformColor,
    glm::mat4 matrizPadre = glm::mat4(1.0f))
{
    glm::mat4 model = matrizPadre;

    // Traslacion
    model = glm::translate(model, posicion);

    // Rotacion X
    model = glm::rotate(
        model,
        glm::radians(rotacion.x),
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    // Rotacion Y
    model = glm::rotate(
        model,
        glm::radians(rotacion.y),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Rotacion Z
    model = glm::rotate(
        model,
        glm::radians(rotacion.z),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    // Escala
    model = glm::scale(model, escala);

    glUniformMatrix4fv(
        uniformModel,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

    glUniform3fv(
        uniformColor,
        1,
        glm::value_ptr(color)
    );

    meshList[indice]->RenderMesh();
}


// ============================================================
// DIBUJAR ESFERA
void DibujarEsfera(
    glm::vec3 posicion,
    glm::vec3 escala,
    glm::vec3 color,
    GLuint uniformModel,
    GLuint uniformColor)
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, posicion);
    model = glm::scale(model, escala);

    glUniformMatrix4fv(
        uniformModel,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

    glUniform3fv(
        uniformColor,
        1,
        glm::value_ptr(color)
    );

    sp.render();
}


// ============================================================
// MAIN
int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();

    // IMPORTANTE:
    // Los indices en meshList quedan de esta manera:
    // 0 = cubo
    // 1 = piramide triangular
    // 2 = cilindro
    // 3 = cono
    // 4 = piramide cuadrangular

    CrearCubo();
    CrearPiramideTriangular();
    CrearCilindro(30, 1.0f);
    CrearCono(30, 1.0f);
    CrearPiramideCuadrangular();

    CreateShaders();


    // ========================================================
    // CAMARA (Vista frontal y mayor velocidad)
    camera = Camera(
        glm::vec3(0.0f, 1.8f, 8.0f),   // Posición centrada en X, altura media en Y y al frente en +Z
        glm::vec3(0.0f, 1.0f, 0.0f),   // Vector Up
        -90.0f,                        // Yaw: -90° para mirar en línea recta hacia -Z (de frente)
        0.0f,                          // Pitch: 0° nivelado al horizonte
        8.0f,                          // MoveSpeed: Aumentado de 3.0f a 8.0f para desplazarse más rápido
        0.2f                           // TurnSpeed: Sensibilidad del ratón
    );


    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
        0.1f,
        100.0f
    );


    // Inicializar esfera
    sp.init();
    sp.load();


    // ========================================================
    // CICLO PRINCIPAL
    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();

        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        // Controles de camara
        camera.keyControl(
            mainWindow.getsKeys(),
            deltaTime
        );

        camera.mouseControl(
            mainWindow.getXChange(),
            mainWindow.getYChange()
        );


        // ====================================================
        // LIMPIAR PANTALLA
        glClearColor(
            0.05f,
            0.05f,
            0.12f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );

        glEnable(GL_DEPTH_TEST);


        // ====================================================
        // USAR SHADER
        shaderList[0].useShader();

        uniformModel =
            shaderList[0].getModelLocation();

        uniformProjection =
            shaderList[0].getProjectLocation();

        uniformView =
            shaderList[0].getViewLocation();

        uniformColor =
            shaderList[0].getColorLocation();


        glUniformMatrix4fv(
            uniformProjection,
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        glUniformMatrix4fv(
            uniformView,
            1,
            GL_FALSE,
            glm::value_ptr(
                camera.calculateViewMatrix()
            )
        );


        // ====================================================
        // PISO
        DibujarInstancia(
            0,
            glm::vec3(0.0f, -0.55f, 0.0f),
            glm::vec3(10.0f, 0.1f, 10.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.12f, 0.12f, 0.12f),
            uniformModel,
            uniformColor
        );


        // ====================================================
        // CUERPO CENTRAL DEL COHETE
        // CILINDRO
        DibujarInstancia(
            2,
            glm::vec3(0.0f, 1.40f, 0.0f),
            glm::vec3(0.75f, 2.80f, 0.75f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.80f, 0.80f, 0.85f),
            uniformModel,
            uniformColor
        );


        // ====================================================
        // PUNTA DEL COHETE
        // CONO
        DibujarInstancia(
            3,
            glm::vec3(0.0f, 3.50f, 0.0f),
            glm::vec3(0.78f, 1.40f, 0.78f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.85f, 0.12f, 0.12f),
            uniformModel,
            uniformColor
        );


        // ====================================================
        // VENTANA FRONTAL
        // ESFERA
        DibujarEsfera(
            glm::vec3(0.0f, 2.10f, 0.70f),
            glm::vec3(0.32f, 0.32f, 0.18f),
            glm::vec3(0.10f, 0.55f, 1.0f),
            uniformModel,
            uniformColor
        );


        // ====================================================
        // SEGUNDA VENTANA TRASERA
        // ESFERA
        DibujarEsfera(
            glm::vec3(0.0f, 2.10f, -0.70f),
            glm::vec3(0.32f, 0.32f, 0.18f),
            glm::vec3(0.10f, 0.55f, 1.0f),
            uniformModel,
            uniformColor
        );


        // ====================================================
        // CINTURON DEL COHETE
        // CUBO
        DibujarInstancia(
            0,
            glm::vec3(0.0f, 0.55f, 0.0f),
            glm::vec3(1.65f, 0.22f, 1.65f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.25f, 0.25f, 0.30f),
            uniformModel,
            uniformColor
        );


        // ====================================================
        // ALETA TRASERA
        // PIRAMIDE CUADRANGULAR
        DibujarInstancia(
            4,
            glm::vec3(0.95f, 0.45f, 0.0f), //Traslacion
            glm::vec3(0.40f, 1.20f, 0.80f), //Escala
            glm::vec3(0.0f, 0.0f, 90.0f), //Rotacion
            glm::vec3(0.85f, 0.10f, 0.10f), //Color
            uniformModel,
            uniformColor
        );


        // ====================================================
        // ALETA FRONTAL
        // PIRAMIDE CUADRANGULAR
        DibujarInstancia(
            4,
            glm::vec3(-0.95f, 0.45f, 0.0f),
            glm::vec3(0.40f, 1.20f, 0.80f),
            glm::vec3(0.0f, 0.0f, -90.0f),
            glm::vec3(0.85f, 0.10f, 0.10f),
            uniformModel,
            uniformColor
        );


        // ====================================================
        // PROPULSOR IZQUIERDO
        // CILINDRO
        DibujarInstancia(
            2,
            glm::vec3(-0.72f, 0.15f, 0.0f),
            glm::vec3(0.25f, 0.95f, 0.25f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.35f, 0.35f, 0.40f),
            uniformModel,
            uniformColor
        );


        // ====================================================
        // PROPULSOR DERECHO
        // CILINDRO
        DibujarInstancia(
            2,
            glm::vec3(0.72f, 0.15f, 0.0f),
            glm::vec3(0.25f, 0.95f, 0.25f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.35f, 0.35f, 0.40f),
            uniformModel,
            uniformColor
        );



        // ====================================================
        // FINAL DEL FRAME
        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}
*/


// Practica 3 - Figura con 8 piramides cuadrangulares
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>

// GLM
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

// Clases del proyecto
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"

using std::vector;

// ============================================================
// VARIABLES GLOBALES
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Camera camera;
Window mainWindow;

vector<Mesh*> meshList;
vector<Shader> shaderList;

// Shaders
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";


// ============================================================
// CUBO
// meshList[0]
void CrearCubo()
{
    unsigned int cubo_indices[] = {
        // front
        0, 1, 2,
        2, 3, 0,

        // right
        1, 5, 6,
        6, 2, 1,

        // back
        7, 6, 5,
        5, 4, 7,

        // left
        4, 0, 3,
        3, 7, 4,

        // bottom
        4, 5, 1,
        1, 0, 4,

        // top
        3, 2, 6,
        6, 7, 3
    };

    GLfloat cubo_vertices[] = {
        // front
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // back
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };

    Mesh* cubo = new Mesh();
    cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
    meshList.push_back(cubo);
}


// ============================================================
// CARAS DE UNA PIRAMIDE CUADRANGULAR
//
// Se crea una piramide apuntando hacia +Z.
// La base queda en Z = 0.
// La punta queda hacia enfrente.
//
// Cada cara se guarda como una malla distinta.
// meshList[1] = cara inferior
// meshList[2] = cara derecha
// meshList[3] = cara superior
// meshList[4] = cara izquierda
void CrearCarasPiramide()
{
    unsigned int indices[] = { 0, 1, 2 };

    // Cara inferior
    GLfloat caraInferior[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.0f, 1.0f
    };

    Mesh* inferior = new Mesh();
    inferior->CreateMesh(caraInferior, indices, 9, 3);
    meshList.push_back(inferior);

    // Cara derecha
    GLfloat caraDerecha[] = {
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.0f,  0.0f, 1.0f
    };

    Mesh* derecha = new Mesh();
    derecha->CreateMesh(caraDerecha, indices, 9, 3);
    meshList.push_back(derecha);

    // Cara superior
    GLfloat caraSuperior[] = {
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.0f,  0.0f, 1.0f
    };

    Mesh* superior = new Mesh();
    superior->CreateMesh(caraSuperior, indices, 9, 3);
    meshList.push_back(superior);

    // Cara izquierda
    GLfloat caraIzquierda[] = {
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.0f,  0.0f, 1.0f
    };

    Mesh* izquierda = new Mesh();
    izquierda->CreateMesh(caraIzquierda, indices, 9, 3);
    meshList.push_back(izquierda);
}


// ============================================================
// SHADERS
void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}


// ============================================================
// DIBUJAR CUBO
void DibujarCubo(
    glm::vec3 posicion,
    glm::vec3 escala,
    glm::vec3 rotacion,
    glm::vec3 color,
    GLuint uniformModel,
    GLuint uniformColor,
    glm::mat4 matrizPadre = glm::mat4(1.0f))
{
    glm::mat4 model = matrizPadre;

    model = glm::translate(model, posicion);

    model = glm::rotate(
        model,
        glm::radians(rotacion.x),
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    model = glm::rotate(
        model,
        glm::radians(rotacion.y),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    model = glm::rotate(
        model,
        glm::radians(rotacion.z),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    model = glm::scale(model, escala);

    glUniformMatrix4fv(
        uniformModel,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

    glUniform3fv(
        uniformColor,
        1,
        glm::value_ptr(color)
    );

    meshList[0]->RenderMesh();
}


// ============================================================
// DIBUJAR PIRAMIDE COMPLETA CON COLORES EN SUS 4 CARAS
//
// Orden de colores:
// cara 1 = inferior
// cara 2 = derecha
// cara 3 = superior
// cara 4 = izquierda
// ============================================================
void DibujarPiramideColores(
    glm::mat4 model,
    glm::vec3 colorInferior,
    glm::vec3 colorDerecha,
    glm::vec3 colorSuperior,
    glm::vec3 colorIzquierda,
    GLuint uniformModel,
    GLuint uniformColor)
{
    glm::vec3 colores[4] = {
        colorInferior,
        colorDerecha,
        colorSuperior,
        colorIzquierda
    };

    glUniformMatrix4fv(
        uniformModel,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

    for (int i = 0; i < 4; i++)
    {
        glUniform3fv(
            uniformColor,
            1,
            glm::value_ptr(colores[i])
        );

        meshList[i + 1]->RenderMesh();
    }
}

// ============================================================
// DIBUJAR PIRAMIDE CON LAS 4 CARAS DE DIFERENTE COLOR
//
// Inferior = rojo
// Derecha  = verde
// Superior = magenta
// Izquierda = amarillo
// ============================================================
void DibujarPiramideMulticolor(
    glm::mat4 model,
    GLuint uniformModel,
    GLuint uniformColor)
{
    DibujarPiramideColores(
        model,

        // Cara inferior - ROJO
        glm::vec3(1.0f, 0.0f, 0.0f),

        // Cara derecha - VERDE
        glm::vec3(0.0f, 1.0f, 0.0f),

        // Cara superior - MAGENTA
        glm::vec3(1.0f, 0.0f, 1.0f),

        // Cara izquierda - AMARILLO
        glm::vec3(1.0f, 1.0f, 0.0f),

        uniformModel,
        uniformColor
    );
}

// ============================================================
// DIBUJAR PIRAMIDE SOLIDA
void DibujarPiramideSolida(
    glm::mat4 model,
    glm::vec3 color,
    GLuint uniformModel,
    GLuint uniformColor)
{
    glUniformMatrix4fv(
        uniformModel,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

    glUniform3fv(
        uniformColor,
        1,
        glm::value_ptr(color)
    );

    for (int i = 0; i < 4; i++)
    {
        meshList[i + 1]->RenderMesh();
    }
}


// ============================================================
// MAIN
int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();

    // Geometria
    CrearCubo();          // meshList[0]
    CrearCarasPiramide(); // meshList[1..4]
    CreateShaders();

    // ========================================================
    // CAMARA 
    camera = Camera(
        glm::vec3(0.0f, 1.8f, 7.5f),   // Posición centrada en X, altura media en Y y al frente en +Z
        glm::vec3(0.0f, 1.0f, 0.0f),   // Vector Up
        -90.0f,                        // Yaw: -90° para mirar en línea recta hacia -Z (de frente)
        0.0f,                          // Pitch: 0° nivelado al horizonte
        8.0f,                          // MoveSpeed: Aumentado de 3.0f a 8.0f para desplazarse más rápido
        0.2f                           // TurnSpeed: Sensibilidad del ratón
    );

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
        0.1f,
        100.0f
    );

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        shaderList[0].useShader();

        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();
        uniformView = shaderList[0].getViewLocation();
        uniformColor = shaderList[0].getColorLocation();

        glUniformMatrix4fv(
            uniformProjection,
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        glUniformMatrix4fv(
            uniformView,
            1,
            GL_FALSE,
            glm::value_ptr(camera.calculateViewMatrix())
        );

        // ====================================================
        // COLORES

        glm::vec3 rojo = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 verde = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 amarillo = glm::vec3(1.0f, 1.0f, 0.0f);
        glm::vec3 magenta = glm::vec3(1.0f, 0.0f, 1.0f);


        // ====================================================
        // PISO

        DibujarCubo(
            glm::vec3(0.0f, -2.15f, 0.0f),
            glm::vec3(10.0f, 0.1f, 10.0f),
            glm::vec3(0.0f),
            glm::vec3(0.12f, 0.12f, 0.12f),
            uniformModel,
            uniformColor
        );


        // ====================================================
    // PIRAMIDES EXTERIORES
    // ARRIBA
        glm::mat4 pArriba = glm::mat4(1.0f);

        pArriba = glm::translate(
            pArriba,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        pArriba = glm::rotate(
            pArriba,
            glm::radians(-90.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        pArriba = glm::scale(
            pArriba,
            glm::vec3(2.10f, 2.1f, 1.05f)
        );

        DibujarPiramideMulticolor(
            pArriba,
            uniformModel,
            uniformColor
        );


        // ABAJO
        glm::mat4 pAbajo = glm::mat4(1.0f);

        pAbajo = glm::translate(
            pAbajo,
            glm::vec3(0.0f, -1.0f, 0.0f)
        );

        pAbajo = glm::rotate(
            pAbajo,
            glm::radians(90.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        pAbajo = glm::scale(
            pAbajo,
            glm::vec3(2.10f, 2.1f, 1.05f)
        );

        DibujarPiramideMulticolor(
            pAbajo,
            uniformModel,
            uniformColor
        );


        // IZQUIERDA
        glm::mat4 pIzquierda = glm::mat4(1.0f);

        pIzquierda = glm::translate(
            pIzquierda,
            glm::vec3(-1.05f, 0.0f, 0.0f)
        );

        pIzquierda = glm::rotate(
            pIzquierda,
            glm::radians(-90.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        pIzquierda = glm::scale(
            pIzquierda,
            glm::vec3(2.10f, 2.0f, 1.05f)
        );

        DibujarPiramideMulticolor(
            pIzquierda,
            uniformModel,
            uniformColor
        );


        // DERECHA
        glm::mat4 pDerecha = glm::mat4(1.0f);

        pDerecha = glm::translate(
            pDerecha,
            glm::vec3(1.05f, 0.0f, 0.0f)
        );

        pDerecha = glm::rotate(
            pDerecha,
            glm::radians(90.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        pDerecha = glm::scale(
            pDerecha,
            glm::vec3(2.10f, 2.0f, 1.05f)
        );

        DibujarPiramideMulticolor(
            pDerecha,
            uniformModel,
            uniformColor
        );


        // ====================================================
        // PIRAMIDES INTERIORES INVERTIDAS
        // ROJA INVERTIDA
        glm::mat4 pCentroRoja = glm::mat4(1.0f);

        pCentroRoja = glm::translate(
            pCentroRoja,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        pCentroRoja = glm::rotate(
            pCentroRoja,
            glm::radians(90.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        pCentroRoja = glm::scale(
            pCentroRoja,
            glm::vec3(2.10f, 2.1f, 1.05f)
        );

        DibujarPiramideMulticolor(
            pCentroRoja,
            uniformModel,
            uniformColor
        );


        // AMARILLA INVERTIDA
        glm::mat4 pCentroAmarilla = glm::mat4(1.0f);

        pCentroAmarilla = glm::translate(
            pCentroAmarilla,
            glm::vec3(0.0f, -1.0f, 0.0f)
        );

        pCentroAmarilla = glm::rotate(
            pCentroAmarilla,
            glm::radians(-90.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        pCentroAmarilla = glm::scale(
            pCentroAmarilla,
            glm::vec3(2.10f, 2.1f, 1.05f)
        );

        DibujarPiramideMulticolor(
            pCentroAmarilla,
            uniformModel,
            uniformColor
        );


        // VERDE INVERTIDA
        glm::mat4 pCentroVerde = glm::mat4(1.0f);

        pCentroVerde = glm::translate(
            pCentroVerde,
            glm::vec3(-1.05f, 0.0f, 0.0f)
        );

        pCentroVerde = glm::rotate(
            pCentroVerde,
            glm::radians(90.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        pCentroVerde = glm::scale(
            pCentroVerde,
            glm::vec3(2.10f, 2.0f, 1.05f)
        );

        DibujarPiramideMulticolor(
            pCentroVerde,
            uniformModel,
            uniformColor
        );


        // MAGENTA INVERTIDA
        glm::mat4 pCentroMagenta = glm::mat4(1.0f);

        pCentroMagenta = glm::translate(
            pCentroMagenta,
            glm::vec3(1.05f, 0.0f, 0.0f)
        );

        pCentroMagenta = glm::rotate(
            pCentroMagenta,
            glm::radians(-90.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        pCentroMagenta = glm::scale(
            pCentroMagenta,
            glm::vec3(2.10f, 2.0f, 1.05f)
        );

        DibujarPiramideMulticolor(
            pCentroMagenta,
            uniformModel,
            uniformColor
        );



        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}