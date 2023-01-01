#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
struct ShaderProgramSource {

    std::string VertexSource;
    std::string FragmentSource;
    std::string ComputeSource;

    bool isCompute;

};

static ShaderProgramSource ParseShader(const std::string& filepath);

class Shader {

public:
    unsigned int ID;
    ShaderProgramSource source;
    Shader();
    Shader(const std::string& filepath);
   

    void setUniforms(const char* uniform, glm::mat4& mvp);
    void Activate();
    void Delete();

   

};