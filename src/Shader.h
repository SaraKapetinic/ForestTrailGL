//
// Created by predrag on 18.2.22..
//

#ifndef CGRAPHICS_SHADER_H
#define CGRAPHICS_SHADER_H
#include <glm/glm.hpp>
#include "glad/glad.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

class Shader {
public:

    unsigned id;

    unsigned int getId() const;

    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setVec3(const std::string &name, glm::vec3& value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
};


#endif //CGRAPHICS_SHADER_H