
//Practica 4: Modelado Jerarquico

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\random.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"

using std::vector;

const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

GLfloat rotArt1 = 35.0f;
GLfloat rotArt2 = -80.0f;
GLfloat rotArt3 = 80.0f;

GLfloat rotLlantaFC = 0.0f;
GLfloat rotLlantaFL = 0.0f;
GLfloat rotLlantaMC = 0.0f;
GLfloat rotLlantaML = 0.0f;
GLfloat rotLlantaTC = 0.0f;
GLfloat rotLlantaTL = 0.0f;

Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader> shaderList;

static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";

Sphere sp = Sphere(1.0f, 20, 20);


// ============================================================
// CUBO
void CrearCubo()
{
    unsigned int indices[] = {
        0,1,2, 2,3,0,
        1,5,6, 6,2,1,
        7,6,5, 5,4,7,
        4,0,3, 3,7,4,
        4,5,1, 1,0,4,
        3,2,6, 6,7,3
    };

    GLfloat vertices[] = {
        -0.5f,-0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f
    };

    Mesh* cubo = new Mesh();
    cubo->CreateMesh(vertices, indices, 24, 36);
    meshList.push_back(cubo);
}


// ============================================================
// CILINDRO
void CrearCilindro(int res, float R)
{
    int n, i;
    GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    for (n = 0; n <= res; n++)
    {
        if (n != res) { x = R * cos(n * dt); z = R * sin(n * dt); }
        else { x = R; z = 0.0f; }

        vertices.push_back(x); vertices.push_back(y); vertices.push_back(z);
        vertices.push_back(x); vertices.push_back(0.5f); vertices.push_back(z);
    }

    for (n = 0; n <= res; n++)
    {
        x = R * cos(n * dt); z = R * sin(n * dt);
        vertices.push_back(x); vertices.push_back(-0.5f); vertices.push_back(z);
    }

    for (n = 0; n <= res; n++)
    {
        x = R * cos(n * dt); z = R * sin(n * dt);
        vertices.push_back(x); vertices.push_back(0.5f); vertices.push_back(z);
    }

    for (i = 0; i < vertices.size(); i++) indices.push_back(i);

    Mesh* cilindro = new Mesh();
    cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
    meshList.push_back(cilindro);
}


// ============================================================
// SHADER
void CreateShaders()
{
    Shader* shader = new Shader();
    shader->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader);
}


// ============================================================
// FUNCIONES PARA DIBUJAR
void DibujarCubo(glm::mat4 model, glm::vec3 color, GLuint uniformModel, GLuint uniformColor)
{
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();
}

void DibujarEsfera(glm::mat4 model, glm::vec3 color, GLuint uniformModel, GLuint uniformColor)
{
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    sp.render();
}


