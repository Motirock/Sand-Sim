#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char *filename);

class Shader {
public:
    Shader();
    Shader(const char *vertexFile, const char *fragmentFile);

    void setVec2(const std::string &name, const glm::vec2 &value);
    void setFloat(const std::string &name, float value);
    void setInt(const std::string &name, int value);
    void activateShader();
    void deleteShader();
    void compileErrors(unsigned int shader, const char *type);
    
    GLuint ID;
};

#endif