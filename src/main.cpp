#include <iostream>
#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "Shader.h"
#include "Model.h"
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "GUI.h"
#include "Water.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
ProgramState ps;
int nightMode = 0;
double y = 0.0;
double g = 1.0;
float i = 0.0;
float j = 1.0;
glm::vec3 bulbPos[] = {
        glm::vec3(-5.0f, 4.2f, -6.75f),
        glm::vec3(8.03f, 4.2f, 7.75f)
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods);
inline void initializeShader(Shader& shader, ProgramState& programState, glm::mat4 &view, glm::mat4& projection);
void setDayNightCycle();

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES,10);
    GLFWwindow* window = glfwCreateWindow(800, 600, "CGraphics", NULL, NULL);
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

    GUI imgui(ps, "../resources/programState.txt");

    imgui.initImGui(window);

    Shader mainShader("../resources/shaders/model.vs", "../resources/shaders/model.fs");
    Shader terrainShader("../resources/shaders/model.vs","../resources/shaders/terrain.fs");

    Shader skyBoxShader("../resources/shaders/skybox.vs","../resources/shaders/skybox.fs");
    Shader waterShader("../resources/shaders/model.vs", "../resources/shaders/water.fs");



    std::string bridgePath = std::filesystem::path("../resources/models/bridge.obj");
    std::string streetLampPath = std::filesystem::path("../resources/models/StreetLamp/StreetLamp.obj");
    std::string lightBulbPath = std::filesystem::path("../resources/models/LightBulb/lightBulb.obj");

    Model bridgeModel(bridgePath.c_str());
    Model streetLampModel(streetLampPath.c_str());
    Model lightBulbModel(lightBulbPath.c_str());

    Terrain terrain(0, 0, 25);
    TerrainModel terrainModel = terrain.generateTerrain();

    Water water(0,0 , 25);
    WaterModel waterModel = water.generateWater();

    std::vector<std::string> skyboxFaces = {
            "../resources/skybox/daylight/right.bmp",
            "../resources/skybox/daylight/left.bmp",
            "../resources/skybox/daylight/top.bmp",
            "../resources/skybox/daylight/bottom.bmp",
            "../resources/skybox/daylight/back.bmp",
            "../resources/skybox/daylight/front.bmp"
    };
    std::vector<std::string> nightSkyboxFaces = {
            "../resources/skybox/nightsky/xneg.jpg",
            "../resources/skybox/nightsky/xpos.jpg",
            "../resources/skybox/nightsky/zpos.jpg",
            "../resources/skybox/nightsky/zneg.jpg",
            "../resources/skybox/nightsky/yneg.jpg",
            "../resources/skybox/nightsky/ypos.jpg"

    };

    SkyBox skyBox(skyboxFaces, nightSkyboxFaces);

    //SkyBox nightSkybox(nightFaces);

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window)){
        glGetError();
        processInput(window);

        glClearColor(0.529f, 0.807f, 0.921f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setDayNightCycle();

        mainShader.use();
        ProgramState &programState = imgui.getPs();
        glm::mat4 projection = glm::perspective(glm::radians(programState.camera.Zoom),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);
        glm::mat4 view = programState.camera.GetViewMatrix();

        initializeShader(mainShader, ps, view, projection);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.6f,1.5f,0.0f));
        model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.05f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0, 0.0, 1.0));

        mainShader.setMat4("model", model);
        bridgeModel.Draw(mainShader);
        model = glm::mat4(1.0f);

        model = glm::scale(model,glm::vec3(0.5f,0.5f,0.5f));
        model = glm::translate(model,glm::vec3(16.0f,-1.0f,15.5f));
        model = glm::rotate(model,glm::radians(60.0f), glm::vec3(0.0,1.0,0.0));

        mainShader.setMat4("model", model);
        streetLampModel.Draw(mainShader);

        model = glm::mat4(1.0f);
        model = glm::scale(model,glm::vec3(0.5f,0.5f,0.5f));
        model = glm::translate(model, glm::vec3(-10.0f,-1.0f,-13.5f));
        model = glm::rotate(model,glm::radians(60.0f), glm::vec3(0.0,1.0,0.0));
        mainShader.setMat4("model", model);
        streetLampModel.Draw(mainShader);

        model = glm::mat4 (1.0f);
        model = glm::translate(model,bulbPos[0]);
        model = glm::scale(model,glm::vec3(0.07f,0.07f,0.07f));

        mainShader.setMat4("model", model);
        lightBulbModel.Draw(mainShader);

        model = glm::mat4 (1.0f);
        model = glm::translate(model,bulbPos[1]);
        model = glm::scale(model,glm::vec3(0.07f,0.07f,0.07f));


        mainShader.setMat4("model", model);
        lightBulbModel.Draw(mainShader);

        mainShader.setMat4("model", model);
        lightBulbModel.Draw(mainShader);



        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(-terrain.getSize()/2.0f * 0.25,-0.50f,-terrain.getSize()/2.0f * 0.25) );
        model = glm::scale(model, glm::vec3(0.25f,0.25f,0.25f));
        terrainShader.use();

        terrainShader.setMat4("model", model);

        initializeShader(terrainShader, ps, view, projection);

        terrainModel.Draw(terrainShader);

        waterShader.use();
        model = glm::mat4(1.0f);


        initializeShader(waterShader, ps, view, projection);
        model = glm::translate(model, glm::vec3(-water.getSize()/2.0f * 0.25f, -1.0f, -water.getSize()/2.0f * 0.25f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        waterShader.setMat4("model", model);
        waterModel.Draw(waterShader);

        glDepthFunc(GL_LEQUAL);
        skyBoxShader.use();
        skyBoxShader.setInt("daySkybox", 0);
        skyBoxShader.setInt("nightSkybox", 1);
        view = glm::mat4(glm::mat3(programState.camera.GetViewMatrix()));
        view = glm::rotate(view, glm::radians((float)glfwGetTime()), glm::vec3(0.0f,1.0f,0.0f));
        skyBoxShader.setMat4("view", view);
        skyBoxShader.setMat4("projection", projection);
        if(nightMode == 1){
            skyBoxShader.setFloat("opacity", (float)y);
        }
        else if(nightMode == 0) {
            skyBoxShader.setFloat("opacity", 0.0);
        }
        else if(nightMode == 2){
            skyBoxShader.setFloat("opacity", (float)g);
        }

        skyBox.Draw();
        glDepthFunc(GL_LESS);



        if(programState.ImguiEnable){
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
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        }
    }
    if(key == GLFW_KEY_F && action == GLFW_PRESS){
        glEnable(GL_MULTISAMPLE);
        ps.enableAntialiasing = true;
    }
    if(key == GLFW_KEY_G && action == GLFW_PRESS){
        glDisable(GL_MULTISAMPLE);
        ps.enableAntialiasing = false;
    }

    if(key == GLFW_KEY_N && action == GLFW_PRESS){
        y = 0.0;
        nightMode = 1;
    }
    if(key == GLFW_KEY_M && action == GLFW_PRESS){
        g = 1.0;
        nightMode = 2;
    }

}
inline void initializeShader(Shader& shader, ProgramState& programState, glm::mat4 &view, glm::mat4& projection){
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setVec3("viewPos",programState.camera.Position);
    shader.setVec3("pointLights[0].position", bulbPos[0]);
    shader.setVec3("pointLights[0].color", programState.lightColor);
    shader.setFloat("pointLights[0].constant", 1.0);
    shader.setFloat("pointLights[0].linear",programState.linear);
    shader.setFloat("pointLights[0].quadratic", programState.quadratic);
    shader.setVec3("pointLights[1].position", bulbPos[1]);
    shader.setVec3("pointLights[1].color", programState.lightColor1);
    shader.setFloat("pointLights[1].constant", 1.0);
    shader.setFloat("pointLights[1].linear",programState.linear);
    shader.setFloat("pointLights[1].quadratic", programState.quadratic);
}
void setDayNightCycle(){

    if(nightMode == 1){
        j=1.0;
        i+=0.01;
    }
    if(y > 1.0 && nightMode == 1 ){
        y = 1.0;
    }
    else if(y < 1.0 && nightMode == 1){
        y = i;
    }

    if(nightMode == 2){
        i= 0.0;
        j-=0.01;
    }
    if(g > 0.0 && nightMode == 2){
        g = j;
    }
    else if(g < 0.0){
        g = 0.0;
    }
}
