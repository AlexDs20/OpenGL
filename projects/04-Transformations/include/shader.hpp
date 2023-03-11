#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>

#include <string>
#include <glm/glm.hpp>


class Shader
{
  public:
    // program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);

    // Use this shade program
    void use();

    // set uniform values
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4f(const std::string &name, const glm::mat4 &mat) const;
};
#endif
