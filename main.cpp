#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "model/stb_image.h"
#include "model/Shader.h"
#include "model/Model.h"
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include "model/Camera.h"

Camera camera(glm::vec3(0.0f,0.0f,3.0f));
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    Shader bridgeShader("../resources/shaders/model_load.vs", "../resources/shaders/model_load.fs");
    std::string bridgePath = std::filesystem::path("../resources/models/bridge.obj");
    std::string streetLampPath = std::filesystem::path("../resources/models/Street_Lamp_1.obj");
    Shader streetLampShader("../resources/shaders/model_load.vs", "../resources/shaders/model_load.fs");
    Model bridgeModel(bridgePath.c_str());
    Model streetLampModel(streetLampPath.c_str());
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window)){

        processInput(window);

        glClearColor(0.921f, 0.419f, 0.776f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        streetLampShader.use();
        bridgeShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        bridgeShader.setMat4("projection",projection);
        bridgeShader.setMat4("view",view);
        streetLampShader.setMat4("projection",projection);
        streetLampShader.setMat4("view",view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
        model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));

        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
        model1 = glm::scale(model1,glm::vec3(1.0f,1.0f,1.0f));

        bridgeShader.setMat4("model", model);
        bridgeModel.Draw(bridgeShader);
        streetLampShader.setMat4("model1", model1);
        streetLampModel.Draw(streetLampShader);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float speed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Position += speed * camera.Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Position -= speed * camera.Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * speed;
}