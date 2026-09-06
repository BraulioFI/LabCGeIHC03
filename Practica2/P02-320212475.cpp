#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>

// GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Clases
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

const float toRadians = 3.14159265f / 180.0f;
Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// Rutas a los archivos de shaders
static const char* fShaderColor = "shaders/shadercolor.frag";
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* vShaderMagenta = "shaders/shadermagenta.vert";
static const char* vShaderAmarillo = "shaders/shaderamarillo.vert";

void CreaPiramide()
{
    unsigned int indices[] = {
        0, 1, 2,
        1, 3, 2,
        3, 0, 2,
        1, 0, 3
    };

    GLfloat vertices[] = {
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.5f, -0.25f,
         0.0f, -0.5f, -0.5f
    };

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1); // meshList[0] -> Pirámide
}

void CrearCubo()
{
    unsigned int cubo_indices[] = {
        0, 1, 2,  2, 3, 0,
        1, 5, 6,  6, 2, 1,
        7, 6, 5,  5, 4, 7,
        4, 0, 3,  3, 7, 4,
        4, 5, 1,  1, 0, 4,
        3, 2, 6,  6, 7, 3
    };

    GLfloat cubo_vertices[] = {
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };

    Mesh* cubo = new Mesh();
    cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
    meshList.push_back(cubo); // meshList[1] -> Cubo
}

