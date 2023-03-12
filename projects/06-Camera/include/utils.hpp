#ifndef UTILS_H
#define UTILS_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

// Callback to resize Viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Process Input and how to handle it
void processInput(GLFWwindow* window);

int logShaderCompile(const unsigned int shaderId, const char* shaderType);

int logProgramLink(const unsigned int shaderProgram);
#endif
