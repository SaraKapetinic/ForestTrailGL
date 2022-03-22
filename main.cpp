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

Camera camera(glm::vec3(0.0f,0.3f,5.5f));
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 lightPos (100.0f, 100.0f, 100.0f);
glm::vec3 lightColor (1.0f,1.0f,1.0f);

bool ImguiEnable= false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods);
void DrawImgui();

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();(void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    Shader shaderProgram("../resources/shaders/model_load.vs", "../resources/shaders/model_load.fs");
    std::string bridgePath = std::filesystem::path("../resources/models/bridge.obj");
    std::string streetLampPath = std::filesystem::path("../resources/models/StreetLamp/StreetLamp.obj");

    Model bridgeModel(bridgePath.c_str());
    Model streetLampModel(streetLampPath.c_str());

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

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shaderProgram.setMat4("projection", projection);
        shaderProgram.setMat4("view", view);
        shaderProgram.setVec3("lightCol", lightColor);
        shaderProgram.setVec3("lightPos", lightPos);
        shaderProgram.setVec3("viewPos",camera.Position);
        

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

        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(-terrain.getSize()/2.0f * 0.25,-0.50f,-terrain.getSize()/2.0f * 0.25) );
        model = glm::scale(model, glm::vec3(0.25f,0.25f,0.25f));



        shaderProgram.setMat4("model", model);
        terrainModel.Draw(shaderProgram);

        glDepthFunc(GL_LEQUAL);
        skyBoxShader.use();
        skyBoxShader.setInt("skybox", 0);
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyBoxShader.setMat4("view", view);
        skyBoxShader.setMat4("projection", projection);
        skyBox.Draw();
        glDepthFunc(GL_LESS);

        if(ImguiEnable){
            DrawImgui();
        }


        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
        if(camera.Position.y > 0 )
            camera.Position += speed * camera.Front;
        else {
            if(camera.Position.y <= 0)
                if(camera.Front.y > 0 )
                    camera.Position += speed * camera.Front;
        }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)

        if(camera.Position.y > 0 )
            camera.Position -= speed * camera.Front;
        else {
            if(camera.Position.y <= 0)
                if(camera.Front.y < 0 )
                    camera.Position -= speed * camera.Front;
        }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * speed;
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
    if(ImguiEnable == false){
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void DrawImgui(){

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Test");
        ImGui::Text("CGraphics");
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}
void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods){
    if(key == GLFW_KEY_I && action == GLFW_PRESS){
        ImguiEnable = !ImguiEnable;
        if(ImguiEnable) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }else{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        }
    }
}