void CrearLetrasBJS()
{
    // ==========================================
    // LETRA 'B' MAYÚSCULA (meshList[2])
    // 6 rectángulos = 36 vértices (108 floats)
    // ==========================================
    GLfloat verticesB[] = {
        // 1. Barra vertical izquierda (completa)
        -0.80f,  0.50f, 0.0f,
        -0.80f, -0.50f, 0.0f,
        -0.68f,  0.50f, 0.0f,
        -0.68f,  0.50f, 0.0f,
        -0.80f, -0.50f, 0.0f,
        -0.68f, -0.50f, 0.0f,

        // 2. Barra horizontal superior
        -0.68f,  0.50f, 0.0f,
        -0.68f,  0.38f, 0.0f,
        -0.40f,  0.50f, 0.0f,
        -0.40f,  0.50f, 0.0f,
        -0.68f,  0.38f, 0.0f,
        -0.40f,  0.38f, 0.0f,

        // 3. Barra horizontal central
        -0.68f,  0.06f, 0.0f,
        -0.68f, -0.06f, 0.0f,
        -0.40f,  0.06f, 0.0f,
        -0.40f,  0.06f, 0.0f,
        -0.68f, -0.06f, 0.0f,
        -0.40f, -0.06f, 0.0f,

        // 4. Barra horizontal inferior
        -0.68f, -0.38f, 0.0f,
        -0.68f, -0.50f, 0.0f,
        -0.40f, -0.38f, 0.0f,
        -0.40f, -0.38f, 0.0f,
        -0.68f, -0.50f, 0.0f,
        -0.40f, -0.50f, 0.0f,

        // 5. Lateral vertical superior derecho
        -0.52f,  0.50f, 0.0f,
        -0.52f,  0.06f, 0.0f,
        -0.40f,  0.50f, 0.0f,
        -0.40f,  0.50f, 0.0f,
        -0.52f,  0.06f, 0.0f,
        -0.40f,  0.06f, 0.0f,

        // 6. Lateral vertical inferior derecho
        -0.52f, -0.06f, 0.0f,
        -0.52f, -0.50f, 0.0f,
        -0.40f, -0.06f, 0.0f,
        -0.40f, -0.06f, 0.0f,
        -0.52f, -0.50f, 0.0f,
        -0.40f, -0.50f, 0.0f
    };

    unsigned int indicesB[36];
    for (int i = 0; i < 36; i++) indicesB[i] = i;

    Mesh* letraB = new Mesh();
    letraB->CreateMesh(verticesB, indicesB, 108, 36);
    meshList.push_back(letraB); // meshList[2] -> Letra B

    // ==========================================
    // LETRA 'J' MAYÚSCULA (meshList[3])
    // 3 rectángulos = 18 vértices (54 floats)
    // ==========================================
    GLfloat verticesJ[] = {
        // 1. Barra superior
        -0.25f,  0.50f, 0.0f,
        -0.25f,  0.38f, 0.0f,
         0.25f,  0.50f, 0.0f,
         0.25f,  0.50f, 0.0f,
        -0.25f,  0.38f, 0.0f,
         0.25f,  0.38f, 0.0f,

         // 2. Barra vertical derecha
          0.00f,  0.38f, 0.0f,
          0.00f, -0.38f, 0.0f,
          0.13f,  0.38f, 0.0f,
          0.13f,  0.38f, 0.0f,
          0.00f, -0.38f, 0.0f,
          0.13f, -0.38f, 0.0f,

          // 3. Base horizontal curva
          -0.25f, -0.38f, 0.0f,
          -0.25f, -0.50f, 0.0f,
           0.13f, -0.38f, 0.0f,
           0.13f, -0.38f, 0.0f,
          -0.25f, -0.50f, 0.0f,
           0.13f, -0.50f, 0.0f
    };

    unsigned int indicesJ[18];
    for (int i = 0; i < 18; i++) indicesJ[i] = i;

    Mesh* letraJ = new Mesh();
    letraJ->CreateMesh(verticesJ, indicesJ, 54, 18);
    meshList.push_back(letraJ); // meshList[3] -> Letra J

    // ==========================================
    // LETRA 'S' MAYÚSCULA (meshList[4])
    // 5 rectángulos = 30 vértices (90 floats)
    // ==========================================
    GLfloat verticesS[] = {
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

    unsigned int indicesS[30];
    for (int i = 0; i < 30; i++) indicesS[i] = i;

    Mesh* letraS = new Mesh();
    letraS->CreateMesh(verticesS, indicesS, 90, 30);
    meshList.push_back(letraS); // meshList[4] -> Letra S
}

void CreateShaders()
{
    Shader* shRojo = new Shader();
    shRojo->CreateFromFiles(vShaderRojo, fShaderColor);
    shaderList.push_back(*shRojo); // shaderList[0] -> Rojo

    Shader* shVerde = new Shader();
    shVerde->CreateFromFiles(vShaderVerde, fShaderColor);
    shaderList.push_back(*shVerde); // shaderList[1] -> Verde

    Shader* shAzul = new Shader();
    shAzul->CreateFromFiles(vShaderAzul, fShaderColor);
    shaderList.push_back(*shAzul); // shaderList[2] -> Azul

    Shader* shCafe = new Shader();
    shCafe->CreateFromFiles(vShaderCafe, fShaderColor);
    shaderList.push_back(*shCafe); // shaderList[3] -> Café

    Shader* shMagenta = new Shader();
    shMagenta->CreateFromFiles(vShaderMagenta, fShaderColor);
    shaderList.push_back(*shMagenta); // shaderList[4] -> Magenta

    Shader* shAmarillo = new Shader();
    shAmarillo->CreateFromFiles(vShaderAmarillo, fShaderColor);
    shaderList.push_back(*shAmarillo); // shaderList[5] -> Amarillo
}

void RenderObjeto(int shaderIndex, int meshIndex, glm::mat4 model, glm::mat4 projection)
{
    // Inclinación en X e Y para visualizar la profundidad 3D (estilo isométrico)
    model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Inclinación vertical
    model = glm::rotate(model, glm::radians(70.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Giro lateral

    shaderList[shaderIndex].useShader();
    GLuint uModel = shaderList[shaderIndex].getModelLocation();
    GLuint uProj = shaderList[shaderIndex].getProjectLocation();
    glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
    meshList[meshIndex]->RenderMesh();
}

void RenderLetra(int shaderIndex, int meshIndex, glm::mat4 model, glm::mat4 projection)
{
    shaderList[shaderIndex].useShader();
    GLuint uModel = shaderList[shaderIndex].getModelLocation();
    GLuint uProj = shaderList[shaderIndex].getProjectLocation();
    glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
    meshList[meshIndex]->RenderMesh();
}

int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();

    CreaPiramide();     // meshList[0] -> Pirámide
    CrearCubo();         // meshList[1] -> Cubo
    CrearLetrasBJS();    // meshList[2]=B, meshList[3]=J, meshList[4]=S
    CreateShaders();

    glm::mat4 projection = glm::ortho(
        -1.0f, 1.0f,
        -1.0f, 1.0f,
        0.1f, 100.0f
    );

    glm::mat4 model(1.0f);

    while (!mainWindow.getShouldClose())
    {
        glfwPollEvents();

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // =====================================================================
        // LETRAS: B J S (Parte Superior: Y = 0.65f)
        // Letra B: Rojo (shaderList[0])
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.65f, -4.0f));
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 1.0f));
        RenderLetra(0, 2, model, projection);

        // Letra J: Amarillo (shaderList[5])
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.65f, -4.0f));
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 1.0f));
        RenderLetra(5, 3, model, projection);

        // Letra S: Azul (shaderList[2])
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.65f, -4.0f));
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 1.0f));
        RenderLetra(2, 4, model, projection);


        // =====================================================================
        // FIGURA 1: PIRÁMIDES APILADAS CON CUBOS ALARGADOS (X = -0.60f)
        float f1X = -0.60f;

        // Pirámide Superior (Amarillo)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f1X, 0.16f, -4.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.24f, 0.14f, 0.24f));
        RenderObjeto(5, 0, model, projection);

        // Pirámide Media (Rojo)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f1X, 0.01f, -4.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.24f, 0.14f, 0.24f));
        RenderObjeto(0, 0, model, projection);

        // Pirámide Inferior (Verde)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f1X, -0.14f, -4.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.24f, 0.14f, 0.24f));
        RenderObjeto(1, 0, model, projection);

        // Barra Lateral Izquierda (Cubo Café)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f1X - 0.15f, 0.06f, -4.0f));
        model = glm::scale(model, glm::vec3(0.045f, 0.5f, 0.045f));
        RenderObjeto(3, 1, model, projection);

        // Barra Lateral Derecha (Cubo Café)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f1X + 0.15f, 0.06f, -4.0f));
        model = glm::scale(model, glm::vec3(0.045f, 0.5f, 0.045f));
        RenderObjeto(3, 1, model, projection);


        // =====================================================================
        // FIGURA 2: CUADRO CON ROMBOS CONCÉNTRICOS (Centro: X = 0.0f)
        float f2X = 0.0f;
        float tamMarco = 0.45f;
        float mitadMarco = tamMarco / 2.0f;       // 0.225f
        float cuartoMarco = tamMarco / 4.0f;      // 0.1125f

        // ---------------------------------------------------------------------
        // 1. CUATRO ESQUINAS EXTERIORES (Capa Fondo, Z = -4.0f)
        // 4 cubos rectos que forman los cuadrantes. El rombo tapará el centro.
        // Sup Izq (Amarillo -> Shader 5)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f2X - cuartoMarco, cuartoMarco, -4.0f));
        model = glm::scale(model, glm::vec3(mitadMarco, mitadMarco, 0.05f));
        RenderObjeto(5, 1, model, projection); // meshList[1] = Cubo

        // Sup Der (Rojo -> Shader 0)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f2X + cuartoMarco, cuartoMarco, -4.0f));
        model = glm::scale(model, glm::vec3(mitadMarco, mitadMarco, 0.05f));
        RenderObjeto(0, 1, model, projection);

        // Inf Izq (Magenta -> Shader 4)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f2X - cuartoMarco, -cuartoMarco, -4.0f));
        model = glm::scale(model, glm::vec3(mitadMarco, mitadMarco, 0.05f));
        RenderObjeto(4, 1, model, projection);

        // Inf Der (Verde -> Shader 1)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f2X + cuartoMarco, -cuartoMarco, -4.0f));
        model = glm::scale(model, glm::vec3(mitadMarco, mitadMarco, 0.05f));
        RenderObjeto(1, 1, model, projection);

        // ---------------------------------------------------------------------
        // 2. ROMBO EXTERIOR AZUL (Capa Media, Z = -3.9f)
        // Hecho de 2 Pirámides (Sin rotar en Z para evitar la deformación)
        // Mitad Superior
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f2X, cuartoMarco - 0.02, -3.9f));
        model = glm::scale(model, glm::vec3(tamMarco, mitadMarco + 0.02, 0.10f));
        RenderObjeto(2, 0, model, projection); // meshList[0] = Pirámide

        // Mitad Inferior
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f2X, -cuartoMarco - 0.01, -3.9f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        // 2. Rota 180° sobre el eje Y
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(tamMarco, mitadMarco, 0.10f));
        RenderObjeto(2, 0, model, projection);

        // ---------------------------------------------------------------------
        // 3. ROMBO INTERIOR CAFÉ (Capa Frontal, Z = -3.8f)
        // Hecho de 2 Pirámides concéntricas más pequeñas
        float tamCafe = tamMarco * 0.50f;
        float mitadCafe = tamCafe / 2.0f;
        float cuartoCafe = tamCafe / 4.0f;

        // Mitad Superior
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f2X, cuartoCafe - 0.025, -3.8f));
        model = glm::scale(model, glm::vec3(tamCafe, mitadCafe, 0.10f));
        RenderObjeto(3, 0, model, projection);

        // Mitad Inferior
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f2X, -cuartoCafe - 0.01, -3.8f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        // 2. Rota 180° sobre el eje Y
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(tamCafe, mitadCafe, 0.10f));
        RenderObjeto(3, 0, model, projection);


        // =====================================================================
        // FIGURA 3: TRIÁNGULO DE 4 PIRÁMIDES (Derecha: X = 0.60f)
        float f3X = 0.60f;
        float subEscala = 0.225f;

        // Pirámide Superior (Magenta)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f3X, 0.1125f, -4.0f));
        model = glm::scale(model, glm::vec3(subEscala, subEscala, subEscala));
        RenderObjeto(4, 0, model, projection);

        // Pirámide Inferior Izquierda (Verde)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f3X - 0.1125f, -0.1125f, -4.0f));
        model = glm::scale(model, glm::vec3(subEscala, subEscala, subEscala));
        RenderObjeto(1, 0, model, projection);

        // Pirámide Inferior Derecha (Verde)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f3X + 0.1125f, -0.1125f, -4.0f));
        model = glm::scale(model, glm::vec3(subEscala, subEscala, subEscala));
        RenderObjeto(0, 0, model, projection);

        // Pirámide Central Invertida (Azul) - Forma la bipirámide y está rotada en Y
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(f3X, -0.09f, -4.0f));
        // 1. Invierte la pirámide (apunta hacia abajo)
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        // 2. Rota 180° sobre el eje Y
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(subEscala, subEscala, subEscala));
        RenderObjeto(5, 0, model, projection);

        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}