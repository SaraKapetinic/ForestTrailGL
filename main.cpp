#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "model/stb_image.h"
#include "model/Shader.h"
#include "model/Model.h"
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include "model/Camera.h"
#include "terrain/Terrain.h"
#include "skybox/SkyBox.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 lightPos (100.0f, 100.0f, 100.0f);

struct ProgramState{
    bool ImguiEnable= false;
    void LoadFromDisk(std::string path);
    void SaveToDisk(std::string path) ;
    Camera camera;
    glm::vec3 lightColor;
    glm::vec3 lightColor1;

    ProgramState()
    :camera(glm::vec3(0.0f,0.3f,5.5f)),lightColor(glm::vec3(1.0f,1.0f,1.0f))
    ,lightColor1(glm::vec3(1.0f,1.0f,1.0f)){}


};

void ProgramState::SaveToDisk(std::string path) {
    std::ofstream out(path);

    out<<ImguiEnable<<'\n'
    <<camera.Position.x<<'\n'
    <<camera.Position.y<<'\n'
    <<camera.Position.z<<'\n'
    <<camera.Front.x<<'\n'
    <<camera.Front.y<<'\n'
    <<camera.Front.z<<'\n'
    <<camera.Pitch<<'\n'
    <<camera.Yaw<<'\n'
    <<lightColor.r<<'\n'
    <<lightColor.g<<'\n'
    <<lightColor.b<<'\n'
    <<lightColor1.r<<'\n'
    <<lightColor1.g<<'\n'
    <<lightColor1.b;
}

void ProgramState::LoadFromDisk(std::string path) {
    std::ifstream in(path);

    if(in){
        in>>ImguiEnable
        >>camera.Position.x
        >>camera.Position.y
        >>camera.Position.z
        >>camera.Front.x
        >>camera.Front.y
        >>camera.Front.z
        >>camera.Pitch
        >>camera.Yaw
        >>lightColor.r
        >>lightColor.g
        >>lightColor.b
        >>lightColor1.r
        >>lightColor1.g
        >>lightColor1.b;
    }
}

glm::vec3 bulbPos[] = {
        glm::vec3(-5.0f, 4.2f, -6.75f),
        glm::vec3(8.03f, 4.2f, 7.75f)
};

