//
// Created by predrag on 4.4.22..
//

#include "Renderer.h"
#include "TreeTranslation.h"

void Renderer::renderModels(Shader &shader) {
    shader.use();
    initializeShader(shader);

    //transformations for bridge
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.6f,1.5f,0.0f));
    model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.05f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0, 0.0, 1.0));
    shader.setMat4("model", model);
    models.at("bridge").Draw(shader);

    //transformations for lamp1
    model = glm::mat4(1.0f);
    model = glm::scale(model,glm::vec3(0.5f,0.5f,0.5f));
    model = glm::translate(model,glm::vec3(16.0f,-1.0f,15.5f));
    model = glm::rotate(model,glm::radians(60.0f), glm::vec3(0.0,1.0,0.0));
    shader.setMat4("model", model);
    models.at("lamp").Draw(shader);

    //transformations for lamp1

    model = glm::mat4(1.0f);
    model = glm::scale(model,glm::vec3(0.5f,0.5f,0.5f));
    model = glm::translate(model, glm::vec3(-10.0f,-1.0f,-13.5f));
    model = glm::rotate(model,glm::radians(60.0f), glm::vec3(0.0,1.0,0.0));
    shader.setMat4("model", model);
    models.at("lamp").Draw(shader);

    //transformations for light bulb1
    model = glm::mat4 (1.0f);
    model = glm::translate(model,ps.lightPosition[0]);
    model = glm::scale(model,glm::vec3(0.07f,0.07f,0.07f));
    shader.setMat4("model", model);
    models.at("lightBulb").Draw(shader);

    //transformations for light bulb1
    model = glm::mat4 (1.0f);
    model = glm::translate(model,ps.lightPosition[1]);
    model = glm::scale(model,glm::vec3(0.07f,0.07f,0.07f));
    shader.setMat4("model", model);
    models.at("lightBulb").Draw(shader);
}

void Renderer::renderTerrain(Shader &shader) {
    glm::mat4 model (1.0f);
    model = glm::translate(model,glm::vec3(-ps.terrainSize / 2.0f * 0.25, -0.50f, -ps.terrainSize / 2.0f * 0.25) );
    model = glm::scale(model, glm::vec3(0.25f,0.25f,0.25f));

    shader.use();
    shader.setMat4("model", model);

    initializeShader(shader);
    if(ps.isDay)
        shader.setFloat("ambientStrength", 0.00000005);
    terrainModel.Draw(shader);
}

void Renderer::renderScene() {
    renderModels(shaders.at("main"));
    renderTerrain(shaders.at("terrain"));
    renderWater(shaders.at("water"));
    renderInstancedModel(shaders.at("instance"));
    renderSkybox(shaders.at("skybox"));
}

void Renderer::renderWater(Shader &shader) {
    shader.use();
    initializeShader(shader);

    glm::mat4 model (1.0f);
    model = glm::translate(model, glm::vec3(-ps.waterSize/2.0f * 0.25f, -1.0f, -ps.waterSize/2.0f * 0.25f));
    model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
    shader.setMat4("model", model);
    waterModel.Draw(shader);
}

void Renderer::renderSkybox(Shader &shader) {
    glDepthFunc(GL_LEQUAL);
    shader.use();
    shader.setInt("daySkybox", 0);
    shader.setInt("nightSkybox", 1);
    view = glm::mat4(glm::mat3(ps.camera.GetViewMatrix()));
    view = glm::rotate(view, glm::radians((float)glfwGetTime()), glm::vec3(0.0f,1.0f,0.0f));
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setFloat("opacity", ps.skyboxOpacity);
    skyBox.Draw();
    glDepthFunc(GL_LESS);
}

void Renderer::renderInstancedModel(Shader &shader) {
    shader.use();
    initializeShader(shader);
    for(int i=0; i < treeTranslations.size();i++)
        shader.setVec3("position[" + std::to_string(i)+ "]", treeTranslations[i]);

    glm::mat4 model (1.0f);
    model = glm::scale(model, glm::vec3(0.15f));
    shader.setMat4("model", model);

    glDisable(GL_BLEND);
    if(ps.isDay)
        shader.setFloat("ambientStrength", 0.25);
    instancedModels.at("tree").Draw(shader, treeTranslations.size());
    glEnable(GL_BLEND);
}

void Renderer::initializeShader(Shader &shader) {
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setVec3("viewPos",ps.camera.Position);
    shader.setVec3("pointLights[0].position", ps.lightPosition[0]);
    shader.setVec3("pointLights[0].color", ps.lightColor[0]);
    shader.setFloat("pointLights[0].constant", 1.0);
    shader.setFloat("pointLights[0].linear",ps.linear);
    shader.setFloat("pointLights[0].quadratic", ps.quadratic);
    shader.setVec3("pointLights[1].position", ps.lightPosition[1]);
    shader.setVec3("pointLights[1].color", ps.lightColor[1]);
    shader.setFloat("pointLights[1].constant", 1.0);
    shader.setFloat("pointLights[1].linear",ps.linear);
    shader.setFloat("pointLights[1].quadratic", ps.quadratic);
    shader.setVec3("pointLights[2].position", ps.lightPosition[2]);
    shader.setVec3("pointLights[2].color", ps.lightColor[2]);
    shader.setFloat("pointLights[2].constant", 1.0);
    shader.setFloat("pointLights[2].linear",ps.linear);
    shader.setFloat("pointLights[2].quadratic", ps.quadratic);
    shader.setBool("isDay", ps.isDay);
    if(ps.isDay){
        shader.setFloat("ambientStrength", 0.5);
    }
    else shader.setFloat("ambientStrength", 8.0);
}

