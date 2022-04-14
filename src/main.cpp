#include <iostream>
#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "GUI.h"
#include "Water.h"
#include "InstancedModel.h"
#include "Loader.h"
#include "Renderer.h"

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX                   = SCR_WIDTH / 2.0f;
float lastY                   = SCR_HEIGHT / 2.0f;
bool firstMouse               = true;
bool shouldChange             = false;
bool isDay                    = true;
ProgramState ps;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods);
void setDayNightCycle();

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES,10);
    GLFWwindow* window = glfwCreateWindow(800, 600, "ForestTrailGL", NULL, NULL);
    if(window == NULL){
        std::cout << " Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window,key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    ps.lightPosition[0] = glm::vec3 (-5.0f, 4.2f, -6.75f);
    ps.lightPosition[1] = glm::vec3 (8.03f, 4.2f, 7.75f);
    ps.lightPosition[2] = glm::vec3 (0.0f, 250.0f, 0.0f);
    ps.lightColor[0] = glm::vec3 (1.0f);
    ps.lightColor[1] = glm::vec3 (1.0f);
    ps.lightColor[2] = glm::vec3 (1.0f, 1.0f, 1.0f);

    GUI imgui(ps, "../resources/programState.txt");
    imgui.initImGui(window);

    std::unordered_map<std::string, Shader> shaders;
    std::unordered_map<std::string, Model> models;
    std::unordered_map<std::string, InstancedModel> instancedModels;

    TerrainModel terrainModel = loadTerrain(ps.terrainSize);
    WaterModel waterModel = loadWater(ps.waterSize);
    SkyBox skyBox = loadSkybox();
    loadShaders(shaders);
    loadModels(models);
    loadInstancedModels(instancedModels);

    Renderer renderer(models, instancedModels, terrainModel, waterModel, skyBox, ps, shaders );

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window)){
        glGetError();
        processInput(window);

        glClearColor(0.529f, 0.807f, 0.921f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setDayNightCycle();
        glm::mat4 projection = glm::perspective(glm::radians(ps.camera.Zoom),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);
        glm::mat4 view = ps.camera.GetViewMatrix();

        renderer.setMatrices(view, projection);
        renderer.renderScene();

        if(ps.ImguiEnable){
            imgui.DrawImgui();
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    imgui.deinitImgui();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0, width, height);
    ps.SCR_HEIGHT = height;
    ps.SCR_WIDTH = width;
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float speed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        if(ps.camera.Position.y > 0 )
            ps.camera.Position += speed * ps.camera.Front;
        else {
            if(ps.camera.Position.y <= 0)
                if(ps.camera.Front.y > 0 )
                    ps.camera.Position += speed * ps.camera.Front;
        }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        if(ps.camera.Position.y > 0 )
            ps.camera.Position -= speed * ps.camera.Front;
        else {
            if(ps.camera.Position.y <= 0)
                if(ps.camera.Front.y < 0 )
                    ps.camera.Position -= speed * ps.camera.Front;
        }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ps.camera.Position -= glm::normalize(glm::cross(ps.camera.Front, ps.camera.Up)) * speed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        ps.camera.Position += glm::normalize(glm::cross(ps.camera.Front, ps.camera.Up)) * speed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            ps.EnableMouseMovement = true;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            ps.EnableMouseMovement = false;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    if(ps.EnableMouseMovement){
        ps.camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ps.camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        ps.ImguiEnable = !ps.ImguiEnable;
        if (ps.ImguiEnable) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            ps.EnableMouseMovement = false;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            ps.EnableMouseMovement = true;

        }
    }

    if(key == GLFW_KEY_F && action == GLFW_PRESS){

        ps.enableAntialiasing = true;
    }

    if(key == GLFW_KEY_G && action == GLFW_PRESS){

        ps.enableAntialiasing = false;
    }

    if(key == GLFW_KEY_N && action == GLFW_PRESS){
        ps.isDay = false;
        ps.skyBoxChange = true;
    }

    if(key == GLFW_KEY_M && action == GLFW_PRESS){
        ps.isDay = true;
        ps.skyBoxChange = true;
    }

    if(key == GLFW_KEY_O && action == GLFW_PRESS){
        if(ps.shadows)
            ps.shadows = false;
        else ps.shadows = true;
    }
    if(key == GLFW_KEY_P && action == GLFW_PRESS){
        if(ps.lightIndex == 0)
            ps.lightIndex = 1;
        else ps.lightIndex = 0;
    }

}

void setDayNightCycle(){

    if(ps.isDay and ps.skyBoxChange){
        if(ps.skyboxOpacity > 0)
            ps.skyboxOpacity-=0.03;
        else ps.skyBoxChange = false;
    }
    else if(!ps.isDay and ps.skyBoxChange){
        if(ps.skyboxOpacity < 1)
            ps.skyboxOpacity+=0.03;
        else ps.skyBoxChange = false;
    }
}



