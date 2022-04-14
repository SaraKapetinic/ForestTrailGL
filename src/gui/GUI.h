//
// Created by predrag on 24.3.22..
//

#ifndef FORESTTRAILGL_GUI_H
#define FORESTTRAILGL_GUI_H
#include "DataStructs.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

class GUI {

public:
    GUI(ProgramState& p, std::string sPath):ps(p),path(sPath){};
    void saveToDisk() {
        std::ofstream out(path);

        out<<ps.ImguiEnable<<'\n'
           <<ps.camera.Position.x<<'\n'
           <<ps.camera.Position.y<<'\n'
           <<ps.camera.Position.z<<'\n'
           <<ps.camera.Front.x<<'\n'
           <<ps.camera.Front.y<<'\n'
           <<ps.camera.Front.z<<'\n'
           <<ps.camera.Pitch<<'\n'
           <<ps.camera.Yaw<<'\n'
           <<ps.lightColor[0].r<<'\n'
           <<ps.lightColor[0].g<<'\n'
           <<ps.lightColor[0].b<<'\n'
           <<ps.lightColor[1].r<<'\n'
           <<ps.lightColor[1].g<<'\n'
           <<ps.lightColor[1].b<<'\n'
           <<ps.linear<<'\n'
           <<ps.quadratic << '\n'
           <<ps.shadows << '\n'
           <<ps.lightIndex << '\n'
           <<ps.isDay << '\n'
           <<ps.enableAntialiasing << '\n'
           <<ps.skyboxOpacity;
    };

    void loadFromDisk() {
       std::ifstream in(path);
       if(in) {
           in>>ps.ImguiEnable
             >>ps.camera.Position.x
             >>ps.camera.Position.y
             >>ps.camera.Position.z
             >>ps.camera.Front.x
             >>ps.camera.Front.y
             >>ps.camera.Front.z
             >>ps.camera.Pitch
             >>ps.camera.Yaw
             >>ps.lightColor[0].r
             >>ps.lightColor[0].g
             >>ps.lightColor[0].b
             >>ps.lightColor[1].r
             >>ps.lightColor[1].g
             >>ps.lightColor[1].b
             >>ps.linear
             >>ps.quadratic
             >>ps.shadows
             >>ps.lightIndex
             >>ps.isDay
             >>ps.enableAntialiasing
             >>ps.skyboxOpacity;
       }
    };
    void initImGui(GLFWwindow* window){
        loadFromDisk();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if(ps.ImguiEnable){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();(void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window,true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    void deinitImgui(){
        saveToDisk();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void DrawImgui(){

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGuiIO io = ImGui::GetIO();
            float display_width = (float)io.DisplaySize.x;
            float display_height = (float)io.DisplaySize.y;
            float pos_x = display_width*0.6;
            float pos_y = display_height*0.7;

            ImGui::SetNextWindowPos(ImVec2(pos_x,pos_y),ImGuiCond_FirstUseEver);
            ImGui::Begin("ForestTrailGL");
            ImGui::ColorEdit3("Bulb color",(float*)&ps.lightColor[0]);
            ImGui::ColorEdit3("Bulb color1",(float*)&ps.lightColor[1]);

            float& linear = ps.linear;
            float& quadratic = ps.quadratic;

            ImGui::Text("Linear intensity");
            if(ImGui::RadioButton("0.7", linear == 0.7f)){
                linear = 0.7f;
            }
            else if(ImGui::RadioButton("0.22", linear == 0.22f)) {
                linear = 0.22f;
            }
            else if(ImGui::RadioButton("0.09",linear == 0.09f)){
                linear = 0.09f;
            }

            ImGui::Text("Quadratic intensity");
            if(ImGui::RadioButton("1.8",quadratic == 1.8f)){
                quadratic = 1.8f;
            }
            else if(ImGui::RadioButton("0.2",quadratic == 0.2f)){
                quadratic = 0.2f;
            }
            else if (ImGui::RadioButton("0.032",quadratic == 0.032f)){
                quadratic = 0.032f;
            }

            ImGui::Checkbox("Enable Antialiasing",&ps.enableAntialiasing);
            if(ps.enableAntialiasing){

                ps.enableAntialiasing = true;
            }else{

                ps.enableAntialiasing = false;
            }

            ImGui::Checkbox("Enable Shadows", &ps.shadows);
            if(ps.shadows){
                ps.shadows = true;
            }
            else{
                ps.shadows = false;
            }

            if(ps.shadows){
                if(ImGui::RadioButton("Lamp 1", ps.lightIndex == 0))
                    ps.lightIndex = 0;
                else if(ImGui::RadioButton("Lamp 2", ps.lightIndex == 1))
                    ps.lightIndex = 1;
            }

            ImGui::End();
        }

        {
            ImGuiIO io = ImGui::GetIO();
            float display_width = (float)io.DisplaySize.x;
            float display_height = (float)io.DisplaySize.y;
            float pos_x = display_width*0.6;
            float pos_y = display_height*0.3;

            ImGui::SetNextWindowPos(ImVec2(pos_x,pos_y),ImGuiCond_FirstUseEver);
            ImGui::Begin("Camera");
            auto &c = ps.camera;
            ImGui::Text("Camera position: (%f, %f, %f)",c.Position.x,c.Position.y,c.Position.z);
            ImGui::Text("Camera pitch: %f",c.Pitch);
            ImGui::Text("Camera yaw: %f",c.Yaw);
            ImGui::Text("Camera front: (%f, %f, %f)",c.Front.x,c.Front.y,c.Front.z);
            //ImGui::Checkbox("Enable camera movement on mouse",&ps.EnableMouseMovement);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

private:
    ProgramState& ps;
public:
    ProgramState &getPs() const {
        return ps;
    }

private:
    std::string path;

};

#endif //FORESTTRAILGL_GUI_H