ProgramState* programState;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods);
void DrawImgui(ProgramState* programState);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    programState = new ProgramState;
    programState->LoadFromDisk("../resources/programState.txt");
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(programState->ImguiEnable){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();(void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    Shader shaderProgram("../resources/shaders/model_load.vs", "../resources/shaders/model_load.fs");
    std::string bridgePath = std::filesystem::path("../resources/models/bridge.obj");
    std::string streetLampPath = std::filesystem::path("../resources/models/StreetLamp/StreetLamp.obj");
    std::string lightBulbPath = std::filesystem::path("../resources/models/LightBulb/lightBulb.obj");

    Model bridgeModel(bridgePath.c_str());
    Model streetLampModel(streetLampPath.c_str());
    Model lightBulbModel(lightBulbPath.c_str());

    Terrain terrain(0, 0, 25);
    TerrainModel terrainModel = terrain.generateTerrain();
    std::vector<std::string> skyboxFaces = {
            "../resources/skybox/daylight/right.bmp",
            "../resources/skybox/daylight/left.bmp",
            "../resources/skybox/daylight/top.bmp",
            "../resources/skybox/daylight/bottom.bmp",
            "../resources/skybox/daylight/back.bmp",
            "../resources/skybox/daylight/front.bmp"
    };

    SkyBox skyBox(skyboxFaces);

    Shader skyBoxShader("../resources/shaders/skybox.vs","../resources/shaders/skybox.fs");
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window)){
        glGetError();
        processInput(window);


        glClearColor(0.529f, 0.807f, 0.921f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        glm::mat4 projection = glm::perspective(glm::radians(programState->camera.Zoom),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);
        glm::mat4 view = programState->camera.GetViewMatrix();

        shaderProgram.setMat4("projection", projection);
        shaderProgram.setMat4("view", view);
        shaderProgram.setVec3("viewPos",programState->camera.Position);
        shaderProgram.setVec3("pointLights[0].position", bulbPos[0]);
        shaderProgram.setVec3("pointLights[0].color", programState->lightColor);
        shaderProgram.setFloat("pointLights[0].constant", 1.0);
        shaderProgram.setFloat("pointLights[0].linear",0.22);
        shaderProgram.setFloat("pointLights[0].quadratic", 0.20);
        shaderProgram.setVec3("pointLights[1].position", bulbPos[1]);
        shaderProgram.setVec3("pointLights[1].color", programState->lightColor1);
        shaderProgram.setFloat("pointLights[1].constant", 1.0);
        shaderProgram.setFloat("pointLights[1].linear",0.22);
        shaderProgram.setFloat("pointLights[1].quadratic", 0.20);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.6f,1.5f,0.0f));
        model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.05f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0, 0.0, 1.0));

        shaderProgram.setMat4("model", model);
        bridgeModel.Draw(shaderProgram);
        model = glm::mat4(1.0f);


        model = glm::scale(model,glm::vec3(0.5f,0.5f,0.5f));
        model = glm::translate(model,glm::vec3(16.0f,-1.0f,15.5f));
        model = glm::rotate(model,glm::radians(60.0f), glm::vec3(0.0,1.0,0.0));

        shaderProgram.setMat4("model", model);
        streetLampModel.Draw(shaderProgram);

        model = glm::mat4(1.0f);
        model = glm::scale(model,glm::vec3(0.5f,0.5f,0.5f));
        model = glm::translate(model, glm::vec3(-10.0f,-1.0f,-13.5f));
        model = glm::rotate(model,glm::radians(60.0f), glm::vec3(0.0,1.0,0.0));
        shaderProgram.setMat4("model", model);
        streetLampModel.Draw(shaderProgram);

        model = glm::mat4 (1.0f);
        model = glm::translate(model,bulbPos[0]);
        model = glm::scale(model,glm::vec3(0.07f,0.07f,0.07f));


        shaderProgram.setMat4("model",model);
        lightBulbModel.Draw(shaderProgram);

        model = glm::mat4 (1.0f);
        model = glm::translate(model,bulbPos[1]);
        model = glm::scale(model,glm::vec3(0.07f,0.07f,0.07f));


        shaderProgram.setMat4("model",model);
        lightBulbModel.Draw(shaderProgram);

        shaderProgram.setMat4("model",model);
        lightBulbModel.Draw(shaderProgram);




        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(-terrain.getSize()/2.0f * 0.25,-0.50f,-terrain.getSize()/2.0f * 0.25) );
        model = glm::scale(model, glm::vec3(0.25f,0.25f,0.25f));

        shaderProgram.setMat4("model", model);
        terrainModel.Draw(shaderProgram);

        glDepthFunc(GL_LEQUAL);
        skyBoxShader.use();
        skyBoxShader.setInt("skybox", 0);
        view = glm::mat4(glm::mat3(programState->camera.GetViewMatrix()));
        skyBoxShader.setMat4("view", view);
        skyBoxShader.setMat4("projection", projection);
        skyBox.Draw();
        glDepthFunc(GL_LESS);

        if(programState->ImguiEnable){
            DrawImgui(programState);
        }


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    programState->SaveToDisk("../resources/programState.txt");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete programState;
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
        if(programState->camera.Position.y > 0 )
            programState->camera.Position += speed * programState->camera.Front;
        else {
            if(programState->camera.Position.y <= 0)
                if(programState->camera.Front.y > 0 )
                    programState->camera.Position += speed * programState->camera.Front;
        }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)

        if(programState->camera.Position.y > 0 )
            programState->camera.Position -= speed * programState->camera.Front;
        else {
            if(programState->camera.Position.y <= 0)
                if(programState->camera.Front.y < 0 )
                    programState->camera.Position -= speed * programState->camera.Front;
        }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        programState->camera.Position -= glm::normalize(glm::cross(programState->camera.Front, programState->camera.Up)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        programState->camera.Position += glm::normalize(glm::cross(programState->camera.Front, programState->camera.Up)) * speed;
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
    if(programState->ImguiEnable == false){
        programState->camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    programState->camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void DrawImgui(ProgramState* programState){

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Test");
        ImGui::Text("CGraphics");
        ImGui::ColorEdit3("Bulb color",(float*)&programState->lightColor);
        ImGui::ColorEdit3("Bulb color1",(float*)&programState->lightColor1);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}
void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        programState->ImguiEnable = !programState->ImguiEnable;
        if (programState->ImguiEnable) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        }
    }

}
