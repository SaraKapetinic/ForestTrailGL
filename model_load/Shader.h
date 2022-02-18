//
// Created by predrag on 18.2.22..
//

#ifndef CGRAPHICS_SHADER_H
#define CGRAPHICS_SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned id;
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;
};


#endif //CGRAPHICS_SHADER_H
