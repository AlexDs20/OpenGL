#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "GLCommon.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <iostream>
#include <string.h>
#include <math.h>
#include <vector>

#include "shader.hpp"
#include "camera.hpp"
#include "utils.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "light.hpp"


std::string model_path("../projects/20-Assimp/resources/models/backpack/backpack.obj");

std::string obj_vertex("../projects/20-Assimp/resources/shaders/model.vs");
std::string obj_fragment("../projects/20-Assimp/resources/shaders/model.fs");

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Frame time (so that movement is same regardless of fps)
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(int argc, char** argv)
{
    // ------------------------------------
    //      Init glfw, glad, window
    // ------------------------------------
    // glfw: initialize and configure
    // ----------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _MAC
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT_GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent( window );
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    Model model(model_path.c_str());
    Shader objectShader(obj_vertex.c_str(), obj_fragment.c_str());

    glm::vec3 lightColor(1.0f);
    PointLight light = {
        .position = glm::vec3(0.0f, 0.0f, 3.0f),
        .ambient = 0.2f*lightColor,
        .diffuse = 0.5f*lightColor,
        .specular = lightColor,
        .linear = 0.09f,
        .quadratic = 0.032f,
    };

    objectShader.setPointLight(light);

    // wireframe mode, good for debugging
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float time = glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;

        // process inputs
        processInput(window);

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // move light
        light.position.x = 3.0f * glm::cos(time);
        light.position.y = 1.0f;
        light.position.z = 3.0f * glm::sin(time);
        objectShader.setPointLight(light);

        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 m = glm::mat4(1.0f);
        objectShader.set3f("viewPos", camera.Position);
        objectShader.setMat4f("proj", proj);
        objectShader.setMat4f("view", view);
        objectShader.setMat4f("model", m);

        objectShader.use();
        model.Draw(objectShader);


        // swap buffers and poll IO events (key pressed/released, ...)
        // -----------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clear all previous allocated GLFW resources
    // ------------------------------------------------------------

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