// ============================================================
// LLANTA CON RIN GRIS VERTICAL
void DibujarLlantaConRin(glm::mat4 baseLlanta, GLfloat rotLlanta, GLuint uniformModel, GLuint uniformColor)
{
    glm::mat4 model;
    glm::vec3 color;

    // Centro de la llanta
    glm::mat4 centro = baseLlanta;
    centro = glm::translate(centro, glm::vec3(-2.4f, 0.0f, 0.0f));

    // Giramos el cilindro para que la llanta quede vertical
    centro = glm::rotate(centro, glm::radians(rotLlanta), glm::vec3(0.0f, 1.0f, 0.0f));


    // ========================================================
    // LLANTA NEGRA
    model = centro;
    model = glm::scale(model, glm::vec3(1.30f, 0.45f, 1.30f));

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

    color = glm::vec3(0.04f, 0.04f, 0.04f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));

    meshList[1]->RenderMeshGeometry();


    // ========================================================
    // 5 RAYOS VERTICALES DEL RIN
    for (int i = 0; i < 5; i++)
    {
        model = centro;

        // Girar alrededor del eje de la rueda
        model = glm::rotate(model, glm::radians(i * 72.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Colocar el rayo desde el centro hacia afuera
        model = glm::translate(model, glm::vec3(0.0f, 0.25f, 0.55f));

        // Rayo largo y delgado
        model = glm::scale(model, glm::vec3(0.13f, 0.10f, 1.05f));

        DibujarCubo(
            model,
            glm::vec3(0.60f, 0.60f, 0.60f),
            uniformModel,
            uniformColor
        );
    }


    // ========================================================
    // ARO GRIS VERTICAL
    for (int i = 0; i < 20; i++)
    {
        model = centro;

        // Girar alrededor del centro de la rueda
        model = glm::rotate(model, glm::radians(i * 18.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Posición alrededor del borde
        model = glm::translate(model, glm::vec3(0.0f, 0.25f, 1.02f));

        // Segmento del aro
        model = glm::scale(model, glm::vec3(0.35f, 0.10f, 0.13f));

        DibujarCubo(
            model,
            glm::vec3(0.55f, 0.55f, 0.55f),
            uniformModel,
            uniformColor
        );
    }


    // ========================================================
    // CENTRO GRIS DEL RIN
    model = centro;
    model = glm::translate(model, glm::vec3(0.0f, 0.25f, 0.0f));
    model = glm::scale(model, glm::vec3(0.38f, 0.22f, 0.38f));

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

    color = glm::vec3(0.60f, 0.60f, 0.60f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));

    meshList[1]->RenderMeshGeometry();


    // ========================================================
    // CENTRO OSCURO
    model = centro;
    model = glm::translate(model, glm::vec3(0.0f, 0.38f, 0.0f));
    model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

    color = glm::vec3(0.15f, 0.15f, 0.15f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));

    meshList[1]->RenderMeshGeometry();
}


// ============================================================
// MAIN
int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();

    CrearCubo();               // indice 0
    CrearCilindro(30, 1.0f);  // indice 1
    CreateShaders();

    sp.init();
    sp.load();


    // ========================================================
    // CAMARA 
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);


    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    glm::mat4 model(1.0f);
    glm::mat4 modelaux(1.0f);
    glm::mat4 modelaux2(1.0f);

    glm::vec3 color(0.0f);


    while (!mainWindow.getShouldClose())
    {
        // ====================================================
        // CAMARA ORIGINAL
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;

        glfwPollEvents();

        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        bool* keys = mainWindow.getsKeys();

        // ====================================================
        // CONTROL DEL BRAZO
        if (keys[GLFW_KEY_F]) rotArt1 += 1.0f;
        if (keys[GLFW_KEY_G]) rotArt1 -= 1.0f;

        if (keys[GLFW_KEY_H]) rotArt2 += 1.0f;
        if (keys[GLFW_KEY_I]) rotArt2 -= 1.0f;

        if (keys[GLFW_KEY_J]) rotArt3 += 1.0f;
        if (keys[GLFW_KEY_K]) rotArt3 -= 1.0f;


        // ====================================================
        // CONTROL DE LAS 6 LLANTAS
        // Frontal cerca
        if (keys[GLFW_KEY_1]) rotLlantaFC += 2.0f;

        // Frontal lejos
        if (keys[GLFW_KEY_2]) rotLlantaFL += 2.0f;

        // Medio cerca
        if (keys[GLFW_KEY_3]) rotLlantaMC += 2.0f;

        // Medio lejos
        if (keys[GLFW_KEY_4]) rotLlantaML += 2.0f;

        // Trasera cerca
        if (keys[GLFW_KEY_5]) rotLlantaTC += 2.0f;

        // Trasera lejos
        if (keys[GLFW_KEY_6]) rotLlantaTL += 2.0f;


        // Limites de las articulaciones
        rotArt1 = glm::clamp(rotArt1, -80.0f, 80.0f);
        rotArt2 = glm::clamp(rotArt2, -140.0f, 20.0f);
        rotArt3 = glm::clamp(rotArt3, 20.0f, 140.0f);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].useShader();

        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();
        uniformView = shaderList[0].getViewLocation();
        uniformColor = shaderList[0].getColorLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));


        // ====================================================
        // CENTRO DEL ROVER
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 4.0f, -4.0f));
        modelaux = model;

        // ====================================================
        // CABINA 
        model = modelaux;
        model = glm::translate(model, glm::vec3(1.0f, 2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 4.0f, 6.0f));
        DibujarCubo(model, glm::vec3(0.60f, 0.60f, 0.60f), uniformModel, uniformColor);

        // ====================================================
        // BASE 
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));

        modelaux = model;
        modelaux2 = model;

        model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f));
        DibujarCubo(model, glm::vec3(0.60f, 0.60f, 0.60f), uniformModel, uniformColor);


        // ====================================================
        // BASE LLANTA FRONTAL CERCA
        model = modelaux2;
        model = glm::translate(model, glm::vec3(-4.8f, -1.25f, 4.15f));
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 baseLlantaFrontalCerca = model;

        model = glm::scale(model, glm::vec3(4.8f, 0.28f, 0.35f));
        DibujarCubo(model, glm::vec3(0.65f), uniformModel, uniformColor);


        // ====================================================
        // BASE LLANTA FRONTAL LEJOS
        model = modelaux2;
        model = glm::translate(model, glm::vec3(-4.8f, -1.25f, -4.15f));
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 baseLlantaFrontalLejos = model;

        model = glm::scale(model, glm::vec3(4.8f, 0.28f, 0.35f));
        DibujarCubo(model, glm::vec3(0.65f), uniformModel, uniformColor);

        // ====================================================
        // BASE LLANTA MEDIO CERCA (Pestaña horizontal hacia -X)
        model = modelaux2;
        // Punto de anclaje en el chasis
        model = glm::translate(model, glm::vec3(0.0f, -0.10f, 4.15f));
        glm::mat4 origenMC = model;

        // 1. Tramo corto horizontal arriba: se proyecta hacia -X
        model = glm::translate(origenMC, glm::vec3(-0.6f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.2f, 0.28f, 0.35f));
        DibujarCubo(model, glm::vec3(0.65f), uniformModel, uniformColor);

        // 2. Tramo largo vertical: desciende desde la esquina hacia el piso
        glm::mat4 esquinaMC = glm::translate(origenMC, glm::vec3(-1.2f, 0.0f, 0.0f));
        model = glm::translate(esquinaMC, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.35f, 2.0f, 0.28f));
        DibujarCubo(model, glm::vec3(0.65f), uniformModel, uniformColor);

        // Anclaje de la rueda 
        glm::mat4 baseLlantaMedioCerca = glm::translate(esquinaMC, glm::vec3(2.4f, -2.0f, 0.0f));
        baseLlantaMedioCerca = glm::rotate(baseLlantaMedioCerca, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


        // ====================================================
        // BASE LLANTA MEDIO LEJOS (Pestaña horizontal hacia -X)
        model = modelaux2;
        model = glm::translate(model, glm::vec3(0.0f, -0.10f, -4.15f));
        glm::mat4 origenML = model;

        model = glm::translate(origenML, glm::vec3(-0.6f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.2f, 0.28f, 0.35f));
        DibujarCubo(model, glm::vec3(0.65f), uniformModel, uniformColor);

        glm::mat4 esquinaML = glm::translate(origenML, glm::vec3(-1.2f, 0.0f, 0.0f));
        model = glm::translate(esquinaML, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.35f, 2.0f, 0.28f));
        DibujarCubo(model, glm::vec3(0.65f), uniformModel, uniformColor);

        glm::mat4 baseLlantaMedioLejos = glm::translate(esquinaML, glm::vec3(2.4f, -2.0f, 0.0f));
        baseLlantaMedioLejos = glm::rotate(baseLlantaMedioLejos, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


        // ====================================================
        // BASE LLANTA TRASERA CERCA (Pestaña horizontal hacia +X)
        model = modelaux2;
        model = glm::translate(model, glm::vec3(2.6f, -0.10f, 4.15f));
        glm::mat4 origenTC = model;

        model = glm::translate(origenTC, glm::vec3(0.6f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.2f, 0.28f, 0.35f));
        DibujarCubo(model, glm::vec3(0.65f), uniformModel, uniformColor);

        glm::mat4 esquinaTC = glm::translate(origenTC, glm::vec3(1.2f, 0.0f, 0.0f));
        model = glm::translate(esquinaTC, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.35f, 2.0f, 0.28f));
        DibujarCubo(model, glm::vec3(0.65f), uniformModel, uniformColor);

        glm::mat4 baseLlantaTraseraCerca = glm::translate(esquinaTC, glm::vec3(2.4f, -2.0f, 0.0f));
        baseLlantaTraseraCerca = glm::rotate(baseLlantaTraseraCerca, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


        // ====================================================
        // BASE LLANTA TRASERA LEJOS (Pestaña horizontal hacia +X)
        model = modelaux2;
        model = glm::translate(model, glm::vec3(2.6f, -0.10f, -4.15f));
        glm::mat4 origenTL = model;

        model = glm::translate(origenTL, glm::vec3(0.6f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.2f, 0.28f, 0.35f));
        DibujarCubo(model, glm::vec3(0.65f), uniformModel, uniformColor);

        glm::mat4 esquinaTL = glm::translate(origenTL, glm::vec3(1.2f, 0.0f, 0.0f));
        model = glm::translate(esquinaTL, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.35f, 2.0f, 0.28f));
        DibujarCubo(model, glm::vec3(0.65f), uniformModel, uniformColor);

        glm::mat4 baseLlantaTraseraLejos = glm::translate(esquinaTL, glm::vec3(2.4f, -2.0f, 0.0f));
        baseLlantaTraseraLejos = glm::rotate(baseLlantaTraseraLejos, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
        // ====================================================
        // LLANTAS CON RIN
        DibujarLlantaConRin(baseLlantaFrontalCerca, rotLlantaFC, uniformModel, uniformColor);
        DibujarLlantaConRin(baseLlantaFrontalLejos, rotLlantaFL, uniformModel, uniformColor);
        DibujarLlantaConRin(baseLlantaMedioCerca, rotLlantaMC, uniformModel, uniformColor);
        DibujarLlantaConRin(baseLlantaMedioLejos, rotLlantaML, uniformModel, uniformColor);
        DibujarLlantaConRin(baseLlantaTraseraCerca, rotLlantaTC, uniformModel, uniformColor);
        DibujarLlantaConRin(baseLlantaTraseraLejos, rotLlantaTL, uniformModel, uniformColor);

        // ====================================================
        // BRAZO ROBOTICO
        model = modelaux2;
        model = glm::translate(model, glm::vec3(-3.8f, 1.0f, 0.0f));

        glm::mat4 articulacion1 = model;


        // Articulacion 1
        model = articulacion1;
        model = glm::scale(model, glm::vec3(0.45f));
        DibujarEsfera(model, glm::vec3(0.0f, 0.0f, 1.0f), uniformModel, uniformColor);


        // Brazo 1
        glm::mat4 brazo1 = articulacion1;
        brazo1 = glm::rotate(brazo1, glm::radians(rotArt1), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 sistemaBrazo1 = brazo1;

        model = glm::translate(brazo1, glm::vec3(0.0f, 1.6f, 0.0f));
        model = glm::scale(model, glm::vec3(0.55f, 3.2f, 0.75f));
        DibujarCubo(model, glm::vec3(0.65f), uniformModel, uniformColor);


        // Articulacion 2
        glm::mat4 articulacion2 = sistemaBrazo1;
        articulacion2 = glm::translate(articulacion2, glm::vec3(0.0f, 3.2f, 0.0f));

        model = articulacion2;
        model = glm::scale(model, glm::vec3(0.40f));
        DibujarEsfera(model, glm::vec3(0.0f, 0.0f, 1.0f), uniformModel, uniformColor);


        // Brazo 2
        glm::mat4 brazo2 = articulacion2;
        brazo2 = glm::rotate(brazo2, glm::radians(rotArt2), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 sistemaBrazo2 = brazo2;

        model = glm::translate(brazo2, glm::vec3(0.0f, 1.5f, 0.0f));
        model = glm::scale(model, glm::vec3(0.50f, 3.0f, 0.70f));
        DibujarCubo(model, glm::vec3(0.75f), uniformModel, uniformColor);


        // Articulacion 3
        glm::mat4 articulacion3 = sistemaBrazo2;
        articulacion3 = glm::translate(articulacion3, glm::vec3(0.0f, 3.0f, 0.0f));

        model = articulacion3;
        model = glm::scale(model, glm::vec3(0.35f));
        DibujarEsfera(model, glm::vec3(0.0f, 0.0f, 1.0f), uniformModel, uniformColor);

        // ============================================================
        // SOPORTE DE LA PINZA (Alineado con el primer brazo: +80 deg)
        glm::mat4 pinza = articulacion3;

        // Rota +80 grados para cancelar los -80 del segundo brazo y recuperar los +35 iniciales
        pinza = glm::rotate(pinza, glm::radians(rotArt3), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(pinza, glm::vec3(0.0f, 0.55f, 0.0f));

        model = glm::scale(model,glm::vec3(0.35f, 0.9f, 0.45f));

        glUniformMatrix4fv(
            uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        color = glm::vec3(0.80f, 0.80f, 0.80f);

        glUniform3fv(uniformColor,1, glm::value_ptr(color));

        meshList[0]->RenderMesh();


        // ============================================================
        // PINZA IZQUIERDA (Hereda la orientación de 'pinza')
        model = pinza;

        model = glm::translate(
            model,
            glm::vec3(-0.28f, 1.05f, 0.0f)
        );

        model = glm::rotate(
            model,
            glm::radians(20.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

        model = glm::scale(
            model,
            glm::vec3(0.18f, 0.75f, 0.25f)
        );

        glUniformMatrix4fv(
            uniformModel,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        color = glm::vec3(0.9f, 0.5f, 0.0f);

        glUniform3fv(
            uniformColor,
            1,
            glm::value_ptr(color)
        );

        meshList[0]->RenderMesh();


        // ============================================================
        // PINZA DERECHA (Hereda la orientación de 'pinza')
        model = pinza;

        model = glm::translate(
            model,
            glm::vec3(0.28f, 1.05f, 0.0f)
        );

        model = glm::rotate(
            model,
            glm::radians(-20.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

        model = glm::scale(
            model,
            glm::vec3(0.18f, 0.75f, 0.25f)
        );

        glUniformMatrix4fv(
            uniformModel,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        color = glm::vec3(0.9f, 0.5f, 0.0f);

        glUniform3fv(
            uniformColor,
            1,
            glm::value_ptr(color)
        );

        meshList[0]->RenderMesh();

        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}


/*
// Practica 4: Sonda espacial con modelado jerarquico 
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"

using std::vector;

const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader> shaderList;

static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";

Sphere sp = Sphere(1.0f, 20, 20);


// ============================================================
// CUBO
void CrearCubo()
{
    unsigned int indices[] = {
        0,1,2, 2,3,0,
        1,5,6, 6,2,1,
        7,6,5, 5,4,7,
        4,0,3, 3,7,4,
        4,5,1, 1,0,4,
        3,2,6, 6,7,3
    };

    GLfloat vertices[] = {
        -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f
    };

    Mesh* cubo = new Mesh();
    cubo->CreateMesh(vertices, indices, 24, 36);
    meshList.push_back(cubo);
}


// ============================================================
// CILINDRO
void CrearCilindro(int res, float R)
{
    int n, i;
    GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    for (n = 0; n <= res; n++)
    {
        x = R * cos(n * dt); z = R * sin(n * dt);
        vertices.push_back(x); vertices.push_back(y); vertices.push_back(z);
        vertices.push_back(x); vertices.push_back(0.5f); vertices.push_back(z);
    }

    for (n = 0; n <= res; n++)
    {
        x = R * cos(n * dt); z = R * sin(n * dt);
        vertices.push_back(x); vertices.push_back(-0.5f); vertices.push_back(z);
    }

    for (n = 0; n <= res; n++)
    {
        x = R * cos(n * dt); z = R * sin(n * dt);
        vertices.push_back(x); vertices.push_back(0.5f); vertices.push_back(z);
    }

    for (i = 0; i < vertices.size(); i++) indices.push_back(i);

    Mesh* cilindro = new Mesh();
    cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
    meshList.push_back(cilindro);
}


// ============================================================
// SHADER
void CreateShaders()
{
    Shader* shader = new Shader();
    shader->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader);
}


// ============================================================
// FUNCIONES DE DIBUJO
void DibujarCubo(glm::mat4 model, glm::vec3 color, GLuint uniformModel, GLuint uniformColor)
{
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();
}

void DibujarCilindro(glm::mat4 model, glm::vec3 color, GLuint uniformModel, GLuint uniformColor)
{
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMeshGeometry();
}

void DibujarEsfera(glm::mat4 model, glm::vec3 color, GLuint uniformModel, GLuint uniformColor)
{
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    sp.render();
}


// ============================================================
// MAIN
int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();

    CrearCubo();               // indice 0
    CrearCilindro(30, 1.0f);  // indice 1
    CreateShaders();

    sp.init();
    sp.load();


    // ========================================================
    // CAMARA ORIGINAL
    // ========================================================

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    glm::mat4 model(1.0f);
    glm::mat4 raiz(1.0f);

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;

        glfwPollEvents();

        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].useShader();

        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();
        uniformView = shaderList[0].getViewLocation();
        uniformColor = shaderList[0].getColorLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));


        // ====================================================
        // RAIZ DE LA SONDA
        raiz = glm::mat4(1.0f);
        raiz = glm::translate(raiz, glm::vec3(0.0f, 2.0f, -8.0f));


        // ====================================================
        // CUERPO PRINCIPAL
        model = raiz;
        model = glm::scale(model, glm::vec3(3.0f, 2.2f, 2.5f));
        DibujarCubo(model, glm::vec3(0.65f, 0.65f, 0.65f), uniformModel, uniformColor);


        // ====================================================
        // PANEL SOLAR IZQUIERDO
        glm::mat4 panelIzq = raiz;
        panelIzq = glm::translate(panelIzq, glm::vec3(-2.5f, 0.0f, 0.0f));

        model = panelIzq;
        model = glm::scale(model, glm::vec3(2.0f, 0.20f, 0.30f));
        DibujarCubo(model, glm::vec3(0.70f), uniformModel, uniformColor);

        model = panelIzq;
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 1.8f, 0.15f));
        DibujarCubo(model, glm::vec3(0.0f, 0.20f, 0.65f), uniformModel, uniformColor);


        // ====================================================
        // PANEL SOLAR DERECHO
        glm::mat4 panelDer = raiz;
        panelDer = glm::translate(panelDer, glm::vec3(2.5f, 0.0f, 0.0f));

        model = panelDer;
        model = glm::scale(model, glm::vec3(2.0f, 0.20f, 0.30f));
        DibujarCubo(model, glm::vec3(0.70f), uniformModel, uniformColor);

        model = panelDer;
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 1.8f, 0.15f));
        DibujarCubo(model, glm::vec3(0.0f, 0.20f, 0.65f), uniformModel, uniformColor);


        // ====================================================
        // MASTIL SUPERIOR
        glm::mat4 mastil = raiz;
        mastil = glm::translate(mastil, glm::vec3(0.0f, 1.8f, 0.0f));

        model = mastil;
        model = glm::scale(model, glm::vec3(0.30f, 1.5f, 0.30f));
        DibujarCubo(model, glm::vec3(0.75f), uniformModel, uniformColor);


        // ====================================================
        // BASE DE LA ANTENA
        glm::mat4 antena = mastil;
        antena = glm::translate(antena, glm::vec3(0.0f, 1.5f, 0.0f));

        model = antena;
        model = glm::scale(model, glm::vec3(0.55f));
        DibujarEsfera(model, glm::vec3(0.20f, 0.20f, 0.20f), uniformModel, uniformColor);


        // ====================================================
        // PLATO DE LA ANTENA
        model = antena;
        model = glm::translate(model, glm::vec3(0.0f, 0.40f, 0.0f));
        model = glm::scale(model, glm::vec3(1.3f, 0.25f, 1.3f));
        DibujarEsfera(model, glm::vec3(0.85f, 0.85f, 0.85f), uniformModel, uniformColor);


        // ====================================================
        // PUNTA DE LA ANTENA
        model = antena;
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.12f, 1.2f, 0.12f));
        DibujarCubo(model, glm::vec3(0.80f), uniformModel, uniformColor);


        // ====================================================
        // BRAZO FRONTAL
        glm::mat4 brazo = raiz;
        brazo = glm::translate(brazo, glm::vec3(0.0f, -0.4f, 1.8f));

        model = brazo;
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.20f, 2.0f, 0.20f));
        DibujarCilindro(model, glm::vec3(0.75f), uniformModel, uniformColor);


        // ====================================================
        // SENSOR FRONTAL
        glm::mat4 sensor = brazo;
        sensor = glm::translate(sensor, glm::vec3(0.0f, 0.0f, 2.0f));

        model = sensor;
        model = glm::scale(model, glm::vec3(0.65f));
        DibujarEsfera(model, glm::vec3(1.0f, 0.30f, 0.0f), uniformModel, uniformColor);


        // ====================================================
        // SENSOR INFERIOR
        model = raiz;
        model = glm::translate(model, glm::vec3(0.0f, -1.6f, 0.0f));
        model = glm::scale(model, glm::vec3(0.60f, 0.80f, 0.60f));
        DibujarCilindro(model, glm::vec3(0.40f, 0.40f, 0.40f), uniformModel, uniformColor);


        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}
*/